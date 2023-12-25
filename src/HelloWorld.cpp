#include "HelloWorld.h"

HelloWorld::HelloWorld(QWidget *parent)
    : QMainWindow(parent), ui(new Ui_HelloWorld)
{
    ui->setupUi(this);

    /**
     * 总体窗口部分
     */
    // 设置窗口标题
    this->setWindowTitle("操作系统:Windows-qt6");
    // 设置窗口大小
    this->resize(1200, 800);
    this->setAutoFillBackground(true);
    // 设置窗口背景
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(QPixmap("icon/background.jpeg").scaled(this->size())));
    this->setPalette(palette);

    /**
     * 此电脑图标部分
     */
    QIcon mycomputer_icon("icon/mycomputer.png");
    QPixmap pix = mycomputer_icon.pixmap(QSize(50, 50));
    QPushButton *mycomputer_btn = new QPushButton("", this);
    mycomputer_btn->setIcon(QIcon(pix));
    mycomputer_btn->setIconSize(pix.size());
    mycomputer_btn->setFixedSize(80, 100);
    mycomputer_btn->move(0, 0);

    // 使用样式表使图标填充按钮
    mycomputer_btn->setStyleSheet("QPushButton {"
                                  "    border: none;"                 // 去除边框
                                  "    background-repeat: no-repeat;" // 禁止平铺
                                  "    background-position: center;"  // 居中对齐
                                  "}");

    // 按钮添加点击事件
    connect(mycomputer_btn, &QPushButton::clicked, [=]()
            {
    // 创建新窗口
    QMainWindow *myComputerWindow = new QMainWindow(this);
    myComputerWindow->setWindowTitle("此电脑");
    myComputerWindow->resize(800, 600);  // 设置窗口大小

    // 创建文件系统模型和视图
    QFileSystemModel *fileModel = new QFileSystemModel(this);
    fileModel->setRootPath(QDir::rootPath());  // 从根目录开始
    fileModel->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);  // 显示所有条目，但排除.和..

    QTreeView *treeView = new QTreeView(myComputerWindow);
    treeView->setModel(fileModel);
    treeView->setRootIndex(fileModel->index(QDir::rootPath()));  // 设置根索引

    // 设置QTreeView的样式以更好地模仿Windows资源管理器
    treeView->setAnimated(false);
    treeView->setIndentation(20);
    treeView->setSortingEnabled(true);
    treeView->sortByColumn(0, Qt::AscendingOrder);  // 根据第一列排序
    treeView->setHeaderHidden(false);  // 显示头部
    treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);  // 根据内容调整列宽

    // 将treeView设置为窗口的中心部件
    myComputerWindow->setCentralWidget(treeView);

    // 显示新窗口
    myComputerWindow->show(); });

    /**
     * 文本文件图标部分
     */
    // 在此电脑按钮的正下方添加另一个按钮
    QIcon recycle_icon("icon/TXT.png");
    QPixmap pixma = recycle_icon.pixmap(QSize(50, 50));
    QPushButton *recycle_btn = new QPushButton("", this);
    recycle_btn->setIcon(QIcon(pixma));
    recycle_btn->setIconSize(pixma.size());
    recycle_btn->setFixedSize(80, 100);
    recycle_btn->move(0, mycomputer_btn->height());

    recycle_btn->setStyleSheet("QPushButton {"
                               "    border: none;"                 // 去除边框
                               "    background-repeat: no-repeat;" // 禁止平铺
                               "    background-position: center;"  // 居中对齐
                               "}");

    // 双击按钮后弹出一个窗口，窗口名字为文本文件
    connect(recycle_btn, &QPushButton::clicked, [=]()
            {
                // 创建新窗口
                QMainWindow *textWindow = new QMainWindow(this);
                textWindow->setWindowTitle("文本文件");
                textWindow->resize(800, 600); // 设置窗口大小
                textWindow->show(); });

    /**
     * 底部状态栏按钮部分
     */
    // 将下面的状态栏设置为一个按钮
    status_btn = new QPushButton(this);
    status_btn->resize(this->width(), 50);
    status_btn->move(0, this->height() - status_btn->height());

    // 设置按钮的样式，包括背景色和其他属性
    status_btn->setStyleSheet("QPushButton { background-color: white; border: 0px; }");
    status_btn->setText("");
    status_btn->setToolTip("");
    status_btn->setFlat(true);
    status_btn->setCursor(Qt::ArrowCursor);
    status_btn->setContextMenuPolicy(Qt::CustomContextMenu);
    status_btn->show();

    // 连接按钮的信号和槽
    connect(status_btn, &QPushButton::clicked, [=]()
            {
                // 点击左键不会有任何操作
            });

    connect(status_btn, &QPushButton::customContextMenuRequested, [=]()
            {
    // 创建任务管理器窗口
    QWidget *task = new QWidget();
    task->setWindowTitle("选项");
    task->resize(200, 100);  // 调整窗口大小

    // 计算窗口的位置使其在status_btn的正上方，并且下边界紧贴status_btn的上边界
    int x = status_btn->mapToGlobal(status_btn->rect().topLeft()).x();
    int y = status_btn->mapToGlobal(status_btn->rect().topLeft()).y() - task->height();

    // 移动窗口到计算出的位置
    task->move(x + (status_btn->width() - task->width()) / 2, y);

    // 创建并设置任务管理器按钮
    QPushButton *taskManagerBtn = new QPushButton("任务管理器", task);
    taskManagerBtn->setFixedSize(120, 30); // 设置按钮大小
    taskManagerBtn->move((task->width() - taskManagerBtn->width()) / 2, (task->height() - taskManagerBtn->height()) / 2);

    // 连接按钮的点击事件
    connect(taskManagerBtn, &QPushButton::clicked, [=]() {
        // 在此处添加显示任务管理器的逻辑或任何其他操作
        // 例如，您可以调用showTaskManager函数或其他功能
        showTaskManager();
    });

    // 显示任务管理器窗口
    task->show(); });

    /**
     * 左下角WIN图标部分
     */
    // WIN 图标,将其设置为一个按钮
    QIcon win_icon("icon/WLOGO.png");
    QPixmap pixmap = win_icon.pixmap(QSize(50, 50));
    win_btn = new QPushButton(QIcon(pixmap), "", this);
    win_btn->resize(pixmap.size());
    win_btn->move(0, this->height() - win_btn->height());

    win_btn->setStyleSheet("QPushButton{border:0px;}");
    win_btn->setFlat(true);
    win_btn->setToolTip("WIN");
    win_btn->setCursor(Qt::PointingHandCursor);
    win_btn->show();

    // 点击WIN图标后，弹出一个窗口，上面包含一个有关机图标的按钮
    connect(win_btn, &QPushButton::clicked, [=]()
            {
    // 检查窗口是否已经打开
    if (win == nullptr) {
        // 创建新的窗口
        win = new QWidget();
        win->setWindowTitle("WIN");
        win->resize(200, 50);
        // 设置窗口背景颜色为白色
        win->setStyleSheet("background-color: white;");
        // 计算窗口的位置使其在WIN按钮的正上方
        int x = win_btn->mapToGlobal(win_btn->rect().topLeft()).x();
        int y = win_btn->mapToGlobal(win_btn->rect().topLeft()).y() - win->height();
        // 移动窗口到计算出的位置
        win->move(x, y);
        // 使用布局管理器
        QVBoxLayout *layout = new QVBoxLayout(win);
        // 创建一个关机按钮并添加到布局中
        QIcon shutdown_icon("icon/shutdown.png");
        QPushButton *shutdown_btn = new QPushButton(shutdown_icon, "关机", win);
        shutdown_btn->setStyleSheet("QPushButton{border:0px;}");
        shutdown_btn->setFlat(true);
        shutdown_btn->setToolTip("关机");
        shutdown_btn->setCursor(Qt::PointingHandCursor);

        layout->addWidget(shutdown_btn);
        // 点击按钮后执行关闭程序操作
        connect(shutdown_btn, &QPushButton::clicked, [=]() {
            QApplication::quit();  
        });
        // 设置窗口布局
        win->setLayout(layout);
        // 窗口关闭时清理资源
        connect(win, &QWidget::destroyed, [=]() {
            delete layout;
            delete win;
            win = nullptr;  // 重置指针
        });
        // 显示窗口
        win->show();
    } else {
        // 如果窗口已经存在，则将其带到前台
        win->raise();
        win->activateWindow();
    } });
}

HelloWorld::~HelloWorld()
{
    delete ui;
}

// 重写resizeEvent函数，实现窗口大小改变时，背景图片大小等部件也跟着改变
void HelloWorld::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    // 更新背景图片大小
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(QPixmap("icon/background.jpeg").scaled(this->size())));
    this->setPalette(palette);
    // 更新按钮的大小和位置
    if (status_btn)
    { // 确保 status_btn 已经被初始化
        status_btn->resize(this->width(), 50);
        status_btn->move(0, this->height() - status_btn->height());
    }
    // 更新 win_btn 的大小和位置
    if (win_btn)
    {
        // 使用同样的逻辑，更新 win_btn 的大小和位置
        win_btn->resize(50, 50);
        win_btn->move(0, this->height() - win_btn->height());
    }
}

// 展示任务管理器界面的函数
void HelloWorld::showTaskManager()
{
    foreach (QWidget *widget, QApplication::topLevelWidgets())
    {
        if (widget->windowTitle() == "选项")
        {
            widget->close();
        }
    }

    // 创建任务管理器窗口
    QMainWindow *taskManagerWindow = new QMainWindow(this);
    taskManagerWindow->setWindowTitle("任务管理器");
    taskManagerWindow->resize(600, 400); // 根据需要调整大小

    // 创建一个表格并设置其布局
    QTableWidget *tableWidget = new QTableWidget(0, 5, taskManagerWindow); // 5列
    QStringList headers = {"任务名称", "任务状态", "CPU", "内存", "磁盘"};
    tableWidget->setHorizontalHeaderLabels(headers);
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 使列自动拉伸以填充整个宽度

    // 添加一些示例行
    for (int i = 0; i < 10; ++i)
    {
        tableWidget->insertRow(i);
        tableWidget->setItem(i, 0, new QTableWidgetItem("任务" + QString::number(i + 1)));
        tableWidget->setItem(i, 1, new QTableWidgetItem("运行中"));
        // 其他列也可以这样设置
    }

    // 使用布局管理器将表格添加到窗口中
    QWidget *centralWidget = new QWidget(taskManagerWindow);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->addWidget(tableWidget);
    centralWidget->setLayout(layout);
    taskManagerWindow->setCentralWidget(centralWidget);

    // 显示任务管理器窗口
    taskManagerWindow->show();
}
