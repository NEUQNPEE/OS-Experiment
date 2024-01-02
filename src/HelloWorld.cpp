#include "HelloWorld.h"

class MemoryWidget : public QWidget
{
public:
    // 初始化函数，生成纯白色，进程id为-1的8*8网格
    MemoryWidget(QWidget *parent = nullptr) : QWidget(parent)
    {
        // 创建一个8x8的网格布局
        QGridLayout *gridLayout = new QGridLayout(this);
        gridLayout->setSpacing(2);

        // 生成纯白色，进程id为-1的8*8网格
        for (int row = 0; row < 8; ++row)
        {
            for (int col = 0; col < 8; ++col)
            {
                // 创建一个标签，显示进程ID，并设置背景颜色
                QLabel *label = new QLabel(QString::number(-1), this);
                label->setAlignment(Qt::AlignCenter);
                label->setStyleSheet("background-color: white");

                // 给标签设置一个边框
                label->setFrameStyle(QFrame::Box | QFrame::Raised);

                // 将标签添加到网格布局中
                gridLayout->addWidget(label, row, col);
            }
        }
    }

    // 供外部调用的绘制函数，参数为进程ID和内存块号
    void draw(int processId, int blockNumber)
    {
        // 首先检查进程ID是否已经存在于映射中
        if (colorMap.find(processId) == colorMap.end())
        {
            // 如果不存在，则生成一个随机颜色
            QColor color = generateRandomColor();

            // 将进程ID和颜色添加到映射中
            colorMap[processId] = color;
        }

        // 获取进程ID对应的颜色
        QColor color = colorMap[processId];

        // 获取网格布局
        QGridLayout *gridLayout = static_cast<QGridLayout *>(this->layout());

        // 获取内存块号对应的标签
        QLabel *label = static_cast<QLabel *>(gridLayout->itemAtPosition(blockNumber / 8, blockNumber % 8)->widget());

        // 设置标签的背景颜色与进程ID
        label->setStyleSheet(QString("background-color: %1").arg(color.name()));
        label->setText(QString::number(processId));
    }

private:
    // 存储进程ID和颜色的映射
    std::map<int, QColor> colorMap;

    // 生成随机颜色，但不为黑白两色
    QColor generateRandomColor()
    {
        int r, g, b;
        do
        {
            r = rand() % 256;
            g = rand() % 256;
            b = rand() % 256;
        } while (isBlackOrWhite(r, g, b));

        return QColor(r, g, b);
    }

    bool isBlackOrWhite(int r, int g, int b)
    {
        // 判断颜色是否为黑色或白色
        return (r < 10 && g < 10 && b < 10) || (r > 245 && g > 245 && b > 245);
    }
};

class DiskWidget : public QWidget
{
private:
    // 兑换区和文件区的指针，便于后续操作
    QGridLayout *exchangeGridLayout;
    QGridLayout *fileGridLayout;

public:
    // 初始化函数
    DiskWidget(QWidget *parent = nullptr) : QWidget(parent)
    {
        // 创建整体布局
        QVBoxLayout *mainLayout = new QVBoxLayout(this);

        // 创建 QSplitter，用于分割兑换区和文件区
        QSplitter *splitter = new QSplitter(this);
        mainLayout->addWidget(splitter);

        // 创建兑换区部件
        QFrame *exchangeFrame = new QFrame();
        exchangeFrame->setFrameStyle(QFrame::Box | QFrame::Raised); // 设置边框样式为凸起的方框
        exchangeFrame->setLineWidth(2);                             // 设置边框宽度
        QVBoxLayout *exchangeLayout = new QVBoxLayout(exchangeFrame);
        exchangeLayout->setContentsMargins(5, 5, 5, 5); // 设置布局边距

        // 兑换区名称标签
        QLabel *exchangeLabel = new QLabel("兑换区", exchangeFrame);
        exchangeLabel->setAlignment(Qt::AlignCenter);
        exchangeLayout->addWidget(exchangeLabel);

        // 创建文件区部件
        QFrame *fileFrame = new QFrame();
        fileFrame->setFrameStyle(QFrame::Box | QFrame::Raised); // 设置边框样式为凸起的方框
        fileFrame->setLineWidth(2);                             // 设置边框宽度
        QVBoxLayout *fileLayout = new QVBoxLayout(fileFrame);
        fileLayout->setContentsMargins(5, 5, 5, 5); // 设置布局边距

        // 文件区名称标签
        QLabel *fileLabel = new QLabel("文件区", fileFrame);
        fileLabel->setAlignment(Qt::AlignCenter);
        fileLayout->addWidget(fileLabel);

        // 创建兑换区布局（31行4列的网格布局）
        exchangeGridLayout = new QGridLayout();
        exchangeGridLayout->setSpacing(2);
        exchangeLayout->addLayout(exchangeGridLayout);

        // 创建文件区布局（30行30列的网格布局）
        fileGridLayout = new QGridLayout();
        fileGridLayout->setSpacing(2);
        fileLayout->addLayout(fileGridLayout);

        // 生成空白的兑换区和文件区
        for (int row = 0; row < 31; ++row)
        {
            for (int col = 0; col < 4; ++col)
            {
                QLabel *label = new QLabel(this);
                label->setAlignment(Qt::AlignCenter);
                label->setStyleSheet(QString("background-color: white"));

                // 边框
                label->setFrameStyle(QFrame::Box | QFrame::Raised);

                // 标签大小最小为10x10
                label->setMinimumSize(10, 10);

                // 将标签添加到兑换区网格布局中
                exchangeGridLayout->addWidget(label, row, col);
            }
        }

        for (int row = 0; row < 30; ++row)
        {
            for (int col = 0; col < 30; ++col)
            {
                QLabel *label = new QLabel(this);
                label->setAlignment(Qt::AlignCenter);
                label->setStyleSheet(QString("background-color: white"));

                // 边框
                label->setFrameStyle(QFrame::Box | QFrame::Raised);

                // 标签大小最小为10x10
                label->setMinimumSize(10, 10);

                // 将标签添加到文件区网格布局中
                fileGridLayout->addWidget(label, row, col);
            }
        }

        // 将兑换区和文件区添加到分割器中
        splitter->addWidget(exchangeFrame);
        splitter->addWidget(fileFrame);
        splitter->setSizes(QList<int>() << this->width() / 5 << this->width() * 4 / 5);
    }

    // 供外部调用的绘制函数，参数为是否占用和块号
    void draw(bool isOccupied, int blockNumber)
    {

        // 获取网格布局中的标签
        QLabel *label = nullptr;
        if (blockNumber < 124)
        {
            // 如果块号小于124，则为兑换区
            label = static_cast<QLabel *>(exchangeGridLayout->itemAtPosition(blockNumber / 4, blockNumber % 4)->widget());
        }
        else
        {
            // 否则为文件区
            label = static_cast<QLabel *>(fileGridLayout->itemAtPosition((blockNumber - 124) / 30, (blockNumber - 124) % 30)->widget());
        }

        // 设置标签的背景颜色
        if (isOccupied)
        {
            label->setStyleSheet(QString("background-color: blue"));
        }
        else
        {
            label->setStyleSheet(QString("background-color: white"));
        }

        // 更新标签
        label->update();
    }
};

class TextFileButton : public QPushButton
{
public:
    // 有参构造函数，创建时就指定文件路径
    TextFileButton(QWidget *parent = nullptr, int height = 0, QString filePath = "") : QPushButton(parent)
    {
        QIcon recycle_icon("icon/TXT.png");
        QPixmap pixma = recycle_icon.pixmap(QSize(50, 50));
        setIcon(QIcon(pixma));
        setIconSize(pixma.size());
        setFixedSize(80, 100);
        move(0, height);

        setStyleSheet("QPushButton {"
                      "    border: none;"                 // 去除边框
                      "    background-repeat: no-repeat;" // 禁止平铺
                      "    background-position: center;"  // 居中对齐
                      "}");

        // 连接双击事件
        installEventFilter(this);

        textEdit = nullptr;

        this->filePath = filePath;

        fileContent = "你好呀，我的名字是永雏塔菲喵！";
    }

private:
    // 文本编辑器
    QTextEdit *textEdit;

    // 本按钮指向的文件地址
    QString filePath;

    // 模拟用的文件内容
    QString fileContent;

    bool eventFilter(QObject *obj, QEvent *event) override
    {
        if (event->type() == QEvent::MouseButtonDblClick)
        {
            openTextWindow();
            return true;
        }

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

    void openTextWindow()
    {
        QMainWindow *textWindow = new QMainWindow(this);
        textWindow->setWindowTitle("文本文件");
        textWindow->resize(800, 600);

        textEdit = new QTextEdit(textWindow);
        textWindow->setCentralWidget(textEdit);

        // 为文本编辑器添加事件过滤器
        textEdit->installEventFilter(this);

        // 读取文件内容,先模拟一下
        if (filePath == "root/first.txt")
        {
            textEdit->setText(fileContent);
        }

        textWindow->show();
    }

    // 能使用ctrl+s保存文件
    void saveFileContent()
    {
        if (textEdit)
        {
            fileContent = textEdit->toPlainText();
        }
    }
};

class CustomItemDelegate : public QStyledItemDelegate
{
public:
    explicit CustomItemDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        QStyleOptionViewItem newOption = option;
        newOption.textElideMode = Qt::ElideMiddle; // 设置文本省略模式

        // 绘制背景
        painter->save();
        painter->fillRect(option.rect, option.palette.base());

        // 绘制文本
        QString text = index.data(Qt::DisplayRole).toString();
        painter->setFont(option.font);
        painter->setPen(option.palette.text().color());

        // 绘制图标（根据实际情况设置图标）
        // 图标就先用WLGOO.png代替了
        QIcon icon("icon/WLOGO.png");

        if (!icon.isNull())
        {
            int iconSize = option.decorationSize.height();
            QRect iconRect = QRect(option.rect.left() + 5, option.rect.top() + (option.rect.height() - iconSize) / 2,
                                   iconSize, iconSize);
            icon.paint(painter, iconRect);
        }

        // 文本在图标右侧，所以需要调整矩形
        QRect textRect = option.rect.adjusted(5, 0, -5, 0);
        textRect.setLeft(textRect.left() + 25);

        // 绘制文本
        painter->drawText(textRect, Qt::AlignVCenter, text);

        painter->restore();
    }
};

class MyComputerButton : public QPushButton
{
public:
    // 有参构造函数，创建时就指定文件路径
    MyComputerButton(QWidget *parent = nullptr, int height = 0) : QPushButton(parent)
    {
        QIcon recycle_icon("icon/mycomputer.png");
        QPixmap pixma = recycle_icon.pixmap(QSize(50, 50));
        setIcon(QIcon(pixma));
        setIconSize(pixma.size());
        setFixedSize(80, 100);
        move(0, height);

        setStyleSheet("QPushButton {"
                      "    border: none;"                 // 去除边框
                      "    background-repeat: no-repeat;" // 禁止平铺
                      "    background-position: center;"  // 居中对齐
                      "}");

        // 连接双击事件
        installEventFilter(this);

        // 写入模拟的目录信息
        rootEntry.type = "folder";
        rootEntry.name = "root";

        DirectoryEntry *subDir1 = new DirectoryEntry;
        subDir1->type = "folder";
        subDir1->name = "subdir1";

        DirectoryEntry *subDir2 = new DirectoryEntry;
        subDir2->type = "folder";
        subDir2->name = "subdir2";

        DirectoryEntry *file1 = new DirectoryEntry;
        file1->type = "file";
        file1->name = "file1.txt";

        DirectoryEntry *file2 = new DirectoryEntry;
        file2->type = "file";
        file2->name = "file2.txt";

        rootEntry.subdirectories.append(subDir1);
        rootEntry.subdirectories.append(subDir2);
        rootEntry.files.append(file1);
        rootEntry.files.append(file2);

        DirectoryEntry *subSubDir = new DirectoryEntry;
        subSubDir->type = "folder";
        subSubDir->name = "subsubdir";

        subDir1->subdirectories.append(subSubDir);

        subDir2->files.append(file1);
    }

private:
    // 模拟用的目录信息，一个链表，包括一个类型，一个名字，n个指向子目录的指针，n个指向子文件的指针
    struct DirectoryEntry
    {
        QString type;                           // 目录类型（例如：文件夹、文件）
        QString name;                           // 目录名字
        QList<DirectoryEntry *> subdirectories; // 指向子目录的指针链表
        QList<DirectoryEntry *> files;          // 指向子文件的指针链表
    };

    // 根目录
    DirectoryEntry rootEntry;

    bool eventFilter(QObject *obj, QEvent *event) override
    {
        if (event->type() == QEvent::MouseButtonDblClick)
        {
            openMyComputerWindow();
            return true;
        }
        return false;
    }

    void openMyComputerWindow()
    {
        QMainWindow *myComputerWindow = new QMainWindow(this);
        myComputerWindow->setWindowTitle("此电脑");
        myComputerWindow->resize(800, 600);

        // 根据模拟的目录信息，创建一个树形视图
        QTreeView *treeView = new QTreeView(myComputerWindow);
        treeView->setHeaderHidden(true); // 隐藏列头

        QStandardItemModel *model = new QStandardItemModel(treeView);
        treeView->setModel(model);

        // 创建根节点
        QStandardItem *rootItem = new QStandardItem(rootEntry.name);
        model->appendRow(rootItem);

        // 递归添加子目录和子文件
        addTreeItems(rootItem, rootEntry.subdirectories);
        addTreeItems(rootItem, rootEntry.files);

        // 设置树形视图的列宽自适应内容
        treeView->resizeColumnToContents(0);

        // 使用自定义ItemDelegate来美化视图的显示
        CustomItemDelegate *itemDelegate = new CustomItemDelegate(treeView);
        treeView->setItemDelegate(itemDelegate);

        // 显示新窗口
        myComputerWindow->setCentralWidget(treeView);
        myComputerWindow->show();
    }

    void addTreeItems(QStandardItem *parentItem, const QList<DirectoryEntry *> &entries)
    {
        foreach (DirectoryEntry *entry, entries)
        {
            QStandardItem *item = new QStandardItem(entry->name);
            parentItem->appendRow(item);

            // 递归添加子目录和子文件
            addTreeItems(item, entry->subdirectories);
            addTreeItems(item, entry->files);
        }
    }
};

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

    MyComputerButton *mycomputer_btn = new MyComputerButton(this, 0);

    // 创建文本文件图标,假设此图标指向的文件路径为root/first.txt
    TextFileButton *textFileIcon = new TextFileButton(this, mycomputer_btn->height(), "root/first.txt");

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

    /**
     * 右下角时间部分
     */
    // 创建一个标签，用于显示时间
    timeLabel = new QLabel(this);
    timeLabel->setStyleSheet("color: black; font-size: 20px;");
    timeLabel->setAlignment(Qt::AlignCenter);
    timeLabel->resize(200, 50);
    timeLabel->move(this->width() - timeLabel->width(), this->height() - timeLabel->height());
    timeLabel->show();

    // 创建一个定时器，每秒更新一次时间
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [=]()
            {
        // 获取当前时间
        QDateTime currentDateTime = QDateTime::currentDateTime();
        // 格式化时间
        QString currentTime = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");
        // 设置标签的文本
        timeLabel->setText(currentTime); });
    timer->start(1000); // 启动定时器，每秒触发一次
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

    // 更新右下角时间的位置
    if (timeLabel)
    {
        timeLabel->move(this->width() - timeLabel->width(), this->height() - timeLabel->height());
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

    // 在任务管理器窗口中添加三个栏目：进程、内存和磁盘
    QTabWidget *tabWidget = new QTabWidget(taskManagerWindow);
    tabWidget->setTabsClosable(false); // 禁止关闭选项卡
    tabWidget->setMovable(true);       // 允许拖动选项卡

    // 选项卡放在顶部
    tabWidget->setTabPosition(QTabWidget::North);

    // 创建三个选项卡
    QWidget *processTab = new QWidget(tabWidget);
    QWidget *memoryTab = new QWidget(tabWidget);
    QWidget *diskTab = new QWidget(tabWidget);

    // 创建布局管理器
    QVBoxLayout *processLayout = new QVBoxLayout(processTab);
    QVBoxLayout *memoryLayout = new QVBoxLayout(memoryTab);
    QVBoxLayout *diskLayout = new QVBoxLayout(diskTab);

    // 将布局管理器设置给选项卡
    processTab->setLayout(processLayout);
    memoryTab->setLayout(memoryLayout);
    diskTab->setLayout(diskLayout);

    // 将选项卡添加到选项卡窗口中
    tabWidget->addTab(processTab, "进程");
    tabWidget->addTab(memoryTab, "内存");
    tabWidget->addTab(diskTab, "磁盘");

    // 将选项卡窗口设置为任务管理器窗口的中心部件
    taskManagerWindow->setCentralWidget(tabWidget);

    // 创建进程表格
    QTableWidget *tableWidget = new QTableWidget(0, 4, taskManagerWindow); // 4列
    QStringList headers = {"任务名称", "任务状态", "内存", "磁盘"};
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

    // 将进程表格添加到进程选项卡中
    processLayout->addWidget(tableWidget);

    // 创建内存UI，8*8网格，每个网格中显示一个占用该内存块的进程id，被同一个进程占用的内存块显示为同一种颜色（随机生成但不为黑白两色）
    MemoryWidget *memoryWidget = new MemoryWidget(memoryTab);
    memoryLayout->addWidget(memoryWidget);

    // 创建磁盘UI，显示磁盘的使用情况，左边有一个小长方形显示124块兑换区的使用情况，右边有一个大一点的长方形显示文件区的使用情况
    // 如果某一块被占用，则显示为蓝色，否则显示为白色
    DiskWidget *diskWidget = new DiskWidget(diskTab);
    diskLayout->addWidget(diskWidget);

    // 测试用例：将内存块号为0-7的内存块分配给进程1，内存块号为8-15的内存块分配给进程2
    for (int i = 0; i < 8; ++i)
    {
        memoryWidget->draw(1, i);
    }
    for (int i = 8; i < 16; ++i)
    {
        memoryWidget->draw(2, i);
    }

    // 测试用例：将0-123号磁盘块改为占用状态
    for (int i = 0; i < 124; ++i)
    {
        diskWidget->draw(true, i);
    }

    // 显示任务管理器窗口
    taskManagerWindow->show();
}
