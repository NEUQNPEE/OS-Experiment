#include "HelloWorld.h"
#include <QApplication>
#include <QThread>

#pragma comment(lib, "user32.lib")

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    // 创建两个线程
    QThread thread1;
    QThread thread2;
    
    // 创建窗口对象
    HelloWorld w1;
    HelloWorld w2;
    
    // 将窗口对象移动到相应的线程中
    w1.moveToThread(&thread1);
    w2.moveToThread(&thread2);
    
    // 连接线程的启动信号到窗口的显示槽函数
    QObject::connect(&thread1, &QThread::started, &w1, &HelloWorld::show);
    QObject::connect(&thread2, &QThread::started, &w2, &HelloWorld::show);
    
    // 启动线程
    thread1.start();
    thread2.start();
    
    // 运行应用程序的事件循环
    int retCode = a.exec();
    
    // 等待线程完成
    thread1.wait();
    thread2.wait();
    
    return retCode;
}