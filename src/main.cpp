#include "HelloWorld.h"
#include <QApplication>
#include <QThread>

#include "OS.h"

#pragma comment(lib, "user32.lib")

int main(int argc, char *argv[])
{
    OSApplication os(argc, argv);
    
    // 创建线程
    QThread thread1;

    // 创建对象
    HelloWorld w1;
    
    // 将窗口对象移动到相应的线程中
    w1.moveToThread(&thread1);
    
    // 连接线程的启动信号到窗口的显示槽函数
    QObject::connect(&thread1, &QThread::started, &w1, &HelloWorld::show);

    // 连接OSApplication的notify信号到窗口的notify槽函数，本质上这模拟了鼠标点击的硬件中断
    void (OSApplication::*notify)(QEvent *) = &OSApplication::notify;
    QObject::connect(&os, notify, &w1, &HelloWorld::notify);

    // 启动线程
    thread1.start();
    
    // 运行应用程序的事件循环
    int retCode = os.exec();
    
    // 等待线程完成
    thread1.wait();
    
    return retCode;
}