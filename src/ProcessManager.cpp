#include <utility>

#include "memory_for_process.h"
#include "process_for_others.h"

ProcessManager processManager;

NamedPipe::NamedPipe(std::string name) : pipeName(std::move(name)) {
    // 创建或打开文件作为命名管道
    fileDescriptor = open(pipeName.c_str(), O_RDWR | O_CREAT, 0666);
}

NamedPipe::~NamedPipe() {
    // 关闭文件描述符
    close(fileDescriptor);
    // 删除文件
    unlink(pipeName.c_str());
}

// 写数据到命名管道
void NamedPipe::writeData(const std::string &data) const {
    int a = write(fileDescriptor, data.c_str(), data.size());
}

// 从命名管道读取数据
std::string NamedPipe::readData() const {
    const int bufferSize = 1024;
    char buffer[bufferSize];
    ssize_t bytesRead = read(fileDescriptor, buffer, bufferSize - 1);
    std::cout << bytesRead << std::endl;
    if (bytesRead > 0) {
        buffer[bytesRead] = '\0'; // 添加字符串结束符
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

// 重载小于运算符，用于比较 Process 对象的优先级
bool Process::operator<(const Process &other) const {
    return priority < other.priority;
}

string Process::getProcessStateStr() const {
    switch (state) {
        case ProcessState::RUNNING:
            return "运行";
        case ProcessState::READY:
            return "就绪";
        case ProcessState::BLOCKED:
            return "阻塞";
        case ProcessState::FINISHED:
            return "结束";
        default:
            return "";
    }
}

// 从进程列表中删除该进程
void deleteProcess(int pid) {
    processManager.processMap.erase(pid);
}

// 构造方法
InitProcess::InitProcess(string name, int pid, ProcessType type)
        : Process(name, pid, ProcessState::RUNNING, type) {}

Folder *InitProcess::execute_init() {
    return init_root();
}

void pass_args_to_init_process(InitProcess *process, int priority) {
    process->priority = priority;
    processManager.processMap.insert(std::pair<int, Process *>(process->pid, process));
}


// 构造方法
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

// 构造方法
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

// 构造方法
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
    //根据pid获取执行进程
    return look_file_content(path, pid);
}

bool execute_write(std::string path, int pid, std::string content) {
    file_change_content(path, content, pid);
    vector pids = look_open_file(path);
    //向所有打开该文件的进程发送数据
    if (pids.size() > 1) {
        ExecutionProcess::sendData(to_string(pid), content);
        for (int i: pids) {
            //从map中找到该进程并调用renew方法
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
    //将data更新为从管道获取的内容
//    std::string newData = receiveData(pipeName);
//    executionProcess->fileInfo->data = &newData;
}

bool destroy_process(Process *process) {
    if (process->state == ProcessState::RUNNING) {
        return false;
    }
    if (process->type == ProcessType::EXECUTION_PROCESS) {
        // 从用户文件表中删除对应的进程占用
        close_file(process->path, process->pid);
        clearMemoryBlock(process->pid);
        // 将阻塞队列中的进程放入就绪队列
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
    //名称为进程类型+pid
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
            //此处本应该抛出异常
            return nullptr;
    }
}

int PIDGenerator::generatePID() {
    std::lock_guard<std::mutex> lock(mtx); // 使用互斥锁确保线程安全
    return counter++;
}


int PIDGenerator::counter = 1;
std::mutex PIDGenerator::mtx;

void schedule() {
    // 从就绪队列中取出优先级最高的进程
    Process *process = processManager.readyQueue.top();
    process->state = ProcessState::RUNNING;
    // 执行进程
    process->execute();
    // 执行完毕后,将进程状态置为FINISHED
    process->state = ProcessState::FINISHED;
    // 从就绪队列中删除该进程
    processManager.readyQueue.pop();
    // 销毁进程
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
    //返回map中的所有value
    vector<Process *> processes;
    processes.reserve(processManager.processMap.size());
    for (auto &it: processManager.processMap) {
        processes.push_back(it.second);
    }
    return processes;
}