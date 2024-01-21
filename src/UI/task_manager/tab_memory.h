#pragma once
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QFrame>
#include <QColor>

class MemoryWidget : public QWidget
{
public:
    // 初始化函数，生成纯白色，进程id为-1的8*8网格
    MemoryWidget(QWidget *parent = nullptr);

    // 供外部调用的绘制函数，参数为进程ID和内存块号
    void draw(int processId, int blockNumber);

    // 信息解析
    void updateData(std::vector<int> memoryBlocksInfo);

private:
    // 存储进程ID和颜色的映射
    std::map<int, QColor> colorMap;

    // 生成随机颜色，但不为黑白两色
    QColor generateRandomColor();

    bool isBlackOrWhite(int r, int g, int b);
};