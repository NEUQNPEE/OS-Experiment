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
 * 命名管道结构体
 */

// tip 命名管道的主要代码部分由文件系统完成，进程只需要调用即可

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
    std::string fName;           // 文件/文件夹名
    std::string path;            // 文件夹/文件路径
    OperationCommand command;    // 操作命令

    Process(string name, int pid, ProcessState state, ProcessType type);

    // 默认构造方法
    Process() = default;

    // 重载小于运算符，用于比较 Process 对象的优先级
    bool operator<(const Process &other) const;

    // 虚函数 execute,用于执行进程
    virtual void execute() {
    }

    // 获取进程状态的str
    [[nodiscard]] string getProcessStateStr() const;
};

/**
 * 进程管理类
 */
class ProcessManager {
public:
    std::map<int, Process *> processMap;
    std::priority_queue<Process *> readyQueue;
    std::queue<Process *> blockQueue;


};

/**
 * 从进程管理类中删除进程
 * @param pid  进程pid
 */
void deleteProcess(int pid);

/**
 * 初始化进程,直到操作系统关闭才会释放内存
 */
class InitProcess : public Process {
public:
    InitProcess(string name, int pid, ProcessType type);

    static Folder *execute_init();
};

/**
 * 数据生成进程
 */
class DataGenerationProcess : public Process {
public:
    DataGenerationProcess(string name, int pid, ProcessState state, ProcessType type);

    void execute() override;

};

/**
 * 数据删除进程
 */
class DataDeletionProcess : public Process {
public:
    DataDeletionProcess(string name, int pid, ProcessState state, ProcessType type);

    void execute() override;
};


/**
 * 执行进程
 */
class ExecutionProcess : public Process {
public:
    ExecutionProcess(string name, int pid, ProcessState state, ProcessType type);

    ExecutionProcess() = default;

    static void sendData(const std::string &pipeName, const std::string &data);

    static std::string receiveData(const std::string &pipeName);

    static void renew(const std::string &pipeName, ExecutionProcess *executionProcess);

    void execute() override;
};

/**
 * 删除进程
 * @param process 进程
 * @return  是否删除成功
 */
bool destroy_process(Process *process);

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

// tip 模拟进程调度的分时间片：假设正在执行的进程都不阻断，只有非运行的进程能够被分时间片换走
// tip 通过引入时间机制，让前端展示的内容动态变化

class TaskScheduler {
public:
    static TaskScheduler &getInstance() {
        static TaskScheduler instance;
        return instance;
    }

private:
    TaskScheduler() = default;

    ~TaskScheduler() = default;

    TaskScheduler(const TaskScheduler &) = delete;

    TaskScheduler &operator=(const TaskScheduler &) = delete;

    TaskScheduler(TaskScheduler &&) = delete;

    TaskScheduler &operator=(TaskScheduler &&) = delete;
};