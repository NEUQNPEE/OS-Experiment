#include <utility>

#include "memory_for_process.h"
#include "process_for_others.h"

ProcessManager processManager;

NamedPipe::NamedPipe(std::string name) : pipeName(std::move(name)) {
    // ��������ļ���Ϊ�����ܵ�
    fileDescriptor = open(pipeName.c_str(), O_RDWR | O_CREAT, 0666);
}

NamedPipe::~NamedPipe() {
    // �ر��ļ�������
    close(fileDescriptor);
    // ɾ���ļ�
    unlink(pipeName.c_str());
}

// д���ݵ������ܵ�
void NamedPipe::writeData(const std::string &data) const {
    int a = write(fileDescriptor, data.c_str(), data.size());
}

// �������ܵ���ȡ����
std::string NamedPipe::readData() const {
    const int bufferSize = 1024;
    char buffer[bufferSize];
    ssize_t bytesRead = read(fileDescriptor, buffer, bufferSize - 1);
    std::cout << bytesRead << std::endl;
    if (bytesRead > 0) {
        buffer[bytesRead] = '\0'; // ����ַ���������
        return std::string(buffer);
    }
    return "";
}

Process::Process(string name, int pid, ProcessState state, ProcessType type) {
    this->name = name;
    this->pid = pid;
    this->state = state;
    this->type = type;
}

// ����С������������ڱȽ� Process ��������ȼ�
bool Process::operator<(const Process &other) const {
    return priority < other.priority;
}

string Process::getProcessStateStr() const {
    switch (state) {
        case ProcessState::RUNNING:
            return "����";
        case ProcessState::READY:
            return "����";
        case ProcessState::BLOCKED:
            return "����";
        case ProcessState::FINISHED:
            return "����";
        default:
            return "";
    }
}

// �ӽ����б���ɾ���ý���
void deleteProcess(int pid) {
    processManager.processMap.erase(pid);
}

// ���췽��
InitProcess::InitProcess(string name, int pid, ProcessType type)
        : Process(name, pid, ProcessState::RUNNING, type) {}

Folder *InitProcess::execute_init() {
    return init_root();
}

void pass_args_to_init_process(InitProcess *process, int priority) {
    process->priority = priority;
    processManager.processMap.insert(std::pair<int, Process *>(process->pid, process));
}


// ���췽��
DataGenerationProcess::DataGenerationProcess(string name, int pid, ProcessState state, ProcessType type)
        : Process(name, pid, state, type) {}

void pass_args_to_generation_process(DataGenerationProcess *process, int priority, OperationCommand command,
                                     std::string fName, std::string path) {
    process->priority = priority;
    process->command = command;
    process->fName = fName;
    process->path = path;
    processManager.processMap.insert(std::pair<int, Process *>(process->pid, process));
    processManager.readyQueue.push(process);
}

void DataGenerationProcess::execute() {
    switch (command) {
        case OperationCommand::CREATE_FOLDER: {
            folder_add_folder(path, fName);
            break;
        }
        case OperationCommand::CREATE_FILE: {
            folder_add_file(path, fName);
            break;
        }
        case OperationCommand::RENAME_FOLDER: {
            if (folder_is_repeat(path, fName)) {
                return;
            }
            folder_change_name(path, fName);
            break;
        }
        case OperationCommand::RENAME_FILE: {
            if (file_is_repeat(path, fName)) {
                return;
            }
            file_change_name(path, fName);
            break;
        }
        default:
            break;
    }
}

// ���췽��
DataDeletionProcess::DataDeletionProcess(string name, int pid, ProcessState state, ProcessType type)
        : Process(name, pid, state, type) {}

bool pass_args_to_deletion_process(DataDeletionProcess *process, int priority, OperationCommand command,
                                   std::string fName, std::string path) {
    if (command == OperationCommand::DELETE_FILE && !look_open_file(path).empty()) {
        return false;
    }
    process->priority = priority;
    process->command = command;
    process->fName = fName;
    process->path = path;
    processManager.processMap.insert(std::pair<int, Process *>(process->pid, process));
    processManager.readyQueue.push(process);
    return true;
}

void DataDeletionProcess::execute() {
    switch (command) {
        case OperationCommand::DELETE_FOLDER: {
            delete_folder(path);
            break;
        }
        case OperationCommand::DELETE_FILE: {
            delete_file(path);
            break;
        }
        default:
            break;
    }
}

// ���췽��
ExecutionProcess::ExecutionProcess(string name, int pid, ProcessState state, ProcessType type)
        : Process(name, pid, state, type) {}


void pass_args_to_execution_process(ExecutionProcess *process, int priority, OperationCommand command,
                                    std::string fName, std::string parentFolderPath, int blockNum) {
    process->priority = priority;
    process->command = command;
    process->fName = fName;
    process->path = parentFolderPath;
    processManager.processMap.insert(std::pair<int, Process *>(process->pid, process));
    vector<int> blocks = assignMemoryBlock(process->pid, blockNum);
    if (blocks[0] == -1) {
        processManager.blockQueue.push(process);
        process->state = ProcessState::BLOCKED;
        return;
    } else {
        process->allocatedMemory = blocks;
    }
    processManager.readyQueue.push(process);
}

void ExecutionProcess::execute() {
}

std::string execute_read(std::string path, int pid) {
    //����pid��ȡִ�н���
    return look_file_content(path, pid);
}

bool execute_write(std::string path, int pid, std::string content) {
    file_change_content(path, content, pid);
    vector pids = look_open_file(path);
    //�����д򿪸��ļ��Ľ��̷�������
    if (pids.size() > 1) {
        ExecutionProcess::sendData(to_string(pid), content);
        for (int i: pids) {
            //��map���ҵ��ý��̲�����renew����
            auto *executionProcess1 = dynamic_cast<ExecutionProcess *>(processManager.processMap[i]);
            ExecutionProcess::renew(to_string(i), executionProcess1);
        }
    }
    return true;
}

void ExecutionProcess::sendData(const string &pipeName, const string &data) {
    NamedPipe(pipeName).writeData(data);
}

std::string ExecutionProcess::receiveData(const string &pipeName) {
    return NamedPipe(pipeName).readData();
}

void ExecutionProcess::renew(const std::string &pipeName, ExecutionProcess *executionProcess) {
    //��data����Ϊ�ӹܵ���ȡ������
//    std::string newData = receiveData(pipeName);
//    executionProcess->fileInfo->data = &newData;
}

bool destroy_process(Process *process) {
    if (process->state == ProcessState::RUNNING) {
        return false;
    }
    if (process->type == ProcessType::EXECUTION_PROCESS) {
        // ���û��ļ�����ɾ����Ӧ�Ľ���ռ��
        close_file(process->path, process->pid);
        clearMemoryBlock(process->pid);
        // �����������еĽ��̷����������
        if (!processManager.blockQueue.empty()) {
            processManager.readyQueue.push(processManager.blockQueue.front());
            processManager.blockQueue.front()->state = ProcessState::READY;
            processManager.blockQueue.pop();
        }
    }
    deleteProcess(process->pid);
    delete process;
    return true;
}

Process *create_process(ProcessType type) {
    int pid = PIDGenerator::generatePID();
    //����Ϊ��������+pid
    string name;
    switch (type) {
        case ProcessType::DATA_GENERATION_PROCESS:
            return new DataGenerationProcess(name + "DATA_GENERATION_PROCESS" + to_string(pid), pid,
                                             ProcessState::READY,
                                             type);
        case ProcessType::DATA_DELETION_PROCESS:
            return new DataDeletionProcess(name + "DATA_DELETION_PROCESS" + to_string(pid), pid,
                                           ProcessState::READY,
                                           type);
        case ProcessType::EXECUTION_PROCESS:
            return new ExecutionProcess(name + "EXECUTION_PROCESS" + to_string(pid), pid,
                                        ProcessState::READY,
                                        type);
        case ProcessType::INIT_PROCESS:
            return new InitProcess(name + "INIT_PROCESS" + to_string(pid), pid, type);
        default:
            //�˴���Ӧ���׳��쳣
            return nullptr;
    }
}

int PIDGenerator::generatePID() {
    std::lock_guard<std::mutex> lock(mtx); // ʹ�û�����ȷ���̰߳�ȫ
    return counter++;
}


int PIDGenerator::counter = 1;
std::mutex PIDGenerator::mtx;

void schedule() {
    // �Ӿ���������ȡ�����ȼ���ߵĽ���
    Process *process = processManager.readyQueue.top();
    process->state = ProcessState::RUNNING;
    // ִ�н���
    process->execute();
    // ִ����Ϻ�,������״̬��ΪFINISHED
    process->state = ProcessState::FINISHED;
    // �Ӿ���������ɾ���ý���
    processManager.readyQueue.pop();
    // ���ٽ���
    destroy_process(process);
}

void end() {
    Process *process = processManager.readyQueue.top();
    processManager.readyQueue.pop();
    destroy_process(process);
}

vector<int> show_process_record() {
    return getProcessRecord();
}

vector<bool> show_disk_block_status() {
    return memory_get_disk_block_status();
}

vector<int> show_group_block_status() {
    return memory_get_group_block_status();
}

vector<Process *> get_process_map() {
    //����map�е�����value
    vector<Process *> processes;
    processes.reserve(processManager.processMap.size());
    for (auto &it: processManager.processMap) {
        processes.push_back(it.second);
    }
    return processes;
}