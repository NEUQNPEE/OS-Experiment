#pragma once
#include <QPushButton>
#include <QIcon>
#include <QListWidget>
#include <QGridLayout>
#include <QMouseEvent>

class WinWindow : public QWidget
{
private:
    // 定死窗口大小为600*650
    const static int WIN_WINDOW_WIDTH = 600;
    const static int WIN_WINDOW_HEIGHT = 650;

    // 左侧窗口宽度60
    const static int LEFT_WIDGET_WIDTH = 60;

    // 中间窗口宽度240
    const static int MIDDLE_WIDGET_WIDTH = 240;

    // 右侧窗口宽度300
    const static int RIGHT_WIDGET_WIDTH = 300;

    // 系统图标大小为60*60
    const static int SYSTEM_ICON_WIDTH = 60;

    // 应用图标大小为95*95
    const static int APP_ICON_WIDTH = 95;

public:
    // 构造函数
    WinWindow(QWidget *parent = nullptr, int x = 0, int y = 0);

    void globalMousePressEvent(QMouseEvent *event);
};
