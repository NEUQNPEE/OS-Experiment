#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <queue>
#include <string>
#include <fcntl.h>
using namespace std;

/**
 * 进程状态枚举
 */
enum class ProcessState {
    READY,
    RUNNING,
    BLOCKED,
    FINISHED
};

/**
 * 队列类型枚举
 */
enum class QueueType {
    READY_QUEUE,
    BLOCKED_QUEUE
};

/**
 * 进程类型枚举,实际上一般的操作系统不会简单的将进程值分为这三类
 */
enum class ProcessType {
    DATA_GENERATION_PROCESS = 1,
    DATA_DELETION_PROCESS = 2,
    EXECUTION_PROCESS = 3,
    INIT_PROCESS = 0
};

/**
 * 命名管道结构体
 */
struct NamedPipe {
    std::string pipeName;
    int fileDescriptor;

    NamedPipe(const std::string& name) : pipeName(name) {
        // 创建或打开文件作为命名管道
        fileDescriptor = open(pipeName.c_str(), O_RDWR | O_CREAT, 0666);
    }

    ~NamedPipe() {
        // 关闭文件描述符
        close(fileDescriptor);
        // 删除文件
        unlink(pipeName.c_str());
    }

    // 写数据到命名管道
    void writeData(const std::string& data) {
        write(fileDescriptor, data.c_str(), data.size());
    }

    // 从命名管道读取数据
    std::string readData() {
        const int bufferSize = 1024;
        char buffer[bufferSize];
        ssize_t bytesRead = read(fileDescriptor, buffer, bufferSize - 1);
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0'; // 添加字符串结束符
            return std::string(buffer);
        }
        return "";
    }
};

/**
 * 进程类
 */
class Process {
public:
    string name;                 // 进程名
    int pid;                     // 进程ID
    int priority;                // 进程优先级
    ProcessState state;          // 进程状态
    ProcessType type;            // 进程类型
    vector<int> allocatedMemory; // 内存块地址
    string directory;            //文件存储信息
    string fileName;

    Process(string &name, int pid, int priority, ProcessState state, ProcessType type) {
        this->name = name;
        this->pid = pid;
        this->priority = priority;
        this->state = state;
        this->type = type;
    }

    // 重载小于运算符，用于比较 Process 对象的优先级
    bool operator<(const Process &other) const {
        return priority < other.priority;
    }

    // 虚函数 create,用于创建进程
    //  virtual void create() = 0;
    // 虚函数 execute,用于执行进程
    virtual void execute() = 0;

    // 虚函数 destroy,用于销毁进程
    virtual void destroy() = 0;
};

/**
 * 进程管理类
 */
class ProcessManager {
public:
    std::vector<Process *> processList;
    std::priority_queue<Process *> readyQueue;
    std::priority_queue<Process *> blockQueue;

    // 从进程列表中删除该进程
    void deleteProcess(int pid) {
        for (int i = 0; i < processList.size(); i++) {
            if (processList[i]->pid == pid) {
                processList.erase(processList.begin() + i);
                break;
            }
        }
    }
};

class DirectoryManager {
public:
    // 项目初始化,将初始块的数据
    static vector<int> init() {
        return {1, 2, 3};
    }

    // 将初始块数据读入内存,返回内存块地址
    static vector<int> read_data_to_memory(vector<int> &initInfo) {
        return {4, 5};
    }

    // 释放内存,参数为内存块地址
    static void releaseMemory(vector<int> &allocatedMemory) {
        cout << "release memory" << endl;
    }

    // 申请外存块,参数为所需大小
    static int allocateBlocks(int dataSize) {
        return 1;
    }

    // 将数据存入磁盘块,参数为数据信息,目录,文件名
    static void saveData(const std::string &dataInfo, const std::string &directory, const std::string &fileName) {
        cout << "save data" << endl;
    }

    // 为其在目录中建立目录项,参数为目录,文件名,分配的磁盘块
    static void createDirectoryEntry(const std::string &directory, const std::string &fileName, int allocatedBlocks) {
        cout << "create directory entry" << endl;
    }

    // 更新空闲盘块信息
    static void updateFreeBlocks() {
        cout << "update free blocks" << endl;
    }

    // 从目录中删除目录项并释放磁盘块,参数为目录,文件名
    static void deleteDirectoryEntry(const std::string &directory, const std::string &fileName) {
        cout << "delete directory entry" << endl;
    }

    // 申请内存块,参数为所需大小
    static vector<int> allocateMemory(int dataSize) {
        return {1, 2, 3};
    }

    // 从磁盘块中读取数据到内存块,如果内存块不够,模拟页面调度,更新页表
    static void read_data_to_memory(const std::string &directory, const std::string &fileName) {
        cout << "read data to memory" << endl;
    }

    // 读取数据,参数为内存块地址
    static std::string read_data(vector<int> &allocatedMemory) {
        return "this is a test!";
    }

    // 将数据写入内存块,参数为数据信息,内存块地址
    static void save_data(const std::string &dataInfo, vector<int> &allocatedMemory) {
        cout << "save data" << endl;
    }

    // 将数据写入磁盘块,参数为内存块地址
    static void write_data_to_disk(vector<int> &allocatedMemory) {
        cout << "write data to disk" << endl;
    }
};

DirectoryManager directoryManager;
ProcessManager processManager;

/**
 * 初始化进程,直到操作系统关闭才会释放内存
 */
class InitProcess : public Process {
private:
    // 构造方法
    InitProcess(string &name, int pid, int priority, ProcessType type)
            : Process(name, pid, priority, ProcessState::RUNNING, type) {}

public:
    static InitProcess create(string name, int pid, int priority, ProcessType processType) {
        auto* initProcess=new InitProcess(name, pid, priority, processType);
        // 将该进程放入进程列表
        processManager.processList.push_back(initProcess);
        return *initProcess;
    }

    void execute() override {
        vector<int> initInfo = DirectoryManager::init();
        allocatedMemory = DirectoryManager::read_data_to_memory(initInfo);
    }

    void destroy() override {
        // 释放内存
        DirectoryManager::releaseMemory(allocatedMemory);
        // 从进程列表中删除该进程
        processManager.deleteProcess(this->pid);
        // 销毁对象
        delete this;
    }
};

/**
 * 数据生成进程
 */
class DataGenerationProcess : public Process {
public:
    // 构造方法
    DataGenerationProcess(string &name, int pid, int priority, ProcessState state, ProcessType type)
            : Process(name, pid, priority, state, type) {}

    static void
    create(string name, int pid, int priority, const std::string &directory, const std::string &fileName) {
        auto* dataGenerationProcess=new DataGenerationProcess(name, pid, priority, ProcessState::READY,
                                                    ProcessType::DATA_GENERATION_PROCESS);
        dataGenerationProcess->fileName = fileName;
        dataGenerationProcess->directory = directory;
        // 将该进程放入进程列表,就绪队列
        processManager.processList.push_back(dataGenerationProcess);
        processManager.readyQueue.push(dataGenerationProcess);
    }

    void execute() override {
        //todo 从外界获取dataSize和dataInfo,这里假设直接给出
        int dataSize = 8;
        string dataInfo;
        // 申请所需大小的外存块
        int allocatedBlocks = DirectoryManager::allocateBlocks(dataSize);

        if (allocatedBlocks < dataSize) {
            // 盘块不够，给出提示
            std::cout << "Error: Not enough free disk blocks for data generation." << std::endl;
            return;
        }

        // 将数据存入磁盘块
        DirectoryManager::saveData(dataInfo, directory, fileName);

        // 调用目录管理功能为其在目录中建立目录项
        DirectoryManager::createDirectoryEntry(directory, fileName, allocatedBlocks);

        // 更改空闲盘块信息
        DirectoryManager::updateFreeBlocks();
    }

    void destroy() override {
        // 从进程列表中删除该进程
        processManager.deleteProcess(this->pid);
        // 销毁对象
        delete this;
    }
};

/**
 * 数据删除进程
 */
class DataDeletionProcess : public Process {
public:
    // 构造方法
    DataDeletionProcess(string &name, int pid, int priority, ProcessState state, ProcessType type)
            : Process(name, pid, priority, state, type) {}

    static void
    create(string name, int pid, int priority, const std::string &directory, const std::string &fileName) {
        auto* dataDeletionProcess=new DataDeletionProcess(name, pid, priority, ProcessState::READY,
                                                ProcessType::DATA_DELETION_PROCESS);
        dataDeletionProcess->fileName = fileName;
        dataDeletionProcess->directory = directory;
        // 将该进程放入进程列表,就绪队列
        processManager.processList.push_back(dataDeletionProcess);
        processManager.readyQueue.push(dataDeletionProcess);
    }

    void execute() override {
        // 从目录中删除目录项并释放磁盘块,需要注意的是,如果当前数据在内存中,则不能删除
        DirectoryManager::deleteDirectoryEntry(directory, fileName);

        // 更改空闲盘块信息
        DirectoryManager::updateFreeBlocks();
    }

    void destroy() override {
        // 从进程列表中删除该进程
        processManager.deleteProcess(this->pid);
        // 销毁对象
        delete this;
    }
};

/**
 * 用户输入指令枚举
 */
enum class UserInputCommand {
    // 退出
    EXIT = 0,
    // 读取数据
    READ_DATA = 1,
    // 写入数据
    WRITE_DATA = 2,
    // 保存数据
    SAVE = 3,
};

/**
 * 执行进程
 */
class ExecutionProcess : public Process {
public:
    // 构造方法
    ExecutionProcess(string &name, int pid, int priority, ProcessState state, ProcessType type)
            : Process(name, pid, priority, state, type) {}

    void send_data_to_pipe(const std::string&data , const std::string&pipeName){
        NamedPipe pipe(pipeName);
        pipe.writeData(data);
    }
    std::string receive_data_from_pipe(const std::string&pipeName){
        NamedPipe pipe(pipeName);
        return pipe.readData();
    }
    static void create(string name, int pid, int priority, const std::string &directory, const std::string &fileName) {
        auto* executionProcess=new ExecutionProcess(name, pid, priority, ProcessState::READY, ProcessType::EXECUTION_PROCESS);
        executionProcess->directory = directory;
        executionProcess->fileName = fileName;
        // 将该进程放入进程列表,就绪队列
        processManager.processList.push_back(executionProcess);
        processManager.readyQueue.push(executionProcess);
    }

    void execute() override {
        std::vector<int> allocatedMemory = DirectoryManager::allocateMemory(8);
        if (allocatedMemory.empty()) {
            std::cout << "Error: Not enough free memory for execution." << std::endl;
            return;
        }
        // 根据文件存储信息,将数据从外存读入内存块,如果内存块不够,模拟页面调度,更新页表
        DirectoryManager::read_data_to_memory(directory, fileName);
        execute_user_input_command(allocatedMemory);
    }

    void destroy() override {
        DirectoryManager::releaseMemory(allocatedMemory);
        processManager.deleteProcess(this->pid);
        delete this;
    }

    // 模拟执行用户输入的命令,参数为内存块地址
    static void execute_user_input_command(std::vector<int> &allocatedMemory) {
        int userInput = 0;
        while (true) {
            std::cout << "waiting for user input..." << std::endl;
            std::cin >> userInput;
            switch (static_cast<UserInputCommand>(userInput)) {
                case UserInputCommand::READ_DATA:
                    // 读取数据,如果该内存块的锁是读锁还是写锁,如果是写锁,则等待
                    std::cout << "Data read from memory: " << DirectoryManager::read_data(allocatedMemory) << std::endl;
                    break;
                case UserInputCommand::WRITE_DATA:
                    // 写入数据,等待QT界面用户输入,这里模拟一下数据吧,如果该内存块的锁是读锁还是写锁,如果是写锁,则等待
                    std::cout << "Data written to memory." << std::endl;
                    DirectoryManager::save_data("this is a test!", allocatedMemory);
                    break;
                case UserInputCommand::SAVE:
                    // 保存数据,如果该内存块的锁是读锁还是写锁,如果是写锁,则等待
                    DirectoryManager::write_data_to_disk(allocatedMemory);
                    break;
                default:
                    break;
            }
        }
    }
};

/**
 * PID生成器
 */
class PIDGenerator {
private:
    static int counter;
    static std::mutex mtx;

public:
    static int generatePID() {
        std::lock_guard<std::mutex> lock(mtx); // 使用互斥锁确保线程安全
        return counter++;
    }
};

int PIDGenerator::counter = 1;
std::mutex PIDGenerator::mtx;


/**
 * 指令枚举
*/
enum class Command {
    // 创建进程
    CREATE_PROCESS = 1,
    // 退出
    EXIT = 2
};

/**
 * 任务调度类
*/
class TaskScheduler {
private:
    std::mutex mutex_;  // 用于保护共享资源
public:
    std::vector<std::thread> threads;  // 存储线程对象的容器

    void createThreads(int numThreads) {
        // 创建多个线程并加入容器
        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back(&TaskScheduler::schedule, this);
        }
    }

    void joinThreads() {
        // 等待所有线程结束
        for (auto &thread: threads) {
            thread.join();
        }
    }

    void schedule() {
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

};

// int main() {
//     // 创建Init进程用于初始化
//     TaskScheduler taskScheduler;
//     InitProcess initProcess = InitProcess::create("init", PIDGenerator::generatePID(), 0, ProcessType::INIT_PROCESS);
//     initProcess.execute();
//     // 循环等待指令输入
//     while (true) {
//         if (!processManager.readyQueue.empty()) {
//              taskScheduler.createThreads(1);
//             // taskScheduler.joinThreads();
//         }
//         int commandInput = 0;
//         std::cout << "waiting for command input..." << std::endl;
//         std::cin >> commandInput;
//         switch (static_cast<Command>(commandInput)) {
//             case Command::CREATE_PROCESS:
//                 int processType;
//                 std::cout << "Please input process type: " << std::endl;
//                 std::cin >> processType;
//                 // 创建进程
//                 //todo 从QT获取directory和fileName,这里加上直接给出
//                 switch (static_cast<ProcessType>(processType)) {
//                     case ProcessType::DATA_GENERATION_PROCESS: {
//                         std::string directory_input;
//                         std::string fileName_input;
//                         int pid = PIDGenerator::generatePID();
//                         DataGenerationProcess::create(
//                                 &"dataGenerationProcess"[pid], pid, 1, directory_input, fileName_input);
//                         break;
//                     }
//                     case ProcessType::DATA_DELETION_PROCESS: {
//                         std::string directory_input;
//                         std::string fileName_input;
//                         int pid = PIDGenerator::generatePID();
//                         DataDeletionProcess::create(
//                                 &"dataDeletionProcess"[pid], PIDGenerator::generatePID(), 1, directory_input,
//                                 fileName_input);
//                         break;
//                     }
//                     case ProcessType::EXECUTION_PROCESS: {
//                         std::string directory_input;
//                         std::string fileName_input;
//                         int pid = PIDGenerator::generatePID();
//                         ExecutionProcess::create(&"executionProcess"[pid], pid, 1,
//                                                                                      directory_input, fileName_input);
//                         break;
//                     }
//                     default:
//                         break;
//                 }
//                 break;
//             case Command::EXIT:
//                 // 销毁进程,释放内存
//                 //找到所有进程,调用destroy方法,第一个进程是init进程,最后销毁
//                 for (int i = 1; i < processManager.processList.size(); i++) {
//                     processManager.processList[i]->destroy();
//                 }
//                 processManager.processList[0]->destroy();
//                 return 0;
//             default:
//                 break;
//         }
//         // taskScheduler.joinThreads();
//     }
// }