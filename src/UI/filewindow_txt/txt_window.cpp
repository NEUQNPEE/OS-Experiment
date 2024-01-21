#include "txt_window.h"

TextFileWindow::TextFileWindow(QWidget *parent) : QMainWindow(parent)
{
    // this->file = file;
    // this->fileInfo = new FileInfo(file);

    // exeProc = ExecutionProcess::create("文本文件读写进程", PIDGenerator::generatePID(), 3, fileInfo, OperationCommand::CREATE_READ_WRITE);

    setWindowTitle(QString::fromStdString("file->get_Name()"));
    resize(800, 600);

    textEdit = new QTextEdit(this);
    setCentralWidget(textEdit);

    // 为文本编辑器添加事件过滤器
    textEdit->installEventFilter(this);

    // 读取文件内容
    // readFileContent(fileInfo);
}

bool TextFileWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_S && keyEvent->modifiers() == Qt::ControlModifier)
        {
            saveFileContent();
            return true;
        }
    }
    return false;
}

void TextFileWindow::saveFileContent()
{
    // 先把文本编辑器的内容写入File
    std::string data = textEdit->toPlainText().toStdString();
    // file->set_Content(data);
    std::string *newData = new std::string(data);
    // fileInfo->setData(newData);

    // 挂载execute_write函数

    // exeProc.execute_write(file, &exeProc);

    // exeProc.execute_read(file, &exeProc);

    // textFileWindow->readFileContent(fileInfo);
}

// 重写关闭事件，关闭窗口时保存文件内容并从就绪队列中移除
void TextFileWindow::closeEvent(QCloseEvent *event)
{
    saveFileContent();

    // todo scheduler.end();

    event->accept();
}

// 读取文件内容
// void TextFileWindow::readFileContent(FileInfo *fileInfo)
// {
//     exeProc.execute_read(file, &exeProc);
//     // 写进文本编辑器
//     string data = fileInfo->getData();
//     textEdit->setText(QString::fromLocal8Bit(data));
// }