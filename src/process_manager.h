#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <queue>
#include <string>
#include <fcntl.h>
#include "tree_directory.h"

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
 * 操作命令枚举
 */
enum class OperationCommand {
    // 创建文件夹
    CREATE_FOLDER = 1,
    // 删除文件夹
    DELETE_FOLDER = 2,
    // 创建文件
    CREATE_FILE = 3,
    // 删除文件
    DELETE_FILE = 4,
    // 进行读写（需要进行内存分配）
    CREATE_READ_WRITE = 5,
    // 重命名文件夹
    RENAME_FOLDER = 6,
    // 重命名文件
    RENAME_FILE = 7,
    // 退出
    EXIT = 0
};

/**
 * 文件信息结构体
 */
struct FileInfo {
    File *file;
    Folder *folder;
    std::string *fileName;
    std::string *data;

    // 默认构造方法
    FileInfo() = default;

    // 构造方法1:创建新文件，需要folder和fileName
    FileInfo(Folder *folder, string *fileName) {
        this->folder = folder;
        this->fileName = fileName;
    }

    // 构造方法2:删除文件夹，只要一个folder
    explicit FileInfo(Folder *folder) {
        this->folder = folder;
    }

    // 构造方法3:删除文件，只要一个file
    explicit FileInfo(File *file) {
        this->file = file;
    }

    // 构造方法4:重命名文件，需要folder、file和fileName
    FileInfo(Folder *folder, File *file, string *fileName) {
        this->folder = folder;
        this->file = file;
        this->fileName = fileName;
    }

};

/**
 * 命名管道结构体
 */
struct NamedPipe {
    std::string pipeName;
    int fileDescriptor;

    explicit NamedPipe(std::string name);

    ~NamedPipe();

    // 写数据到命名管道
    void writeData(const std::string &data) const;

    // 从命名管道读取数据
    std::string readData() const;
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
    FileInfo fileInfo{};         // 文件信息
    OperationCommand command;    // 操作命令

    Process(string &name, int pid, int priority, ProcessState state, ProcessType type);

    // 重载小于运算符，用于比较 Process 对象的优先级
    bool operator<(const Process &other) const;

    // 虚函数 execute,用于执行进程
    virtual void execute() = 0;

    // 虚函数 destroy,用于销毁进程
    virtual void destroy() = 0;

    // 获取进程状态的str
    [[nodiscard]] string getProcessStateStr() const;
};

/**
 * 进程管理类
 */
class ProcessManager {
public:
    std::vector<Process *> processList;
    std::priority_queue<Process *> readyQueue;
    std::queue<Process *> blockQueue;

    // 从进程列表中删除该进程
    void deleteProcess(int pid);
};

/**
 * 初始化进程,直到操作系统关闭才会释放内存
 */
class InitProcess : public Process {
private:
    // 构造方法
    InitProcess(string &name, int pid, int priority, ProcessType type);

    Folder *folder;

public:
    static InitProcess create(string name, int pid, int priority, ProcessType processType);

    void execute() override;

    void destroy() override;

    Folder *get_folder();
};

/**
 * 数据生成进程
 */
class DataGenerationProcess : public Process {
public:
    // 构造方法
    DataGenerationProcess(string &name, int pid, int priority, ProcessState state, ProcessType type);

    static void create(string name, int pid, int priority, FileInfo *fileInfo, OperationCommand command);

    void execute() override;

    void destroy() override;
};

/**
 * 数据删除进程
 */
class DataDeletionProcess : public Process {
public:
    // 构造方法
    DataDeletionProcess(string &name, int pid, int priority, ProcessState state, ProcessType type);

    static void create(string name, int pid, int priority, FileInfo *fileInfo, OperationCommand command);

    void execute() override;

    void destroy() override;
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
};

/**
 * 执行进程
 */
class ExecutionProcess : public Process {
public:
    // 构造方法
    ExecutionProcess(string &name, int pid, int priority, ProcessState state, ProcessType type);

    static void sendData(const std::string &pipeName, const std::string &data);

    static std::string receiveData(const std::string &pipeName);

    void renew(const std::string& pipeName);

    static void create(string name, int pid, int priority, FileInfo *fileInfo, OperationCommand command);

    void execute() override;

    void destroy() override;

    static void execute_user_input_command(File *file, ExecutionProcess *executionProcess);
};

/**
 * PID生成器
 */
class PIDGenerator {
private:
    static int counter;
    static std::mutex mtx;

public:
    static int generatePID();
};

/**
 * 任务调度类
 */
class TaskScheduler {
public:
    static TaskScheduler &getInstance() {
        static TaskScheduler instance;
        return instance;
    }

    std::vector<std::thread> threads; // 存储线程对象的容器

    void createThreads(int numThreads);

    void joinThreads();

    void schedule();

private:
    TaskScheduler() = default;

    ~TaskScheduler() = default;

    TaskScheduler(const TaskScheduler &) = delete;

    TaskScheduler &operator=(const TaskScheduler &) = delete;

    TaskScheduler(TaskScheduler &&) = delete;

    TaskScheduler &operator=(TaskScheduler &&) = delete;
};

vector<int> show_process_record();

vector<bool> show_disk_block_status();

vector<int> show_group_block_status();

vector<Process *> get_process_list();