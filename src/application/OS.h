/*
 * @Author       : NieFire planet_class@foxmail.com
 * @Date         : 2024-01-24 00:01:23
 * @LastEditors  : NieFire planet_class@foxmail.com
 * @LastEditTime : 2024-01-24 14:11:11
 * @FilePath     : \OS-Experiment\src\OS.h
 * @Description  : 操作系统应用程序，继承自QApllication
 * ( ﾟ∀。)只要加满注释一切都会好起来的( ﾟ∀。)
 * Copyright (c) 2024 by NieFire, All Rights Reserved.
 */

#pragma once
#include <QApplication>
#include <QEvent>
#include <QMouseEvent>
#include <QObject>

class OSApplication : public QApplication
{
    Q_OBJECT
    
public:
    // 构造函数
    OSApplication(int argc, char *argv[]);

    bool notify(QObject* obj, QEvent * event) override;

signals:
    // 给窗口的信号
    void notify(QEvent * event);
};
