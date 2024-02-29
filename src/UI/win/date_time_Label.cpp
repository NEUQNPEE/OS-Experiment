#include "date_time_Label.h"

DateTimeLabel::DateTimeLabel(QWidget *parent) : QLabel(parent)
{
    this->setStyleSheet("color: black; font-size: 20px;");
    this->setAlignment(Qt::AlignCenter);
    this->resize(200, 50);
    this->move(parent->width() - this->width(), parent->height() - this->height());
    this->show();

    // 在初始化时先显示一次时间，否则会等待1秒才显示
    this->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

    timer = new QTimer(this);
    timer->start(1000); // 启动定时器，每秒触发一次
    connect(timer, &QTimer::timeout, [=]()
            {
        // 获取当前时间
        QDateTime currentDateTime = QDateTime::currentDateTime();
        // 格式化时间
        QString currentTime = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");
        // 设置标签的文本
        this->setText(currentTime); });
}