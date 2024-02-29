/*
 * @Author       : NieFire planet_class@foxmail.com
 * @Date         : 2024-02-25 15:53:14
 * @LastEditors  : NieFire planet_class@foxmail.com
 * @LastEditTime : 2024-02-25 16:11:50
 * @FilePath     : \OS-Experiment\src\ui_controller\ui_controller.h
 * @Description  : UI交互（槽函数）
 * ( ﾟ∀。)只要加满注释一切都会好起来的( ﾟ∀。)
 * Copyright (c) 2024 by NieFire, All Rights Reserved. 
 */
#pragma once
#include <QObject>
#include "../ui/ui.h"
class UIController : public QObject
{
    Q_OBJECT
public:
    // 底部状态栏左键单击
    void StatusBarLeftClicked(StatusBar *statusBar);

    // 底部状态栏右键单击
    void StatusBarRightClicked(StatusBar *statusBar);

    void showTaskManager();
};