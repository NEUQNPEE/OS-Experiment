#include "win.h"

Win::Win(QWidget *parent): QPushButton(parent)
{
    QIcon win_icon("icon/WLOGO.png");
    this->setIcon(win_icon);
    this->resize(50, 50);
    int a = parent->width();
    int b = parent->height();
    this->move(0, parent->height() - this->height());

    this->setStyleSheet("QPushButton{border:0px;}");
    this->setFlat(true);
    this->setToolTip("WIN");
    this->setCursor(Qt::PointingHandCursor);
}