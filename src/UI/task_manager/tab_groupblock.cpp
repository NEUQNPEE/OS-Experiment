#include "tab_groupblock.h"

GroupBlockWidget::GroupBlockWidget(QWidget *parent) : QWidget(parent)
{
    // 成组链块分八个组，每组有128个盘块（4*32的网格布局），使用QSplitter分割
    QSplitter *splitter = new QSplitter(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(splitter);

    // 创建八个组的布局
    for (int i = 0; i < 8; ++i)
    {
        QFrame *frame = new QFrame();
        frame->setFrameStyle(QFrame::Box | QFrame::Raised); // 设置边框样式为凸起的方框
        frame->setLineWidth(2);                             // 设置边框宽度
        QVBoxLayout *layout = new QVBoxLayout(frame);
        layout->setContentsMargins(5, 5, 5, 5); // 设置布局边距

        // 组名称标签
        QLabel *label = new QLabel(QString("组%1").arg(i + 1), frame);
        label->setAlignment(Qt::AlignCenter);
        layout->addWidget(label);

        // 创建网格布局
        QGridLayout *gridLayout = new QGridLayout();
        gridLayout->setSpacing(2);
        layout->addLayout(gridLayout);

        // 生成空白的网格
        for (int row = 0; row < 32; ++row)
        {
            for (int col = 0; col < 4; ++col)
            {
                QLabel *label = new QLabel(this);
                label->setAlignment(Qt::AlignCenter);
                label->setStyleSheet(QString("background-color: white"));

                // 边框
                label->setFrameStyle(QFrame::Box | QFrame::Raised);

                // 标签大小最小为10x10
                label->setMinimumSize(10, 10);

                // 将标签添加到网格布局中
                gridLayout->addWidget(label, row, col);
            }
        }

        // 将组布局添加到分割器中
        splitter->addWidget(frame);

        // 将组布局添加到组布局数组中
        groupLayouts.push_back(gridLayout);
    }
}

void GroupBlockWidget::draw(int n, int blockNumber)
{
    // 找到第n个块所在的组，这个块显示块号
    int groupNumber = n / 128;
    int blockNumberInGroup = n % 128;

    // 获取网格布局中的标签
    QLabel *label = static_cast<QLabel *>(groupLayouts[groupNumber]->itemAtPosition(blockNumberInGroup / 4, blockNumberInGroup % 4)->widget());

    // 设置标签
    label->setText(QString::number(blockNumber));
}

// 解析传来的信息：1024+8大小的int数组，如果是-1说明进入下一个组，否则显示块号，如果是-2说明结束
void GroupBlockWidget::updateData(std::vector<int> groupBlocksInfo)
{
    int n = 0;
    // 遍历数组，将每个盘块的占用情况显示出来
    for (int i = 0; i < groupBlocksInfo.size(); ++i)
    {
        if (groupBlocksInfo[i] == -2)
        {
            break;
        }
        else if (groupBlocksInfo[i] == -1)
        {
            continue;
        }
        else
        {
            draw(n, groupBlocksInfo[i]);
            n++;
        }
    }
}