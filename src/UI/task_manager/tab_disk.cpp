#include "tab_disk.h"

DiskWidget::DiskWidget(QWidget *parent) : QWidget(parent)
{
    // 创建整体布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 创建 QSplitter，用于分割兑换区和文件区
    QSplitter *splitter = new QSplitter(this);
    mainLayout->addWidget(splitter);

    // 创建兑换区部件
    QFrame *exchangeFrame = new QFrame();
    exchangeFrame->setFrameStyle(QFrame::Box | QFrame::Raised); // 设置边框样式为凸起的方框
    exchangeFrame->setLineWidth(2);                             // 设置边框宽度
    QVBoxLayout *exchangeLayout = new QVBoxLayout(exchangeFrame);
    exchangeLayout->setContentsMargins(5, 5, 5, 5); // 设置布局边距

    // 兑换区名称标签
    QLabel *exchangeLabel = new QLabel("兑换区", exchangeFrame);
    exchangeLabel->setAlignment(Qt::AlignCenter);
    exchangeLayout->addWidget(exchangeLabel);

    // 创建文件区部件
    QFrame *fileFrame = new QFrame();
    fileFrame->setFrameStyle(QFrame::Box | QFrame::Raised); // 设置边框样式为凸起的方框
    fileFrame->setLineWidth(2);                             // 设置边框宽度
    QVBoxLayout *fileLayout = new QVBoxLayout(fileFrame);
    fileLayout->setContentsMargins(5, 5, 5, 5); // 设置布局边距

    // 文件区名称标签
    QLabel *fileLabel = new QLabel("文件区", fileFrame);
    fileLabel->setAlignment(Qt::AlignCenter);
    fileLayout->addWidget(fileLabel);

    // 创建兑换区布局（31行4列的网格布局）
    exchangeGridLayout = new QGridLayout();
    exchangeGridLayout->setSpacing(2);
    exchangeLayout->addLayout(exchangeGridLayout);

    // 创建文件区布局（30行30列的网格布局）
    fileGridLayout = new QGridLayout();
    fileGridLayout->setSpacing(2);
    fileLayout->addLayout(fileGridLayout);

    // 生成空白的兑换区和文件区
    for (int row = 0; row < 31; ++row)
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

            // 标记第几块
            label->setText(QString::number(row * 4 + col));

            // 将标签添加到兑换区网格布局中
            exchangeGridLayout->addWidget(label, row, col);
        }
    }

    for (int row = 0; row < 30; ++row)
    {
        for (int col = 0; col < 30; ++col)
        {
            QLabel *label = new QLabel(this);
            label->setAlignment(Qt::AlignCenter);
            label->setStyleSheet(QString("background-color: white"));

            // 边框
            label->setFrameStyle(QFrame::Box | QFrame::Raised);

            // 标签大小最小为10x10
            label->setMinimumSize(10, 10);

            // 标记第几块
            label->setText(QString::number(row * 30 + col + 124));

            // 将标签添加到文件区网格布局中
            fileGridLayout->addWidget(label, row, col);
        }
    }

    // 将兑换区和文件区添加到分割器中
    splitter->addWidget(exchangeFrame);
    splitter->addWidget(fileFrame);
    splitter->setSizes(QList<int>() << this->width() / 5 << this->width() * 4 / 5);
}

DiskWidget::~DiskWidget()
{
}

void DiskWidget::draw(bool isOccupied, int blockNumber)
{

    // 获取网格布局中的标签
    QLabel *label = nullptr;
    if (blockNumber < 124)
    {
        // 如果块号小于124，则为兑换区
        label = static_cast<QLabel *>(exchangeGridLayout->itemAtPosition(blockNumber / 4, blockNumber % 4)->widget());
    }
    else
    {
        // 否则为文件区
        label = static_cast<QLabel *>(fileGridLayout->itemAtPosition((blockNumber - 124) / 30, (blockNumber - 124) % 30)->widget());
    }

    // 设置标签的背景颜色
    if (isOccupied)
    {
        label->setStyleSheet(QString("background-color: blue"));
    }
    else
    {
        label->setStyleSheet(QString("background-color: white"));
    }

    // 更新标签
    label->update();
}

void DiskWidget::updateData(std::vector<bool> diskBlocksInfo)
{
    // 遍历数组，将每个盘块的占用情况显示出来
    for (int i = 0; i < diskBlocksInfo.size(); ++i)
    {
        draw(diskBlocksInfo[i], i);
    }
}
