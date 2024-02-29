#include "win_window.h"

WinWindow::WinWindow(QWidget *parent, int x, int y) : QWidget(parent)
{
    this->setWindowTitle("WIN");
    this->resize(WIN_WINDOW_WIDTH, WIN_WINDOW_HEIGHT);
    // 设置窗口背景颜色为白色
    this->setStyleSheet("background-color: white;");

    // 设为无边框
    // this->setWindowFlags(Qt::FramelessWindowHint);

    // 移动窗口到给出的位置
    this->move(x, y - WIN_WINDOW_HEIGHT);

    QWidget *leftWidget = new QWidget(this);
    // 左边占1/10的宽度
    leftWidget->setGeometry(0, 0, LEFT_WIDGET_WIDTH, WIN_WINDOW_HEIGHT);

    QPushButton *topButton = new QPushButton(leftWidget);
    // 按钮为正方形，边长与左边窗口的宽度相同
    topButton->setGeometry(0, 0, SYSTEM_ICON_WIDTH, SYSTEM_ICON_WIDTH);
    // 按钮的图标
    QIcon topButtonIcon("icon/shutdown.png");
    topButton->setIcon(topButtonIcon);
    // 按钮无边框
    topButton->setFlat(true);

    QPushButton *bottomButton1 = new QPushButton(leftWidget);
    bottomButton1->setGeometry(0, WIN_WINDOW_HEIGHT - SYSTEM_ICON_WIDTH, SYSTEM_ICON_WIDTH, SYSTEM_ICON_WIDTH);
    QIcon bottomButton1Icon("icon/shutdown.png");
    bottomButton1->setIcon(bottomButton1Icon);
    bottomButton1->setFlat(true);

    QPushButton *bottomButton2 = new QPushButton(leftWidget);
    bottomButton2->setGeometry(0, WIN_WINDOW_HEIGHT - SYSTEM_ICON_WIDTH * 2, SYSTEM_ICON_WIDTH, SYSTEM_ICON_WIDTH);
    QIcon bottomButton2Icon("icon/shutdown.png");
    bottomButton2->setIcon(bottomButton2Icon);
    bottomButton2->setFlat(true);

    // 创建中间的窗口
    QListWidget *middleWidget = new QListWidget(this);
    middleWidget->setGeometry(LEFT_WIDGET_WIDTH, 0, MIDDLE_WIDGET_WIDTH, WIN_WINDOW_HEIGHT);

    QListWidgetItem *item1 = new QListWidgetItem("列表项1");
    QListWidgetItem *item2 = new QListWidgetItem("列表项2");
    QListWidgetItem *item3 = new QListWidgetItem("列表项3");

    middleWidget->addItem(item1);
    middleWidget->addItem(item2);
    middleWidget->addItem(item3);

    // 创建右边的窗口
    QWidget *rightWidget = new QWidget(this);
    rightWidget->setGeometry(LEFT_WIDGET_WIDTH + MIDDLE_WIDGET_WIDTH, 0, RIGHT_WIDGET_WIDTH, WIN_WINDOW_HEIGHT);

    QGridLayout *gridLayout = new QGridLayout(rightWidget);
    gridLayout->setSpacing(APP_ICON_WIDTH);

    for (int row = 0; row < 5; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            QPushButton *button = new QPushButton(QString("按钮 %1-%2").arg(row).arg(col), rightWidget);
            gridLayout->addWidget(button, row, col);
        }
    }
}

void WinWindow::globalMousePressEvent(QMouseEvent *event)
{
    // // debug 检查this->geometry()的值
    // QRect rect = this->geometry();

    // int x1 = rect.x();
    // int y1 = rect.y();
    // int x2 = rect.x() + rect.width();
    // int y2 = rect.y() + rect.height();

    // // debug 检查event->globalPos()的值
    // int x = event->pos().x();
    // int y = event->pos().y();

    // 如果点击的不是窗口内部，就关闭窗口
    if (!this->geometry().contains(event->pos()))
    {
        this->close();
    }
}
