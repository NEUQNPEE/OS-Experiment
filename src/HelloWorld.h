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
#include <QResizeEvent>
#include <QTableWidget>
#include <QHeaderView>
#include <QTreeView>
#include <QFileSystemModel>
#include <QPainter>
#include <QSplitter>
#include <QTextEdit>
#include <QMessageBox>
class HelloWorld : public QMainWindow
{
    Q_OBJECT

public:
    HelloWorld(QWidget *parent = nullptr);
    ~HelloWorld();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui_HelloWorld *ui;
    QPushButton *status_btn;
    QPushButton *win_btn;
    QWidget *win = nullptr;
    void showTaskManager();
};
