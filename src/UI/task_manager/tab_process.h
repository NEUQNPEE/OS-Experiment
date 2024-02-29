#include <QWidget>
#include <QTableWidget>
#include <QHeaderView>

class ProcessWidget : public QTableWidget
{
public:
    // 初始化函数
    ProcessWidget(QWidget *parent = nullptr);

    // 信息更新函数
    void updateData();
};
