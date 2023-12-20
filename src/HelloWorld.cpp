#include "HelloWorld.h"

HelloWorld::HelloWorld(QWidget *parent)
    : QMainWindow(parent), ui(new Ui_HelloWorld)
{
    ui->setupUi(this);

    // 设置窗口标题
    this->setWindowTitle("操作系统:Windows-qt6");

    // 设置窗口大小
    this->resize(1200, 800);
    this->setAutoFillBackground(true);
    // 设置窗口背景
    QPalette palette;
    palette.setBrush(QPalette::Window,QBrush(QPixmap("icon/background.jpeg").scaled(this->size())));
    this->setPalette(palette);

    // 添加一个按钮
    QPushButton *btn = new QPushButton("按钮", this);

    // 设置按钮位置
    btn->move(100, 100);

    // // 菜单栏
    // QMenuBar *menuBar = new QMenuBar(this);
    // this->setMenuBar(menuBar);

    // QMenu *menu1 = new QMenu("文件");
    // QMenu *menu2 = new QMenu("编辑");
    // QMenu *menu3=new QMenu("构建");
    // menuBar->addMenu(menu1);
    // menuBar->addMenu(menu2);
    // menuBar->addMenu(menu3);
    // QAction *act1=new QAction("打开文件");
    // QAction *act2= new QAction("另存为");
    // QAction *act3=new QAction("关闭");

    QAction* begin_action = new QAction(QIcon("icon/WLOGO.png"), "Action", this);
    // menu1->addAction(act1);
    // menu1->addAction (act2);
    // menu1->addAction(act3);

    // 底部工具栏
    QToolBar *toolBar = new QToolBar(this);
    this->addToolBar(Qt::BottomToolBarArea,toolBar);
    toolBar->setMovable(false);
    toolBar->addAction(begin_action);



}

HelloWorld::~HelloWorld()
{
    delete ui;
}