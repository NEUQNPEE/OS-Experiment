#pragma once
#include <QPushButton>
#include <QIcon>
#include <QListWidget>
#include <QGridLayout>

class Win : public QPushButton
{
private:
    // 图标
    QIcon icon;

public:
    // 构造函数
    Win(QWidget *parent = nullptr);
};
