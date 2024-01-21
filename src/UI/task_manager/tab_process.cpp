#include "tab_process.h"

ProcessWidget::ProcessWidget(QWidget *parent) : QTableWidget(parent)
{
    // 设置本表格的列数
    this->setColumnCount(4);
    QStringList headers = {"进程名", "进程id", "进程状态", "使用内存块"};
    this->setHorizontalHeaderLabels(headers);
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 使列自动拉伸以填充整个宽度
}
void ProcessWidget::updateData()
{
    // todo，这里先随便写一下，填个3行数据
    for (int i = 0; i < 3; i++)
    {
        this->insertRow(i);
        this->setItem(i, 0, new QTableWidgetItem("进程名"));
        this->setItem(i, 1, new QTableWidgetItem("进程id"));
        this->setItem(i, 2, new QTableWidgetItem("进程状态"));
        this->setItem(i, 3, new QTableWidgetItem("使用内存块"));
    }
}