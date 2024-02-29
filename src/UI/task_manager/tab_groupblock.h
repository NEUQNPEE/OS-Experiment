#pragma once
#include <QWidget>
#include <QSplitter>
#include <QVBoxLayout>
#include <QLabel>

class GroupBlockWidget : public QWidget
{
private:
    // 八个布局，每个布局对应一个组
    std::vector<QGridLayout *> groupLayouts;

public:
    // 初始化函数
    GroupBlockWidget(QWidget *parent = nullptr);

    // 供外部调用的绘制函数，参数第n块，块号
    void draw(int n, int blockNumber);

    // 解析传来的信息：1024+8大小的int数组，如果是-1说明进入下一个组，否则显示块号，如果是-2说明结束
    void updateData(std::vector<int> groupBlocksInfo);
};
