#include "task_manager.h"

TaskManagerWidget::TaskManagerWidget(QWidget *parent) : QMainWindow(parent)
{
    this->setWindowTitle("任务管理器");
    this->resize(600, 400); // 根据需要调整大小

    // 在任务管理器窗口中添加栏目
    QTabWidget *tabWidget = new QTabWidget(this);
    tabWidget->setTabsClosable(false); // 禁止关闭选项卡
    tabWidget->setMovable(true);       // 允许拖动选项卡

    // 选项卡放在顶部
    tabWidget->setTabPosition(QTabWidget::North);

    // 设置选项卡
    QWidget *tabProcess = new QWidget(tabWidget);
    QWidget *tabMemory = new QWidget(tabWidget);
    QWidget *tabDisk = new QWidget(tabWidget);
    QWidget *tabGroupBlock = new QWidget(tabWidget);

    QVBoxLayout *processLayout = new QVBoxLayout(tabProcess);
    QVBoxLayout *memoryLayout = new QVBoxLayout(tabMemory);
    QVBoxLayout *diskLayout = new QVBoxLayout(tabDisk);
    QVBoxLayout *groupBlockLayout = new QVBoxLayout(tabGroupBlock);

    // 将布局管理器设置给选项卡
    tabProcess->setLayout(processLayout);
    tabMemory->setLayout(memoryLayout);
    tabDisk->setLayout(diskLayout);
    tabGroupBlock->setLayout(groupBlockLayout);

    // 将选项卡添加到选项卡窗口中
    tabWidget->addTab(tabProcess, "进程");
    tabWidget->addTab(tabMemory, "内存");
    tabWidget->addTab(tabDisk, "磁盘");
    tabWidget->addTab(tabGroupBlock, "成组链块");

    // 将选项卡窗口设置为中心窗口
    this->setCentralWidget(tabWidget);

    processWidget = new ProcessWidget(tabProcess);
    processLayout->addWidget(processWidget);

    memoryWidget = new MemoryWidget(tabMemory);
    memoryLayout->addWidget(memoryWidget);

    diskWidget = new DiskWidget(tabDisk);
    diskLayout->addWidget(diskWidget);

    groupBlockWidget = new GroupBlockWidget(tabGroupBlock);
    groupBlockLayout->addWidget(groupBlockWidget);
}

void TaskManagerWidget::updateProcessData()
{
    processWidget->updateData();
}

void TaskManagerWidget::updateMemoryData(std::vector<int> memoryMessage)
{
    memoryWidget->updateData(memoryMessage);
}

void TaskManagerWidget::updateDiskData(std::vector<bool> diskMessage)
{
    diskWidget->updateData(diskMessage);
}

void TaskManagerWidget::updateGroupBlockData(std::vector<int> groupBlockMessage)
{
    groupBlockWidget->updateData(groupBlockMessage);
}