#include <utility>

#include "process_manager.h"


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
    write(fileDescriptor, data.c_str(), data.size());
}

// 从命名管道读取数据
std::string NamedPipe::readData() const {
    const int bufferSize = 1024;
    char buffer[bufferSize];
    ssize_t bytesRead = read(fileDescriptor, buffer, bufferSize - 1);
    if (bytesRead > 0) {
        buffer[bytesRead] = '\0'; // 添加字符串结束符
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

// 重载小于运算符，用于比较 Process 对象的优先级
bool Process::operator<(const Process &other) const {
    return priority < other.priority;
}


// 从进程列表中删除该进程
void ProcessManager::deleteProcess(int pid) {
    for (int i = 0; i < processList.size(); i++) {
        if (processList[i]->pid == pid) {
            processList.erase(processList.begin() + i);
            break;
        }
      
    }
}

// // 构造方法
// InitProcess::InitProcess(string &name, int pid, int priority, ProcessType type)
//         : Process(name, pid, priority, ProcessState::RUNNING, type) {}

// InitProcess InitProcess::create(string name, int pid, int priority, ProcessType processType) {
//     auto *initProcess = new InitProcess(name, pid, priority, processType);
//     // 将该进程放入进程列表
//     processManager.processList.push_back(initProcess);
//     return *initProcess;
// }

// void InitProcess::execute() {
//     Folder *folder = init();
//     //todo 在这里向QT发送folder指针
// }

// void InitProcess::destroy() {
//     clearBlock_ids(this->pid);
//     // 从进程列表中删除该进程
//     processManager.deleteProcess(this->pid);
//     // 销毁对象
//     delete this;
// }


// // 构造方法
// DataGenerationProcess::DataGenerationProcess(string &name, int pid, int priority, ProcessState state, ProcessType type)
//         : Process(name, pid, priority, state, type) {}

// void DataGenerationProcess::
// create(string name, int pid, int priority, FileInfo *fileInfo, OperationCommand command) {
//     auto *dataGenerationProcess = new DataGenerationProcess(name, pid, priority, ProcessState::READY,
//                                                             ProcessType::DATA_GENERATION_PROCESS);
//     dataGenerationProcess->fileInfo.fileName = fileInfo->fileName;
//     dataGenerationProcess->fileInfo.folder = fileInfo->folder;
//     dataGenerationProcess->command = command;
//     // 将该进程放入进程列表,就绪队列
//     processManager.processList.push_back(dataGenerationProcess);
//     processManager.readyQueue.push(dataGenerationProcess);
// }

// void DataGenerationProcess::execute() {
//     judge_is_repeat(fileInfo.folder, *fileInfo.fileName);
//     switch (command) {
//         case OperationCommand::CREATE_FOLDER: {
//             add_folder(fileInfo.folder, *fileInfo.fileName);
//             break;
//         }
//         case OperationCommand::CREATE_FILE: {
//             add_file(fileInfo.folder, *fileInfo.fileName);
//             break;
//         }
//         case OperationCommand::RENAME_FOLDER: {
//             folder_change_name(fileInfo.folder, *fileInfo.fileName);
//             break;
//         }
//         case OperationCommand::RENAME_FILE: {
//             file_change_name(fileInfo.file, *fileInfo.fileName);
//             break;
//         }
//         default:
//             break;
//     }
// }

// void DataGenerationProcess::destroy() {
//     // 从进程列表中删除该进程
//     processManager.deleteProcess(this->pid);
//     // 销毁对象
//     delete this;
// }

// void DataGenerationProcess::judge_is_repeat(Folder *folder, std::string str) {
//     if (is_repeat(folder, std::move(str))) {
//         //todo 在这里向QT发送重名信息
//         return;
//     }
// }

// // 构造方法
// DataDeletionProcess::DataDeletionProcess(string &name, int pid, int priority, ProcessState state, ProcessType type)
//         : Process(name, pid, priority, state, type) {}

// void DataDeletionProcess::
// create(string name, int pid, int priority, FileInfo *fileInfo, OperationCommand command) {
//     auto *dataDeletionProcess = new DataDeletionProcess(name, pid, priority, ProcessState::READY,
//                                                         ProcessType::DATA_DELETION_PROCESS);
//     dataDeletionProcess->fileInfo.fileName = fileInfo->fileName;
//     dataDeletionProcess->fileInfo.folder = fileInfo->folder;
//     dataDeletionProcess->command = command;
//     // 将该进程放入进程列表,就绪队列
//     processManager.processList.push_back(dataDeletionProcess);
//     processManager.readyQueue.push(dataDeletionProcess);
// }

// void DataDeletionProcess::execute() {
//     switch (command) {
//         case OperationCommand::DELETE_FOLDER: {
//             delete_folder(fileInfo.folder);
//             break;
//         }
//         case OperationCommand::DELETE_FILE: {
//             delete_file(fileInfo.file);
//             break;
//         }
//         default:
//             break;
//     }
// }

// void DataDeletionProcess::destroy() {
//     // 从进程列表中删除该进程
//     processManager.deleteProcess(this->pid);
//     // 销毁对象
//     delete this;
// }

// // 构造方法
// ExecutionProcess::ExecutionProcess(string &name, int pid, int priority, ProcessState state, ProcessType type)
//         : Process(name, pid, priority, state, type) {}

// void ExecutionProcess::send_data_to_pipe(const std::string &data, const std::string &pipeName) {
//     NamedPipe pipe(pipeName);
//     pipe.writeData(data);
// }

// std::string ExecutionProcess::receive_data_from_pipe(const std::string &pipeName) {
//     NamedPipe pipe(pipeName);
//     return pipe.readData();
// }

// void ExecutionProcess::create(string name, int pid, int priority, FileInfo *fileInfo, OperationCommand command) {
//     auto *executionProcess = new ExecutionProcess(name, pid, priority, ProcessState::READY,
//                                                   ProcessType::EXECUTION_PROCESS);
//     executionProcess->fileInfo.file = fileInfo->file;
//     executionProcess->command = command;
//     //todo 申请内存

//     // 将该进程放入进程列表,就绪队列
//     processManager.processList.push_back(executionProcess);
//     processManager.readyQueue.push(executionProcess);
// }

// void ExecutionProcess::execute() {
//     execute_user_input_command(fileInfo.file);
// }

// void ExecutionProcess::destroy() {
//     clearBlock_ids(this->pid);
//     processManager.deleteProcess(this->pid);
//     delete this;
// }

// // 模拟执行用户输入的命令,参数为内存块地址
// void ExecutionProcess::execute_user_input_command(File *file) {
//     while (true) {
//         //todo 从QT不停获取用户输入的指令，比如读，写
//         UserInputCommand userInputCommand;
//         switch (userInputCommand) {
//             case UserInputCommand::READ_DATA: {
//                 std::string data = look_file_content(file);
//                 //todo 在这里向QT发送data
//                 break;
//             }
//             case UserInputCommand::WRITE_DATA: {
//                 //todo 从QT获取用户输入的data
//                 std::string data;
//                 file_change_content(file, data);
//                 break;
//             }
//             case UserInputCommand::EXIT:

//                 return;
//             default:
//                 break;
//         }
//     }
// }


int PIDGenerator::generatePID() {
    std::lock_guard<std::mutex> lock(mtx); // 使用互斥锁确保线程安全
    return counter++;
}


int PIDGenerator::counter = 1;
std::mutex PIDGenerator::mtx;


void TaskScheduler::createThreads(int numThreads) {
    // 创建多个线程并加入容器
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(&TaskScheduler::schedule, this);
    }
}

void TaskScheduler::joinThreads() {
    // 等待所有线程结束
    for (auto &thread: threads) {
        thread.join();
    }
}

void TaskScheduler::schedule() {
    std::lock_guard<std::mutex> lock(mutex_);
    // 从就绪队列中取出优先级最高的进程
    Process *process = processManager.readyQueue.top();
    process->state = ProcessState::RUNNING;
    // 执行进程
    cout << "process " << process->name << " start" << endl;
    process->execute();
    // 执行完毕后,将进程状态置为FINISHED
    process->state = ProcessState::FINISHED;
    // 从就绪队列中删除该进程
    processManager.readyQueue.pop();
    // 销毁进程
    process->destroy();
}

// int main() {
//     // 创建Init进程用于初始化
//     TaskScheduler taskScheduler;
//     InitProcess::create("init", PIDGenerator::generatePID(), 0, ProcessType::INIT_PROCESS).execute();
//     // 循环等待指令输入
//     while (true) {
//         if (!processManager.readyQueue.empty()) {
//             taskScheduler.createThreads(1);
//             // taskScheduler.joinThreads();
//         }
//         int commandInput = 0;
//         //todo 在这里从QT获取commandInput
//         std::cout << "waiting for command input..." << std::endl;
//         std::cin >> commandInput;
//         switch (static_cast<OperationCommand>(commandInput)) {
//             case OperationCommand::CREATE_FOLDER: {
//                 //todo 从QT获取目录的指针folder和文件名str
//                 Folder *folder;
//                 std::string file_name;
//                 FileInfo fileInfo = {nullptr, folder, &file_name, nullptr};
//                 int pid = PIDGenerator::generatePID();
//                 DataGenerationProcess::create(
//                         &"dataGenerationProcess"[pid], pid, 1, &fileInfo, OperationCommand::CREATE_FOLDER);
//                 break;
//             }
//             case OperationCommand::DELETE_FOLDER: {
//                 //todo 从QT获取待删除文件夹的指针
//                 Folder *folder;
//                 FileInfo fileInfo = {nullptr, folder, nullptr, nullptr};
//                 int pid = PIDGenerator::generatePID();
//                 DataDeletionProcess::create(
//                         &"dataGenerationProcess"[pid], pid, 1, &fileInfo, OperationCommand::DELETE_FOLDER);
//                 break;
//             }
//             case OperationCommand::CREATE_FILE: {
//                 //todo 从QT获取目录的指针folder和文件名str
//                 Folder *folder;
//                 std::string file_name;
//                 FileInfo fileInfo = {nullptr, folder, nullptr, nullptr};
//                 int pid = PIDGenerator::generatePID();
//                 DataGenerationProcess::create(
//                         &"dataGenerationProcess"[pid], pid, 1, &fileInfo, OperationCommand::CREATE_FILE);
//                 break;
//             }
//             case OperationCommand::DELETE_FILE: {
//                 //todo 从QT获取待删除文件的指针
//                 File *file;
//                 FileInfo fileInfo = {file, nullptr, nullptr, nullptr};
//                 int pid = PIDGenerator::generatePID();
//                 DataDeletionProcess::create(
//                         &"dataGenerationProcess"[pid], pid, 1, &fileInfo, OperationCommand::DELETE_FILE);
//                 break;
//             }
//             case OperationCommand::CREATE_READ_WRITE: {
//                 //todo 从QT获取待读写文件的指针
//                 File *file;
//                 FileInfo fileInfo = {file, nullptr, nullptr, nullptr};
//                 int pid = PIDGenerator::generatePID();
//                 ExecutionProcess::create(
//                         &"executionProcess"[pid], pid, 1, &fileInfo, OperationCommand::CREATE_READ_WRITE);
//             }
//             case OperationCommand::RENAME_FOLDER: {
//                 //todo 从QT获取待重命名文件夹的指针和新的文件夹名
//                 Folder *folder;
//                 std::string new_name;
//                 FileInfo fileInfo = {nullptr, folder, &new_name, nullptr};
//                 int pid = PIDGenerator::generatePID();
//                 DataGenerationProcess::create(
//                         &"dataGenerationProcess"[pid], pid, 1, &fileInfo, OperationCommand::RENAME_FOLDER);
//             }
//             case OperationCommand::RENAME_FILE: {
//                 //todo 从QT获取待重命名文件的指针和新的文件名
//                 File *file;
//                 std::string new_name;
//                 FileInfo fileInfo = {file, nullptr, &new_name, nullptr};
//                 int pid = PIDGenerator::generatePID();
//                 DataGenerationProcess::create(
//                         &"dataGenerationProcess"[pid], pid, 1, &fileInfo, OperationCommand::RENAME_FILE);
//             }
//             case OperationCommand::EXIT: {
//                 // 销毁进程,释放内存
//                 //找到所有进程,调用destroy方法,第一个进程是init进程,最后销毁
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

