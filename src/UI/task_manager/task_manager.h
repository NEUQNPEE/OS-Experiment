#pragma once
#include <QWidget>
#include <QMainWindow>


#include "tab_memory.h"
#include "tab_disk.h"
#include "tab_groupblock.h"
#include "tab_process.h"


class TaskManagerWidget : public QMainWindow
{
private:
    // 四个选项卡
    ProcessWidget *processWidget;
    MemoryWidget *memoryWidget;
    DiskWidget *diskWidget;
    GroupBlockWidget *groupBlockWidget;

public:
    // 构造函数
    TaskManagerWidget(QWidget *parent = nullptr);
    
    // 为四个选项卡传入数据(暂时)
    void updateProcessData();

    void updateMemoryData(std::vector<int> memoryMessage);

    void updateDiskData(std::vector<bool> diskMessage);

    void updateGroupBlockData(std::vector<int> groupBlockMessage);
};
