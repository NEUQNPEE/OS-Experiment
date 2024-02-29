#pragma once
#include <QLabel>
#include <QTimer>
#include <QDateTime>
#include <QString>

class DateTimeLabel : public QLabel
{
private:
    // 定时器
    QTimer *timer;
public:
    // 构造函数
    DateTimeLabel(QWidget *parent = nullptr);
};
