#include <utility>

#include "process_manager.h"
#include "memoryBlock.h"

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
    // std::cout << a << std::endl;
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


Process::Process(string &name, int pid, int priority, ProcessState state, ProcessType type) {
    this->name = name;
    this->pid = pid;
    this->priority = priority;
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
void ProcessManager::deleteProcess(int pid) {
    for (int i = 0; i < processList.size(); i++) {
        if (processList[i]->pid == pid) {
            processList.erase(processList.begin() + i);
            break;
        }

    }
}

// ���췽��
InitProcess::InitProcess(string &name, int pid, int priority, ProcessType type)
        : Process(name, pid, priority, ProcessState::RUNNING, type) {}

InitProcess InitProcess::create(string name, int pid, int priority, ProcessType processType) {
    auto *initProcess = new InitProcess(name, pid, priority, processType);
    initProcess->state = ProcessState::RUNNING;

    // ���ý��̷�������б�
    processManager.processList.push_back(initProcess);
    return *initProcess;
}

void InitProcess::execute() {
    folder = init_root();
    //todo ��������QT����folderָ��
}

void InitProcess::destroy() {
//    clearBlock_ids(this->pid);
    // �ӽ����б���ɾ���ý���
    processManager.deleteProcess(this->pid);
    // ���ٶ���
    delete this;
}

Folder *InitProcess::get_folder() {
    return folder;
}

// ���췽��
DataGenerationProcess::DataGenerationProcess(string &name, int pid, int priority, ProcessState state, ProcessType type)
        : Process(name, pid, priority, state, type) {}

// ���췽��������Ϊ��������������id���������ȼ����ļ���Ϣ������ָ��
void DataGenerationProcess::
create(string name, int pid, int priority, FileInfo *fileInfo, OperationCommand command) {
    auto *dataGenerationProcess = new DataGenerationProcess(name, pid, priority, ProcessState::READY,
                                                            ProcessType::DATA_GENERATION_PROCESS);

    dataGenerationProcess->fileInfo = new FileInfo();
    dataGenerationProcess->fileInfo->fileName = fileInfo->fileName;
    dataGenerationProcess->fileInfo->folder = fileInfo->folder;
    dataGenerationProcess->fileInfo->file = fileInfo->file;
    dataGenerationProcess->command = command;
    // ���ý��̷�������б�,��������
    processManager.processList.push_back(dataGenerationProcess);
    processManager.readyQueue.push(dataGenerationProcess);
}

void DataGenerationProcess::execute() {
    switch (command) {
        case OperationCommand::CREATE_FOLDER: {
            folder_add_folder(fileInfo->folder, *fileInfo->fileName);
            break;
        }
        case OperationCommand::CREATE_FILE: {
            folder_add_file(fileInfo->folder, *fileInfo->fileName);
            break;
        }
        case OperationCommand::RENAME_FOLDER: {
            if (folder_is_repeat(fileInfo->folder, *fileInfo->fileName)) {
                //todo ��������QT����������Ϣ
                return;
            }
            folder_change_name(fileInfo->folder, *fileInfo->fileName);
            break;
        }
        case OperationCommand::RENAME_FILE: {
            if (file_is_repeat(fileInfo->folder, *fileInfo->fileName)) {
                //todo ��������QT����������Ϣ
                return;
            }
            file_change_name(fileInfo->file, *fileInfo->fileName);
            break;
        }
        default:
            break;
    }
}

void DataGenerationProcess::destroy() {
    // �ӽ����б���ɾ���ý���
    processManager.deleteProcess(this->pid);
    // ���ٶ���
    delete this;
}

// ���췽��
DataDeletionProcess::DataDeletionProcess(string &name, int pid, int priority, ProcessState state, ProcessType type)
        : Process(name, pid, priority, state, type) {}

bool DataDeletionProcess::
create(string name, int pid, int priority, FileInfo *fileInfo, OperationCommand command) {
    if(command==OperationCommand::DELETE_FILE&&!look_open_file(fileInfo->file).empty()){
        return false;
    }
    auto *dataDeletionProcess = new DataDeletionProcess(name, pid, priority, ProcessState::READY,
                                                        ProcessType::DATA_DELETION_PROCESS);

    dataDeletionProcess->fileInfo = new FileInfo();                              
    dataDeletionProcess->fileInfo->fileName = fileInfo->fileName;
    dataDeletionProcess->fileInfo->folder = fileInfo->folder;
    dataDeletionProcess->fileInfo->file = fileInfo->file;
    dataDeletionProcess->command = command;
    // ���ý��̷�������б�,��������
    processManager.processList.push_back(dataDeletionProcess);
    processManager.readyQueue.push(dataDeletionProcess);
    return true;
}

void DataDeletionProcess::execute() {
    switch (command) {
        case OperationCommand::DELETE_FOLDER: {
            delete_folder(fileInfo->folder);
            break;
        }
        case OperationCommand::DELETE_FILE: {
            delete_file(fileInfo->file);
            break;
        }
        default:
            break;
    }
}

void DataDeletionProcess::destroy() {
    // �ӽ����б���ɾ���ý���
    processManager.deleteProcess(this->pid);
    // ���ٶ���
    delete this;
}

// ���췽��
ExecutionProcess::ExecutionProcess(string &name, int pid, int priority, ProcessState state, ProcessType type)
        : Process(name, pid, priority, state, type) {}


ExecutionProcess ExecutionProcess::create(string name, int pid, int priority, FileInfo *fileInfo, OperationCommand command) {
    auto *executionProcess = new ExecutionProcess(name, pid, priority, ProcessState::READY,
                                                  ProcessType::EXECUTION_PROCESS);
    executionProcess->fileInfo = fileInfo;
    executionProcess->command = command;
    // ���ý��̷�������б�,��������
    processManager.processList.push_back(executionProcess);
    // �����ڴ�
    if (initialBlock_ids(executionProcess->pid)[0] == -1) {
        //todo ��������QT�����ڴ�������Ϣ

        //������������
        processManager.blockQueue.push(executionProcess);
        executionProcess->state = ProcessState::BLOCKED;
        return *executionProcess;
    }
    processManager.readyQueue.push(executionProcess);

    return *executionProcess;
}

void ExecutionProcess::execute() {
    // this->state = ProcessState::RUNNING;
    // execute_read(fileInfo.file, this);
    // execute_user_input_command(fileInfo.file, this);
}

void ExecutionProcess::destroy() {
    // ���û��ļ�����ɾ����Ӧ�Ľ���ռ��
    close_file(fileInfo->file, this->pid);
    clearBlock_ids(this->pid);
    processManager.deleteProcess(this->pid);
    // �����������еĽ��̷����������
    if (!processManager.blockQueue.empty()) {
        processManager.readyQueue.push(processManager.blockQueue.front());
        processManager.blockQueue.front()->state = ProcessState::READY;
        processManager.blockQueue.pop();
    }
    delete this;
}

void ExecutionProcess::execute_read(File *file, ExecutionProcess *executionProcess){
    std::string read_data = look_file_content(file, executionProcess->pid);
    executionProcess->fileInfo->data=&read_data;
}

void ExecutionProcess::execute_write(File *file, ExecutionProcess *executionProcess){
    file_change_content(file, *executionProcess->fileInfo->data, executionProcess->pid);
    vector pids =look_open_file(file);
    if(pids.size()>1){
        ExecutionProcess::sendData(to_string(executionProcess->pid), *executionProcess->fileInfo->data);
        for (int pid : pids) {
            if(pid!=executionProcess->pid){
                //�ӽ����б����ҵ��ý���
                for (auto & j : processManager.processList) {
                    if(j->pid==pid){
                        auto* executionProcess1=dynamic_cast<ExecutionProcess*>(j);
                        executionProcess1->renew(to_string(executionProcess->pid),executionProcess1);
                    }
                }
            }
        }
    }
}

// // ģ��ִ���û����������,����Ϊ�ڴ���ַ
// void ExecutionProcess::execute_user_input_command(File *file, ExecutionProcess *executionProcess) {
//     while (true) {
//         if(!processManager.commandQueue.empty()){
//             int command = processManager.commandQueue.front();
//             processManager.commandQueue.pop();
//             if (command == 0) {
//                 // �˳�
//                 return;
//             } else if (command == 1) {
//                 // д������
//                 file_change_content(file, *executionProcess->fileInfo.data, executionProcess->pid);
//                 vector pids =look_open_file(file);
//                 if(pids.size()>1){
//                     ExecutionProcess::sendData(to_string(this->pid), *executionProcess->fileInfo.data);
//                     for (int pid : pids) {
//                         if(pid!=this->pid){
//                             //�ӽ����б����ҵ��ý���
//                             for (auto & j : processManager.processList) {
//                                 if(j->pid==pid){
//                                     auto* executionProcess1=dynamic_cast<ExecutionProcess*>(j);
//                                     executionProcess1->renew(to_string(this->pid));
//                                 }
//                             }
//                         }
//                     }
//                 }
//             }
//         }
//     }
// }

void ExecutionProcess::sendData(const string &pipeName, const string &data) {
    NamedPipe(pipeName).writeData(data);
}

std::string ExecutionProcess::receiveData(const string &pipeName) {
    return NamedPipe(pipeName).readData();
}

void ExecutionProcess::renew(const std::string& pipeName,ExecutionProcess *executionProcess) {
    //��data����Ϊ�ӹܵ���ȡ������
    std::string newData=receiveData(pipeName);
    executionProcess->fileInfo->data=&newData;
}


int PIDGenerator::generatePID() {
    std::lock_guard<std::mutex> lock(mtx); // ʹ�û�����ȷ���̰߳�ȫ
    return counter++;
}


int PIDGenerator::counter = 1;
std::mutex PIDGenerator::mtx;


void TaskScheduler::createThreads(int numThreads) {
    // ��������̲߳���������
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(&TaskScheduler::schedule, this);
    }
}

void TaskScheduler::joinThreads() {
    // �ȴ������߳̽���
    for (auto &thread: threads) {
        thread.join();
    }
}

void TaskScheduler::schedule() {
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
    process->destroy();
}

void TaskScheduler::end()
{
    Process *process = processManager.readyQueue.top();
    processManager.readyQueue.pop();
    process->destroy();
}

// int main() {
//     // ����Init�������ڳ�ʼ��
//     TaskScheduler taskScheduler;
//     InitProcess::create("init", PIDGenerator::generatePID(), 0, ProcessType::INIT_PROCESS).execute();
//     // ѭ���ȴ�ָ������
//     while (true) {
//         if (!processManager.readyQueue.empty()) {
//             taskScheduler.createThreads(1);
//             // taskScheduler.joinThreads();
//         }
//         int commandInput = 0;
//         //todo �������QT��ȡcommandInput
//         std::cout << "waiting for command input..." << std::endl;
//         std::cin >> commandInput;
//         switch (static_cast<OperationCommand>(commandInput)) {
//             case OperationCommand::CREATE_FOLDER: {
//                 //todo ��QT��ȡĿ¼��ָ��folder���ļ���str
//                 Folder *folder;
//                 std::string file_name;
//                 FileInfo fileInfo = {nullptr, folder, &file_name, nullptr};
//                 int pid = PIDGenerator::generatePID();
//                 DataGenerationProcess::create(
//                         &"dataGenerationProcess"[pid], pid, 1, &fileInfo, OperationCommand::CREATE_FOLDER);
//                 break;
//             }
//             case OperationCommand::DELETE_FOLDER: {
//                 //todo ��QT��ȡ��ɾ���ļ��е�ָ��
//                 Folder *folder;
//                 FileInfo fileInfo = {nullptr, folder, nullptr, nullptr};
//                 int pid = PIDGenerator::generatePID();
//                 DataDeletionProcess::create(
//                         &"dataGenerationProcess"[pid], pid, 1, &fileInfo, OperationCommand::DELETE_FOLDER);
//                 break;
//             }
//             case OperationCommand::CREATE_FILE: {
//                 //todo ��QT��ȡĿ¼��ָ��folder���ļ���str
//                 Folder *folder;
//                 std::string file_name;
//                 FileInfo fileInfo = {nullptr, folder, nullptr, nullptr};
//                 int pid = PIDGenerator::generatePID();
//                 DataGenerationProcess::create(
//                         &"dataGenerationProcess"[pid], pid, 1, &fileInfo, OperationCommand::CREATE_FILE);
//                 break;
//             }
//             case OperationCommand::DELETE_FILE: {
//                 //todo ��QT��ȡ��ɾ���ļ���ָ��
//                 File *file;
//                 FileInfo fileInfo = {file, nullptr, nullptr, nullptr};
//                 int pid = PIDGenerator::generatePID();
//                 DataDeletionProcess::create(
//                         &"dataGenerationProcess"[pid], pid, 1, &fileInfo, OperationCommand::DELETE_FILE);
//                 break;
//             }
//             case OperationCommand::CREATE_READ_WRITE: {
//                 //todo ��QT��ȡ����д�ļ���ָ��
//                 File *file;
//                 FileInfo fileInfo = {file, nullptr, nullptr, nullptr};
//                 int pid = PIDGenerator::generatePID();
//                 ExecutionProcess::create(
//                         &"executionProcess"[pid], pid, 1, &fileInfo, OperationCommand::CREATE_READ_WRITE);
//             }
//             case OperationCommand::RENAME_FOLDER: {
//                 //todo ��QT��ȡ���������ļ��е�ָ����µ��ļ�����
//                 Folder *folder;
//                 std::string new_name;
//                 FileInfo fileInfo = {nullptr, folder, &new_name, nullptr};
//                 int pid = PIDGenerator::generatePID();
//                 DataGenerationProcess::create(
//                         &"dataGenerationProcess"[pid], pid, 1, &fileInfo, OperationCommand::RENAME_FOLDER);
//             }
//             case OperationCommand::RENAME_FILE: {
//                 //todo ��QT��ȡ���������ļ���ָ����µ��ļ���
//                 File *file;
//                 std::string new_name;
//                 FileInfo fileInfo = {file, nullptr, &new_name, nullptr};
//                 int pid = PIDGenerator::generatePID();
//                 DataGenerationProcess::create(
//                         &"dataGenerationProcess"[pid], pid, 1, &fileInfo, OperationCommand::RENAME_FILE);
//             }
//             case OperationCommand::EXIT: {
//                 // ���ٽ���,�ͷ��ڴ�
//                 //�ҵ����н���,����destroy����,��һ��������init����,�������
//                 for (int i = 1; i < processManager.processList.size(); i++) {
//                     processManager.processList[i]->destroy();
//                 }
//                 processManager.processList[0]->destroy();
//                 return 0;
//             }

//         }
//         // taskScheduler.joinThreads();
//     }
// }
vector<int> show_process_record() {
    return getProcessRecord();
}

vector<bool> show_disk_block_status() {
    return memory_get_disk_block_status();
}

vector<int> show_group_block_status() {
    return memory_get_group_block_status();
}

vector<Process *> get_process_list() {
    return processManager.processList;
}