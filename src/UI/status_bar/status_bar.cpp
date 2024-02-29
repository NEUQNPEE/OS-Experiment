#include "status_bar.h"

StatusBar::StatusBar(QWidget *parent) : QPushButton(parent)
{
    this->resize(this->width(), 50);
    this->move(0, this->height() - this->height());

    // 设置按钮的样式，包括背景色和其他属性
    this->setStyleSheet("QPushButton { background-color: white; border: 0px; }");
    this->setText("");
    this->setToolTip("");
    this->setFlat(true);
    this->setCursor(Qt::ArrowCursor);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
}