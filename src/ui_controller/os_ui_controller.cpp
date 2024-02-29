#include "os_ui_controller.h"
// tip 你猜为什么非得加个os_前缀吗？那是因为逆天qt把ui的xml文件引入项目的方式，是引用名为ui_文件名.h的文件，使用宏定义解析任何以ui_开头的include语句为引入文件名.ui的文件!
// tip 所以如果你写#include "ui_controller.h"，项目就会因为找不到controller.ui而报错！

// 点击左键不会有任何操作
void UIController::StatusBarLeftClicked(StatusBar *statusBar)
{
    connect(statusBar, &StatusBar::clicked, [=]() {});
}

void UIController::StatusBarRightClicked(StatusBar *statusBar)
{
    connect(statusBar, &StatusBar::customContextMenuRequested, [=]()
            {
        // 创建状态栏窗口
        StatusBarWindow *statusBarWindow = new StatusBarWindow();
        int x = statusBar->mapToGlobal(statusBar->rect().topLeft()).x();
        int y = statusBar->mapToGlobal(statusBar->rect().topLeft()).y() - statusBarWindow->height();

        statusBarWindow->move(x, y);
        
        connect(statusBarWindow->taskManagerBtn, &QPushButton::clicked, [=]()
                {
            showTaskManager();
        });

});
}

void UIController::showTaskManager()
{
    // tip 本部分代码需建立进程与前端的中介者之后才能编写
}