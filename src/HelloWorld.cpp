#include "HelloWorld.h"

HelloWorld::HelloWorld(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui_HelloWorld)
{
    ui->setupUi(this);

    // 设置窗口标题
    this->setWindowTitle("操作系统:Windows-qt6");

    // 设置窗口大小
    this->resize(1200, 800);

    // 添加一个按钮
    QPushButton* btn = new QPushButton("按钮", this);

    // 设置按钮位置
    btn->move(100, 100);


}

HelloWorld::~HelloWorld()
{
    delete ui; 
}