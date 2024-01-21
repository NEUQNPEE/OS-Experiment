#pragma once
#include <QMainWindow>
#include <QTextEdit>
#include <QCloseEvent>

class TextFileWindow : public QMainWindow
{
private:
    // File *file;
    // FileInfo *fileInfo;

    // 文本编辑器
    QTextEdit *textEdit;

    // 执行进程
    // ExecutionProcess exeProc;

public:
    // 有参构造函数
    TextFileWindow(QWidget *parent = nullptr);

    bool eventFilter(QObject *obj, QEvent *event) override;

    void saveFileContent();

    // 重写关闭事件，关闭窗口时保存文件内容并从就绪队列中移除
    void closeEvent(QCloseEvent *event) override;

    // 读取文件内容
    // void readFileContent(FileInfo *fileInfo){};
};
