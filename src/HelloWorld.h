#pragma once
#include "ui_HelloWorld.h"
#include <QMainWindow>

class HelloWorld : public QMainWindow {
    Q_OBJECT
    
public:
    HelloWorld(QWidget* parent = nullptr);
    ~HelloWorld();

private:
    Ui_HelloWorld* ui;
};