#pragma once
#include "ui_HelloWorld.h"
#include <QMainWindow>
#include <QPushButton>
#include <QWidget>
#include <QVBoxLayout>
#include <QToolBar>
#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QGridLayout>
class HelloWorld : public QMainWindow {
    Q_OBJECT
    
public:
    HelloWorld(QWidget* parent = nullptr);
    ~HelloWorld();

private:
    Ui_HelloWorld* ui;
};