#include "tab_memory.h"

MemoryWidget::MemoryWidget(QWidget *parent) : QWidget(parent)
{
    // 创建一个8x8的网格布局
    QGridLayout *gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(2);

    // 生成纯白色，进程id为-1的8*8网格
    for (int row = 0; row < 8; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {
            // 创建一个标签，显示进程ID，并设置背景颜色
            QLabel *label = new QLabel(QString::number(-1), this);
            label->setAlignment(Qt::AlignCenter);
            label->setStyleSheet("background-color: white");

            // 给标签设置一个边框
            label->setFrameStyle(QFrame::Box | QFrame::Raised);

            // 将标签添加到网格布局中
            gridLayout->addWidget(label, row, col);
        }
    }
}

void MemoryWidget::draw(int processId, int blockNumber)
{
    QColor color;

    // 首先检查进程ID是否已经存在于映射中
    if (colorMap.find(processId) == colorMap.end())
    {
        // 如果不存在，则生成一个随机颜色
        color = generateRandomColor();

        // 将进程ID和颜色添加到映射中
        colorMap[processId] = color;
    }

    // blockNumber是否合法
    if (blockNumber < 0 || blockNumber > 63)
    {
        return;
    }

    // tip 通过上述两段if检查过后，能保证参数的合法性

    // 获取进程ID对应的颜色
    color = colorMap[processId];

    // 获取网格布局
    // todo 希望能在这里面，将layout()的访问权限封闭
    QGridLayout *gridLayout = static_cast<QGridLayout *>(this->layout());

    // 获取内存块号对应的标签
    QLabel *label = static_cast<QLabel *>(gridLayout->itemAtPosition(blockNumber / 8, blockNumber % 8)->widget());

    // 设置标签的背景颜色与进程ID
    label->setStyleSheet(QString("background-color: %1").arg(color.name()));
    label->setText(QString::number(processId));
}

void MemoryWidget::updateData(std::vector<int> memoryBlocksInfo)
{
    // 遍历数组，将每个内存块的占用情况显示出来
    for (int i = 0; i < memoryBlocksInfo.size(); ++i)
    {
        // 检查内存块号是否合法
        if (memoryBlocksInfo[i] < -1 || memoryBlocksInfo[i] > 63)
        {
            continue;
            // todo 抛出异常
        }

        draw(memoryBlocksInfo[i], i);
        // todo：三级制调用：传入信息并解析信息的函数，将信息写入暂存的一个函数，将暂存信息显示在界面上的函数
    }
}

QColor MemoryWidget::generateRandomColor()
{
    int r, g, b;
    do
    {
        r = rand() % 256;
        g = rand() % 256;
        b = rand() % 256;
    } while (isBlackOrWhite(r, g, b));

    return QColor(r, g, b);
}

bool MemoryWidget::isBlackOrWhite(int r, int g, int b)
{
    // 判断颜色是否为黑色或白色
    return (r < 10 && g < 10 && b < 10) || (r > 245 && g > 245 && b > 245);

    // tip 复杂条件判断语句，抽出来写
    // return isBlack(r, g, b) || isWhite(r, g, b);

    // tip 改成一个参数，rgb用自定义的结构体表示
    // RGB rgb(r, g, b);
    // return isBlackOrWhite(rgb);
}