#include "HelloWorld.h"
#include <QApplication>
#include <QThread>

#pragma comment(lib, "user32.lib")

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    // 创建线程
    QThread thread1;

    // 创建对象
    HelloWorld w1;
    
    // 将窗口对象移动到相应的线程中
    w1.moveToThread(&thread1);
    
    // 连接线程的启动信号到窗口的显示槽函数
    QObject::connect(&thread1, &QThread::started, &w1, &HelloWorld::show);

    // 连接窗口的关闭信号到线程的退出槽函数
    QObject::connect(&w1, &HelloWorld::close, &thread1, &QThread::quit);
    
    // 启动线程
    thread1.start();
    
    // 运行应用程序的事件循环
    int retCode = a.exec();
    
    // 等待线程完成
    thread1.wait();
    
    return retCode;
}