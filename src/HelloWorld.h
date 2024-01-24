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
#include <QTimer>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QMenu>
#include <QModelIndex>
#include <QInputDialog>

#include <QMouseEvent>

#include "ui/task_manager/task_manager.h"
#include "ui/filewindow_txt/txt_window.h"
#include "ui/win/win.h"
#include "ui/win/win_window.h"

#include "process_manager.h"
class HelloWorld : public QMainWindow
{
    Q_OBJECT

public:
    HelloWorld(QWidget *parent = nullptr);
    ~HelloWorld();

    // close()函数
    void close();

    // 接收全局事件
    void notify(QEvent * event);

protected:
    void resizeEvent(QResizeEvent *event) override;
    

private:
    Ui_HelloWorld *ui;
    QPushButton *status_btn;
    Win *win_btn;
    WinWindow *win_window;
    QLabel *timeLabel;

    InitProcess initProc = InitProcess::create("init", PIDGenerator::generatePID(), 0, ProcessType::INIT_PROCESS);
    
    void showTaskManager();

    void showWinWindow();

signals:
    void globalLeftClicked(QMouseEvent *event);
    void globalRightClicked(QMouseEvent *event);
    void globalClicked(QMouseEvent *event);
};
