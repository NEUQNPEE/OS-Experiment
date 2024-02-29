#include "status_bar_window.h"

StatusBarWindow::StatusBarWindow(QWidget *parent) : QWidget(parent)
{
    this->setWindowTitle("选项");
    this->resize(200, 100);  // 调整窗口大小

    // 创建并设置任务管理器按钮
    taskManagerBtn = new QPushButton("任务管理器", this);
    taskManagerBtn->setFixedSize(120, 30); // 设置按钮大小
    taskManagerBtn->move((this->width() - taskManagerBtn->width()) / 2, (this->height() - taskManagerBtn->height()) / 2);
}