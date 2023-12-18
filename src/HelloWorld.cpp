#include "HelloWorld.h"

HelloWorld::HelloWorld(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui_HelloWorld)
{
    ui->setupUi(this);
}

HelloWorld::~HelloWorld()
{
    delete ui; 
}