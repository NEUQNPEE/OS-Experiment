#pragma once
#include "ui_HelloWorld.h"
#include <QMainWindow>
#include <QPushButton>

class HelloWorld : public QMainWindow {
    Q_OBJECT
    
public:
    HelloWorld(QWidget* parent = nullptr);
    ~HelloWorld();

private:
    Ui_HelloWorld* ui;
};