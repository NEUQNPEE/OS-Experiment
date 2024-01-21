#pragma once
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QSplitter>

class DiskWidget : public QWidget
{
private:
    // 兑换区和文件区的指针，便于后续操作
    QGridLayout *exchangeGridLayout;
    QGridLayout *fileGridLayout;

public:
    // 初始化函数
    DiskWidget(QWidget *parent = nullptr);
    ~DiskWidget();

    // 供外部调用的绘制函数，参数为是否占用和块号
    void draw(bool isOccupied, int blockNumber);

    // 解析传来的信息1024块磁盘的占用情况，是一个1024大小的std：bool数组
    void updateData(std::vector<bool> diskBlocksInfo);
};
