#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <queue>
#include <string>
#include <fcntl.h>
using namespace std;

/**
 * ����״̬ö��
 */
enum class ProcessState {
    READY,
    RUNNING,
    BLOCKED,
    FINISHED
};

/**
 * ��������ö��
 */
enum class QueueType {
    READY_QUEUE,
    BLOCKED_QUEUE
};

/**
 * ��������ö��,ʵ����һ��Ĳ���ϵͳ����򵥵Ľ�����ֵ��Ϊ������
 */
enum class ProcessType {
    DATA_GENERATION_PROCESS = 1,
    DATA_DELETION_PROCESS = 2,
    EXECUTION_PROCESS = 3,
    INIT_PROCESS = 0
};

/**
 * �����ܵ��ṹ��
 */
struct NamedPipe {
    std::string pipeName;
    int fileDescriptor;

    NamedPipe(const std::string& name) : pipeName(name) {
        // ��������ļ���Ϊ�����ܵ�
        fileDescriptor = open(pipeName.c_str(), O_RDWR | O_CREAT, 0666);
    }

    ~NamedPipe() {
        // �ر��ļ�������
        close(fileDescriptor);
        // ɾ���ļ�
        unlink(pipeName.c_str());
    }

    // д���ݵ������ܵ�
    void writeData(const std::string& data) {
        write(fileDescriptor, data.c_str(), data.size());
    }

    // �������ܵ���ȡ����
    std::string readData() {
        const int bufferSize = 1024;
        char buffer[bufferSize];
        ssize_t bytesRead = read(fileDescriptor, buffer, bufferSize - 1);
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0'; // ����ַ���������
            return std::string(buffer);
        }
        return "";
    }
};

/**
 * ������
 */
class Process {
public:
    string name;                 // ������
    int pid;                     // ����ID
    int priority;                // �������ȼ�
    ProcessState state;          // ����״̬
    ProcessType type;            // ��������
    vector<int> allocatedMemory; // �ڴ���ַ
    string directory;            //�ļ��洢��Ϣ
    string fileName;

    Process(string &name, int pid, int priority, ProcessState state, ProcessType type) {
        this->name = name;
        this->pid = pid;
        this->priority = priority;
        this->state = state;
        this->type = type;
    }

    // ����С������������ڱȽ� Process ��������ȼ�
    bool operator<(const Process &other) const {
        return priority < other.priority;
    }

    // �麯�� create,���ڴ�������
    //  virtual void create() = 0;
    // �麯�� execute,����ִ�н���
    virtual void execute() = 0;

    // �麯�� destroy,�������ٽ���
    virtual void destroy() = 0;
};

/**
 * ���̹�����
 */
class ProcessManager {
public:
    std::vector<Process *> processList;
    std::priority_queue<Process *> readyQueue;
    std::priority_queue<Process *> blockQueue;

    // �ӽ����б���ɾ���ý���
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
    // ��Ŀ��ʼ��,����ʼ�������
    static vector<int> init() {
        return {1, 2, 3};
    }

    // ����ʼ�����ݶ����ڴ�,�����ڴ���ַ
    static vector<int> read_data_to_memory(vector<int> &initInfo) {
        return {4, 5};
    }

    // �ͷ��ڴ�,����Ϊ�ڴ���ַ
    static void releaseMemory(vector<int> &allocatedMemory) {
        cout << "release memory" << endl;
    }

    // ��������,����Ϊ�����С
    static int allocateBlocks(int dataSize) {
        return 1;
    }

    // �����ݴ�����̿�,����Ϊ������Ϣ,Ŀ¼,�ļ���
    static void saveData(const std::string &dataInfo, const std::string &directory, const std::string &fileName) {
        cout << "save data" << endl;
    }

    // Ϊ����Ŀ¼�н���Ŀ¼��,����ΪĿ¼,�ļ���,����Ĵ��̿�
    static void createDirectoryEntry(const std::string &directory, const std::string &fileName, int allocatedBlocks) {
        cout << "create directory entry" << endl;
    }

    // ���¿����̿���Ϣ
    static void updateFreeBlocks() {
        cout << "update free blocks" << endl;
    }

    // ��Ŀ¼��ɾ��Ŀ¼��ͷŴ��̿�,����ΪĿ¼,�ļ���
    static void deleteDirectoryEntry(const std::string &directory, const std::string &fileName) {
        cout << "delete directory entry" << endl;
    }

    // �����ڴ��,����Ϊ�����С
    static vector<int> allocateMemory(int dataSize) {
        return {1, 2, 3};
    }

    // �Ӵ��̿��ж�ȡ���ݵ��ڴ��,����ڴ�鲻��,ģ��ҳ�����,����ҳ��
    static void read_data_to_memory(const std::string &directory, const std::string &fileName) {
        cout << "read data to memory" << endl;
    }

    // ��ȡ����,����Ϊ�ڴ���ַ
    static std::string read_data(vector<int> &allocatedMemory) {
        return "this is a test!";
    }

    // ������д���ڴ��,����Ϊ������Ϣ,�ڴ���ַ
    static void save_data(const std::string &dataInfo, vector<int> &allocatedMemory) {
        cout << "save data" << endl;
    }

    // ������д����̿�,����Ϊ�ڴ���ַ
    static void write_data_to_disk(vector<int> &allocatedMemory) {
        cout << "write data to disk" << endl;
    }
};

DirectoryManager directoryManager;
ProcessManager processManager;

/**
 * ��ʼ������,ֱ������ϵͳ�رղŻ��ͷ��ڴ�
 */
class InitProcess : public Process {
private:
    // ���췽��
    InitProcess(string &name, int pid, int priority, ProcessType type)
            : Process(name, pid, priority, ProcessState::RUNNING, type) {}

public:
    static InitProcess create(string name, int pid, int priority, ProcessType processType) {
        auto* initProcess=new InitProcess(name, pid, priority, processType);
        // ���ý��̷�������б�
        processManager.processList.push_back(initProcess);
        return *initProcess;
    }

    void execute() override {
        vector<int> initInfo = DirectoryManager::init();
        allocatedMemory = DirectoryManager::read_data_to_memory(initInfo);
    }

    void destroy() override {
        // �ͷ��ڴ�
        DirectoryManager::releaseMemory(allocatedMemory);
        // �ӽ����б���ɾ���ý���
        processManager.deleteProcess(this->pid);
        // ���ٶ���
        delete this;
    }
};

/**
 * �������ɽ���
 */
class DataGenerationProcess : public Process {
public:
    // ���췽��
    DataGenerationProcess(string &name, int pid, int priority, ProcessState state, ProcessType type)
            : Process(name, pid, priority, state, type) {}

    static void
    create(string name, int pid, int priority, const std::string &directory, const std::string &fileName) {
        auto* dataGenerationProcess=new DataGenerationProcess(name, pid, priority, ProcessState::READY,
                                                    ProcessType::DATA_GENERATION_PROCESS);
        dataGenerationProcess->fileName = fileName;
        dataGenerationProcess->directory = directory;
        // ���ý��̷�������б�,��������
        processManager.processList.push_back(dataGenerationProcess);
        processManager.readyQueue.push(dataGenerationProcess);
    }

    void execute() override {
        //todo ������ȡdataSize��dataInfo,�������ֱ�Ӹ���
        int dataSize = 8;
        string dataInfo;
        // ���������С������
        int allocatedBlocks = DirectoryManager::allocateBlocks(dataSize);

        if (allocatedBlocks < dataSize) {
            // �̿鲻����������ʾ
            std::cout << "Error: Not enough free disk blocks for data generation." << std::endl;
            return;
        }

        // �����ݴ�����̿�
        DirectoryManager::saveData(dataInfo, directory, fileName);

        // ����Ŀ¼������Ϊ����Ŀ¼�н���Ŀ¼��
        DirectoryManager::createDirectoryEntry(directory, fileName, allocatedBlocks);

        // ���Ŀ����̿���Ϣ
        DirectoryManager::updateFreeBlocks();
    }

    void destroy() override {
        // �ӽ����б���ɾ���ý���
        processManager.deleteProcess(this->pid);
        // ���ٶ���
        delete this;
    }
};

/**
 * ����ɾ������
 */
class DataDeletionProcess : public Process {
public:
    // ���췽��
    DataDeletionProcess(string &name, int pid, int priority, ProcessState state, ProcessType type)
            : Process(name, pid, priority, state, type) {}

    static void
    create(string name, int pid, int priority, const std::string &directory, const std::string &fileName) {
        auto* dataDeletionProcess=new DataDeletionProcess(name, pid, priority, ProcessState::READY,
                                                ProcessType::DATA_DELETION_PROCESS);
        dataDeletionProcess->fileName = fileName;
        dataDeletionProcess->directory = directory;
        // ���ý��̷�������б�,��������
        processManager.processList.push_back(dataDeletionProcess);
        processManager.readyQueue.push(dataDeletionProcess);
    }

    void execute() override {
        // ��Ŀ¼��ɾ��Ŀ¼��ͷŴ��̿�,��Ҫע�����,�����ǰ�������ڴ���,����ɾ��
        DirectoryManager::deleteDirectoryEntry(directory, fileName);

        // ���Ŀ����̿���Ϣ
        DirectoryManager::updateFreeBlocks();
    }

    void destroy() override {
        // �ӽ����б���ɾ���ý���
        processManager.deleteProcess(this->pid);
        // ���ٶ���
        delete this;
    }
};

/**
 * �û�����ָ��ö��
 */
enum class UserInputCommand {
    // �˳�
    EXIT = 0,
    // ��ȡ����
    READ_DATA = 1,
    // д������
    WRITE_DATA = 2,
    // ��������
    SAVE = 3,
};

/**
 * ִ�н���
 */
class ExecutionProcess : public Process {
public:
    // ���췽��
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
        // ���ý��̷�������б�,��������
        processManager.processList.push_back(executionProcess);
        processManager.readyQueue.push(executionProcess);
    }

    void execute() override {
        std::vector<int> allocatedMemory = DirectoryManager::allocateMemory(8);
        if (allocatedMemory.empty()) {
            std::cout << "Error: Not enough free memory for execution." << std::endl;
            return;
        }
        // �����ļ��洢��Ϣ,�����ݴ��������ڴ��,����ڴ�鲻��,ģ��ҳ�����,����ҳ��
        DirectoryManager::read_data_to_memory(directory, fileName);
        execute_user_input_command(allocatedMemory);
    }

    void destroy() override {
        DirectoryManager::releaseMemory(allocatedMemory);
        processManager.deleteProcess(this->pid);
        delete this;
    }

    // ģ��ִ���û����������,����Ϊ�ڴ���ַ
    static void execute_user_input_command(std::vector<int> &allocatedMemory) {
        int userInput = 0;
        while (true) {
            std::cout << "waiting for user input..." << std::endl;
            std::cin >> userInput;
            switch (static_cast<UserInputCommand>(userInput)) {
                case UserInputCommand::READ_DATA:
                    // ��ȡ����,������ڴ������Ƕ�������д��,�����д��,��ȴ�
                    std::cout << "Data read from memory: " << DirectoryManager::read_data(allocatedMemory) << std::endl;
                    break;
                case UserInputCommand::WRITE_DATA:
                    // д������,�ȴ�QT�����û�����,����ģ��һ�����ݰ�,������ڴ������Ƕ�������д��,�����д��,��ȴ�
                    std::cout << "Data written to memory." << std::endl;
                    DirectoryManager::save_data("this is a test!", allocatedMemory);
                    break;
                case UserInputCommand::SAVE:
                    // ��������,������ڴ������Ƕ�������д��,�����д��,��ȴ�
                    DirectoryManager::write_data_to_disk(allocatedMemory);
                    break;
                default:
                    break;
            }
        }
    }
};

/**
 * PID������
 */
class PIDGenerator {
private:
    static int counter;
    static std::mutex mtx;

public:
    static int generatePID() {
        std::lock_guard<std::mutex> lock(mtx); // ʹ�û�����ȷ���̰߳�ȫ
        return counter++;
    }
};

int PIDGenerator::counter = 1;
std::mutex PIDGenerator::mtx;


/**
 * ָ��ö��
*/
enum class Command {
    // ��������
    CREATE_PROCESS = 1,
    // �˳�
    EXIT = 2
};

/**
 * ���������
*/
class TaskScheduler {
private:
    std::mutex mutex_;  // ���ڱ���������Դ
public:
    std::vector<std::thread> threads;  // �洢�̶߳��������

    void createThreads(int numThreads) {
        // ��������̲߳���������
        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back(&TaskScheduler::schedule, this);
        }
    }

    void joinThreads() {
        // �ȴ������߳̽���
        for (auto &thread: threads) {
            thread.join();
        }
    }

    void schedule() {
        std::lock_guard<std::mutex> lock(mutex_);
        // �Ӿ���������ȡ�����ȼ���ߵĽ���
        Process *process = processManager.readyQueue.top();
        process->state = ProcessState::RUNNING;
        // ִ�н���
        cout << "process " << process->name << " start" << endl;
        process->execute();
        // ִ����Ϻ�,������״̬��ΪFINISHED
        process->state = ProcessState::FINISHED;
        // �Ӿ���������ɾ���ý���
        processManager.readyQueue.pop();
        // ���ٽ���
        process->destroy();
    }

};

// int main() {
//     // ����Init�������ڳ�ʼ��
//     TaskScheduler taskScheduler;
//     InitProcess initProcess = InitProcess::create("init", PIDGenerator::generatePID(), 0, ProcessType::INIT_PROCESS);
//     initProcess.execute();
//     // ѭ���ȴ�ָ������
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
//                 // ��������
//                 //todo ��QT��ȡdirectory��fileName,�������ֱ�Ӹ���
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
//                 // ���ٽ���,�ͷ��ڴ�
//                 //�ҵ����н���,����destroy����,��һ��������init����,�������
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