/*
 * @Author       : NieFire planet_class@foxmail.com
 * @Date         : 2024-01-03 20:15:46
 * @LastEditors  : NieFire planet_class@foxmail.com
 * @LastEditTime : 2024-01-09 17:20:05
 * @FilePath     : \OS-Experiment\src\HelloWorld.cpp
 * @Description  :
 * ( ﾟ∀。)只要加满注释一切都会好起来的( ﾟ∀。)
 * Copyright (c) 2024 by NieFire, All Rights Reserved.
 */
#include "HelloWorld.h"

TaskScheduler &scheduler = TaskScheduler::getInstance();

// 目录结点类型枚举类
enum class FolderType
{
    // 文件夹
    FOLDER,
    // 文件
    FILE
};

// 目录结点类，继承自QStandardItem
class QFolderItem : public QStandardItem
{
public:
    // 有参构造函数，参数为结点名称和结点类型
    QFolderItem(QString name, FolderType type, Folder *folder) : QStandardItem(name)
    {
        this->type = type;
        this->folder = folder;
    }

    // 获取结点类型
    FolderType getType()
    {
        return type;
    }

    // Folder指针
    Folder *folder;

private:
    // 结点类型
    FolderType type;
};

// 文件子类，添加一个File指针
class QFolderItemFileStyle : public QFolderItem
{
public:
    // 有参构造函数，参数为结点名称和结点类型
    QFolderItemFileStyle(QString name, FolderType type, Folder *folder, File *thisFile) : QFolderItem(name, type, folder)
    {
        this->thisFile = thisFile;
    }

    // File指针
    File *thisFile;
};

// 文件夹子类，添加一个File指针
class QFolderItemFolderStyle : public QFolderItem
{
public:
    // 有参构造函数，参数为结点名称和结点类型
    QFolderItemFolderStyle(QString name, FolderType type, Folder *folder, Folder *thisFolder) : QFolderItem(name, type, folder)
    {
        this->thisFolder = thisFolder;
    }

    // Folder指针
    Folder *thisFolder;
};

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
        QColor color;

        // 首先检查进程ID是否已经存在于映射中
        if (colorMap.find(processId) == colorMap.end())
        {
            // 如果不存在，则生成一个随机颜色
            color = generateRandomColor();

            // 将进程ID和颜色添加到映射中
            colorMap[processId] = color;
        }

        // blockNumber是否合法
        if (blockNumber < 0 || blockNumber > 63)
        {
            return;
        }

        // tip 通过上述两段if检查过后，能保证参数的合法性

        // 获取进程ID对应的颜色
        color = colorMap[processId];

        // 获取网格布局
        // todo 希望能在这里面，将layout()的访问权限封闭
        QGridLayout *gridLayout = static_cast<QGridLayout *>(this->layout());

        // 获取内存块号对应的标签
        QLabel *label = static_cast<QLabel *>(gridLayout->itemAtPosition(blockNumber / 8, blockNumber % 8)->widget());

        // 设置标签的背景颜色与进程ID
        label->setStyleSheet(QString("background-color: %1").arg(color.name()));
        label->setText(QString::number(processId));
    }

    // 信息解析
    void parseMemoryBlocksInfo(std::vector<int> memoryBlocksInfo)
    {
        // 遍历数组，将每个内存块的占用情况显示出来
        for (int i = 0; i < memoryBlocksInfo.size(); ++i)
        {
            // 检查内存块号是否合法
            if (memoryBlocksInfo[i] < -1 || memoryBlocksInfo[i] > 63)
            {
                continue;
                // todo 抛出异常
            }

            draw(memoryBlocksInfo[i], i);
            // todo：三级制调用：传入信息并解析信息的函数，将信息写入暂存的一个函数，将暂存信息显示在界面上的函数
        }
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

        // tip 复杂条件判断语句，抽出来写
        // return isBlack(r, g, b) || isWhite(r, g, b);

        // tip 改成一个参数，rgb用自定义的结构体表示
        // RGB rgb(r, g, b);
        // return isBlackOrWhite(rgb);
    
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

                // 标记第几块
                label->setText(QString::number(row * 4 + col));

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

                // 标记第几块
                label->setText(QString::number(row * 30 + col + 124));

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

    // 解析传来的信息1024块磁盘的占用情况，是一个1024大小的std：bool数组
    void parseDiskBlocksInfo(std::vector<bool> diskBlocksInfo)
    {
        // 遍历数组，将每个盘块的占用情况显示出来
        for (int i = 0; i < diskBlocksInfo.size(); ++i)
        {
            draw(diskBlocksInfo[i], i);
        }
    }
};

class GroupBlockWidget : public QWidget
{
private:
    // 八个布局，每个布局对应一个组
    std::vector<QGridLayout *> groupLayouts;

public:
    // 初始化函数
    GroupBlockWidget(QWidget *parent = nullptr) : QWidget(parent)
    {
        // 成组链块分八个组，每组有128个盘块（4*32的网格布局），使用QSplitter分割
        QSplitter *splitter = new QSplitter(this);
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->addWidget(splitter);

        // 创建八个组的布局
        for (int i = 0; i < 8; ++i)
        {
            QFrame *frame = new QFrame();
            frame->setFrameStyle(QFrame::Box | QFrame::Raised); // 设置边框样式为凸起的方框
            frame->setLineWidth(2);                             // 设置边框宽度
            QVBoxLayout *layout = new QVBoxLayout(frame);
            layout->setContentsMargins(5, 5, 5, 5); // 设置布局边距

            // 组名称标签
            QLabel *label = new QLabel(QString("组%1").arg(i + 1), frame);
            label->setAlignment(Qt::AlignCenter);
            layout->addWidget(label);

            // 创建网格布局
            QGridLayout *gridLayout = new QGridLayout();
            gridLayout->setSpacing(2);
            layout->addLayout(gridLayout);

            // 生成空白的网格
            for (int row = 0; row < 32; ++row)
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

                    // 将标签添加到网格布局中
                    gridLayout->addWidget(label, row, col);
                }
            }

            // 将组布局添加到分割器中
            splitter->addWidget(frame);

            // 将组布局添加到组布局数组中
            groupLayouts.push_back(gridLayout);
        }
    }

    // 供外部调用的绘制函数，参数第n块，块号
    void draw(int n, int blockNumber)
    {
        // 找到第n个块所在的组，这个块显示块号
        int groupNumber = n / 128;
        int blockNumberInGroup = n % 128;

        // 获取网格布局中的标签
        QLabel *label = static_cast<QLabel *>(groupLayouts[groupNumber]->itemAtPosition(blockNumberInGroup / 4, blockNumberInGroup % 4)->widget());

        // 设置标签
        label->setText(QString::number(blockNumber));
    }

    // 解析传来的信息：1024+8大小的int数组，如果是-1说明进入下一个组，否则显示块号，如果是-2说明结束
    void parseGroupBlocksInfo(std::vector<int> groupBlocksInfo)
    {
        int n = 0;
        // 遍历数组，将每个盘块的占用情况显示出来
        for (int i = 0; i < groupBlocksInfo.size(); ++i)
        {
            if (groupBlocksInfo[i] == -2)
            {
                break;
            }
            else if (groupBlocksInfo[i] == -1)
            {
                continue;
            }
            else
            {
                draw(n, groupBlocksInfo[i]);
                n++;
            }
        }
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

// 文本文件窗口，继承自QMainWindow
class TextFileWindow : public QMainWindow
{
private:
    File *file;
    FileInfo *fileInfo;

    // 文本编辑器
    QTextEdit *textEdit;

    // 执行进程
    ExecutionProcess exeProc;

    

public:
    // 有参构造函数
    TextFileWindow(QWidget *parent = nullptr, File *file = nullptr) : QMainWindow(parent)
    {
        this->file = file;
        this->fileInfo = new FileInfo(file);

        exeProc = ExecutionProcess::create("文本文件读写进程", PIDGenerator::generatePID(), 3, fileInfo, OperationCommand::CREATE_READ_WRITE);

        setWindowTitle(QString::fromStdString(file->get_Name()));
        resize(800, 600);

        textEdit = new QTextEdit(this);
        setCentralWidget(textEdit);

        // 为文本编辑器添加事件过滤器
        textEdit->installEventFilter(this);

        // 读取文件内容
        readFileContent(fileInfo);
    }

    bool eventFilter(QObject *obj, QEvent *event) override
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

    void saveFileContent()
    {
        // 先把文本编辑器的内容写入File
        string data = textEdit->toPlainText().toStdString();
        // file->set_Content(data);
string *newData = new string(data);
        fileInfo->setData(newData);

        // 挂载execute_write函数

        exeProc.execute_write(file, &exeProc);

        // exeProc.execute_read(file, &exeProc);

        // textFileWindow->readFileContent(fileInfo);
    }

    // 重写关闭事件，关闭窗口时保存文件内容并从就绪队列中移除
    void closeEvent(QCloseEvent *event) override
    {
        saveFileContent();

        scheduler.end();
        
        event->accept();
    }

    // 读取文件内容
    void readFileContent(FileInfo *fileInfo)
    {
exeProc.execute_read(file, &exeProc);
        // 写进文本编辑器
        string data = fileInfo->getData();
        textEdit->setText(QString::fromLocal8Bit(data));
    }
};

// 目录的显示美化布局
// TODO 改成简单易懂的名字
class CustomItemDelegate : public QStyledItemDelegate
{
Folder *root;

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
    }

    // 生成根节点
    void parseFolderInfo(Folder *root)
    {
        string name = root->get_Name();
        string type = root->get_Type();

        int size = root->get_Size();
        int file_number = root->get_File_number();
        int folder_number = root->get_Folder_number();

        string create_time = root->get_Create_time();
        string change_time = root->get_Change_time();

        // 生成一个新的根节点
        QFolderItem *rootItem = new QFolderItem(QString::fromLocal8Bit(name), FolderType::FOLDER, root);
        model->appendRow(rootItem);

        inlineParseFolderInfo(root, rootItem);
    }

    void inlineParseFolderInfo(Folder *nowFolder, QStandardItem *rootItem)
    {
        // 遍历子文件对象
        vector<File *> file_child = nowFolder->get_File_child();
        for (int i = 0; i < file_child.size(); i++)
        {
            string name = file_child[i]->get_Name();
            int size = file_child[i]->get_Size();
            string create_time = file_child[i]->get_Create_time();
            string change_time = file_child[i]->get_Change_time();

            // 生成一个新的子文件节点
            // QStandardItem *fileItem = new QStandardItem(QString::fromLocal8Bit(name));
            QFolderItem *fileItem = new QFolderItemFileStyle(QString::fromLocal8Bit(name), FolderType::FILE, nowFolder, file_child[i]);
            rootItem->appendRow(fileItem);
        }

        // 递归遍历子文件夹对象，使用新的递归函数
        vector<Folder *> folder_child = nowFolder->get_Folder_child();
        for (int i = 0; i < folder_child.size(); i++)
        {
            // 生成一个新的子文件夹节点
            // QStandardItem *folderItem = new QStandardItem(QString::fromLocal8Bit(folder_child[i]->get_Name()));
            QFolderItem *folderItem = new QFolderItemFolderStyle(QString::fromLocal8Bit(folder_child[i]->get_Name()), FolderType::FOLDER, nowFolder, folder_child[i]);
            rootItem->appendRow(folderItem);

            // parseFolderInfo(folder_child[i]);
            inlineParseFolderInfo(folder_child[i], folderItem);
        }
    }

    // 先清空，再解析的包装函数、
    void parseFolderInfoWrapper(Folder *root)
    {
        model->clear();
        parseFolderInfo(root);
    }

    // 设置root
    void set_root(Folder *root)
    {
        this->root = root;
    }

private:
    // 便于后续操作的指针
    QStandardItemModel *model;

    Folder *root;

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

        model = new QStandardItemModel(treeView);
        treeView->setModel(model);

        // 添加标识列的标题
        model->setHorizontalHeaderLabels(QStringList() << "名称"
                                                       << "大小"
                                                       << "创建时间"
                                                       << "修改时间");

        // 根据根文件夹对象，解析目录信息
        parseFolderInfoWrapper(root);

        // 设置树形视图的列宽自适应内容
        treeView->resizeColumnToContents(0);

        // 使用自定义ItemDelegate来美化视图的显示
        CustomItemDelegate *itemDelegate = new CustomItemDelegate(treeView);
        treeView->setItemDelegate(itemDelegate);

        // 为树形视图添加上下文菜单
        treeView->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(treeView, &QTreeView::customContextMenuRequested, this, [=](const QPoint &pos)
                {

        // QStandardItem *item = model->itemFromIndex(treeView->indexAt(pos));
        QFolderItem *item = static_cast<QFolderItem *>(model->itemFromIndex(treeView->indexAt(pos)));
        QModelIndex index = treeView->indexAt(pos);
        if (index.isValid()) {
        QMenu menu;

        QString name = index.data().toString();
        Folder *folder = item->folder;

        //右键点击的是文件
        if (item->getType() == FolderType::FILE) {

            QAction *readFileAction = menu.addAction("读写文件");
            connect(readFileAction, &QAction::triggered, this, [=]() {
                // 解析为文件子类
                QFolderItemFileStyle *fileItem = static_cast<QFolderItemFileStyle *>(item);
                File *file = fileItem->thisFile;

                readWriteFile(file);

            });

            QAction *renameFileAction = menu.addAction("重命名文件");
            connect(renameFileAction, &QAction::triggered, this, [=]() {
                // 先弹出一个对话框，让用户输入文件名
                QString fileName = QInputDialog::getText(myComputerWindow, "重命名文件", "请输入文件名");
                // 如果用户点击取消，直接返回
                if (fileName.isEmpty()) {
                    return;
                }

                // 解析QFolderItem为文件子类
                QFolderItemFileStyle *fileItem = static_cast<QFolderItemFileStyle *>(item);

                renameFile(folder,fileItem->thisFile,fileName.toStdString());
            
            }); 

            QAction *deleteFileAction = menu.addAction("删除文件");
            connect(deleteFileAction, &QAction::triggered, this, [=]() {
                // 解析QFolderItem为文件子类
                QFolderItemFileStyle *fileItem = static_cast<QFolderItemFileStyle *>(item);

                // 先弹出一个对话框，让用户确定是否删除
                QMessageBox msgBox(myComputerWindow);
                msgBox.setWindowTitle("删除文件");
                msgBox.setText("确定要删除文件吗？");

                msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                QPushButton* yesButton = qobject_cast<QPushButton*>(msgBox.button(QMessageBox::Yes));
                QPushButton* noButton = qobject_cast<QPushButton*>(msgBox.button(QMessageBox::No));

                QString buttonStyle = "QPushButton {"
                                    "    border: 1px solid black;"
                                    "    padding: 5px;"
                                    "}";

                yesButton->setStyleSheet(buttonStyle);
                noButton->setStyleSheet(buttonStyle);

                QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(msgBox.layout());
                if (layout)
                {
                    layout->setContentsMargins(20, 10, 20, 10);
                    layout->setSpacing(20);
                }

                QMessageBox::StandardButton reply = static_cast<QMessageBox::StandardButton>(msgBox.exec());
            
                if (reply == QMessageBox::Yes) 
                {
                    deleteFile(fileItem->thisFile);
                }
                else 
                {
                    return;
                }
            });
        }

        //右键点击的文件夹
        else {
            QAction *newFileAction = menu.addAction("新建文件");
            connect(newFileAction, &QAction::triggered, this, [=]() {
                // 解析QFolderItem为文件夹子类
                QFolderItemFolderStyle *folderItem = static_cast<QFolderItemFolderStyle *>(item);
                Folder *thisFolder = folderItem->folder;
                // 先弹出一个对话框，让用户输入文件名
                QString fileName = QInputDialog::getText(myComputerWindow, "新建文件", "请输入文件名");
                // 如果用户点击取消，直接返回
                if (fileName.isEmpty()) {
                    return;
                }
                createNewFile(thisFolder,fileName.toStdString());

                
            });

            QAction *newFolderAction = menu.addAction("新建文件夹");
            connect(newFolderAction, &QAction::triggered, this, [=]() {
                // 解析QFolderItem为文件夹子类
                QFolderItemFolderStyle *folderItem = static_cast<QFolderItemFolderStyle *>(item);
                Folder *folder = folderItem->folder;
                // 先弹出一个对话框，让用户输入文件夹名
                QString folderName = QInputDialog::getText(myComputerWindow, "新建文件夹", "请输入文件夹名");
                // 如果用户点击取消，直接返回
                if (folderName.isEmpty()) {
                    return;
                }
                createNewFolder(folder,folderName.toStdString());
            });

            QAction *deleteFolderAction = menu.addAction("删除文件夹");
            connect(deleteFolderAction, &QAction::triggered, this, [=]() {
                // 解析QFolderItem为文件夹子类
                QFolderItemFolderStyle *folderItem = static_cast<QFolderItemFolderStyle *>(item);

                // 先弹出一个对话框，让用户确定是否删除
                QMessageBox msgBox(myComputerWindow);
                msgBox.setWindowTitle("删除文件夹");
                msgBox.setText("确定要删除文件夹吗？");

                msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                QPushButton* yesButton = qobject_cast<QPushButton*>(msgBox.button(QMessageBox::Yes));
                QPushButton* noButton = qobject_cast<QPushButton*>(msgBox.button(QMessageBox::No));

                QString buttonStyle = "QPushButton {"
                                    "    border: 1px solid black;"
                                    "    padding: 5px;"
                                    "}";

                yesButton->setStyleSheet(buttonStyle);
                noButton->setStyleSheet(buttonStyle);

                QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(msgBox.layout());
                if (layout)
                {
                    layout->setContentsMargins(20, 10, 20, 10);
                    layout->setSpacing(20);
                }

                QMessageBox::StandardButton reply = static_cast<QMessageBox::StandardButton>(msgBox.exec());
            
                if (reply == QMessageBox::Yes) 
                {
                    deleteFolder(folderItem->thisFolder);
                }
                else 
                {
                    return;
                }
            });

            QAction *renameFolderAction = menu.addAction("重命名文件夹");
            connect(renameFolderAction, &QAction::triggered, this, &MyComputerButton::renameFolder);
        }
        
        menu.exec(treeView->mapToGlobal(pos));
    } });

        // 显示新窗口
        myComputerWindow->setCentralWidget(treeView);
        myComputerWindow->show();
    }

    // 新建文件
    void createNewFile(Folder *parentFolder, string fileName)
    {
        File *file = new File(fileName);
        // 遍历root，找到folderName对应的文件夹
        // Folder *target_folder = findFolder(root, folderName);

        string *fileNamePtr = new string(fileName);
        FileInfo *fileInfo = new FileInfo(parentFolder, fileNamePtr);

        DataGenerationProcess::create("文件生成进程", PIDGenerator::generatePID(), 1, fileInfo, OperationCommand::CREATE_FILE);

        scheduler.schedule();

        parseFolderInfoWrapper(root);
    }

    // 新建文件夹
    void createNewFolder(Folder *parentFolder, string folderName)
    {
        Folder *folder = new Folder(folderName);

        // 遍历root，找到parentFolderName对应的文件夹
        // Folder *target_folder = findFolder(root, parentFolderName);

        string *folderNamePtr = new string(folderName);
        FileInfo *fileInfo = new FileInfo(parentFolder, folderNamePtr);

        DataGenerationProcess::create("文件夹生成进程", PIDGenerator::generatePID(), 1, fileInfo, OperationCommand::CREATE_FOLDER);

        scheduler.schedule();

        parseFolderInfoWrapper(root);
    }

    // 删除文件夹
    void deleteFolder(Folder *Folder)
    {
        FileInfo *fileInfo = new FileInfo(Folder);

        DataDeletionProcess::create("文件夹删除进程", PIDGenerator::generatePID(), 1, fileInfo, OperationCommand::DELETE_FOLDER);

        scheduler.schedule();

        parseFolderInfoWrapper(root);
    }

    // 重命名文件夹
    void renameFolder()
    {
        // todo
    }

    // 读写文件
    void readWriteFile(File *file)
    {
        TextFileWindow *textFileWindow = new TextFileWindow(this, file);
        textFileWindow->show();

        FileInfo *fileInfo = new FileInfo(file);

        // string *data = new string("你学塔菲！");

        // fileInfo->setData(data);

       // ExecutionProcess exeProc = ExecutionProcess::create("文本文件读写进程", PIDGenerator::generatePID(), 1, fileInfo, OperationCommand::CREATE_READ_WRITE);

        // exeProc.execute_write(file, &exeProc);

        // // 窗口先关闭
        // textFileWindow->close();

        // exeProc.execute_read(file, &exeProc);

        // textFileWindow->readFileContent(fileInfo);

        // // 窗口再显示
        // textFileWindow->show();
    }

    // 重命名文件
    void renameFile(Folder *parentFolder, File *file, string newFileName)
    {
        FileInfo *fileInfo = new FileInfo(parentFolder, file, new string(newFileName));

        DataGenerationProcess::create("文件重命名进程", PIDGenerator::generatePID(), 1, fileInfo, OperationCommand::RENAME_FILE);

        scheduler.schedule();

        parseFolderInfoWrapper(root);
    }

    // 删除文件
    void deleteFile(File *file)
    {
        FileInfo *fileInfo = new FileInfo(file);

        if (!DataDeletionProcess::create("文件删除进程", PIDGenerator::generatePID(), 1, fileInfo, OperationCommand::DELETE_FILE))
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle("删除文件");
            msgBox.setText("有文件正在读取该文件，无法删除！");

            msgBox.setStandardButtons(QMessageBox::Yes);
            QPushButton *yesButton = qobject_cast<QPushButton *>(msgBox.button(QMessageBox::Yes));

            QString buttonStyle = "QPushButton {"
                                  "    border: 1px solid black;"
                                  "    padding: 5px;"
                                  "}";

            yesButton->setStyleSheet(buttonStyle);

            QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(msgBox.layout());
            if (layout)
            {
                layout->setContentsMargins(20, 10, 20, 10);
                layout->setSpacing(20);
            }

            msgBox.exec();

            return;
        };

        scheduler.schedule();

        parseFolderInfoWrapper(root);
    }
};

HelloWorld::HelloWorld(QWidget *parent)
    : QMainWindow(parent), ui(new Ui_HelloWorld)
{
    ui->setupUi(this);

    initProc.execute();

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

    mycomputer_btn->set_root(initProc.get_folder());

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

    // 添加成组链块选项卡
    QWidget *groupBlockTab = new QWidget(tabWidget);

    // 再加一个选项卡
    QWidget *anotherTab = new QWidget(tabWidget);

    // 创建布局管理器
    QVBoxLayout *processLayout = new QVBoxLayout(processTab);
    QVBoxLayout *memoryLayout = new QVBoxLayout(memoryTab);
    QVBoxLayout *diskLayout = new QVBoxLayout(diskTab);
    QVBoxLayout *groupBlockLayout = new QVBoxLayout(groupBlockTab);
    QVBoxLayout *anotherLayout = new QVBoxLayout(anotherTab);

    // 将布局管理器设置给选项卡
    processTab->setLayout(processLayout);
    memoryTab->setLayout(memoryLayout);
    diskTab->setLayout(diskLayout);
    groupBlockTab->setLayout(groupBlockLayout);
    anotherTab->setLayout(anotherLayout);

    // 将选项卡添加到选项卡窗口中
    tabWidget->addTab(processTab, "进程");
    tabWidget->addTab(memoryTab, "内存");
    tabWidget->addTab(diskTab, "磁盘");
    tabWidget->addTab(groupBlockTab, "成组链块");
    tabWidget->addTab(anotherTab, "其他");

    // 将选项卡窗口设置为任务管理器窗口的中心部件
    taskManagerWindow->setCentralWidget(tabWidget);

    // 创建进程表格
    QTableWidget *tableWidget = new QTableWidget(0, 4, taskManagerWindow); // 4列
    QStringList headers = {"进程名", "进程id", "进程状态", "使用内存块"};
    tableWidget->setHorizontalHeaderLabels(headers);
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 使列自动拉伸以填充整个宽度
    // 将进程表格添加到进程选项卡中
    processLayout->addWidget(tableWidget);

    // 创建内存UI，8*8网格，每个网格中显示一个占用该内存块的进程id，被同一个进程占用的内存块显示为同一种颜色（随机生成但不为黑白两色）
    MemoryWidget *memoryWidget = new MemoryWidget(memoryTab);
    memoryLayout->addWidget(memoryWidget);

    // 创建磁盘UI，显示磁盘的使用情况，左边有一个小长方形显示124块兑换区的使用情况，右边有一个大一点的长方形显示文件区的使用情况
    // 如果某一块被占用，则显示为蓝色，否则显示为白色
    DiskWidget *diskWidget = new DiskWidget(diskTab);
    diskLayout->addWidget(diskWidget);

    // 创建成组链块UI，显示成组链块的使用情况，每个组有128个盘块，一共有8个组，每个组用一个小长方形显示，如果某一块被占用，则显示为蓝色，否则显示为白色
    GroupBlockWidget *groupBlockWidget = new GroupBlockWidget(groupBlockTab);
    groupBlockLayout->addWidget(groupBlockWidget);

    std::vector<Process *> processList = get_process_list();
    int processNumber = processList.size();
    for (int i = 0; i < processList.size(); ++i)
    {
        tableWidget->insertRow(i);
        Process *temp = processList[i];
        if (temp == nullptr)
        {
            continue;
        }
        // 依次检查
        string Tname = temp->name;
        int Tpid = temp->pid;
        string Tstate = temp->getProcessStateStr();

        QString processName = QString::fromLocal8Bit(processList[i]->name);
        tableWidget->setItem(i, 0, new QTableWidgetItem(processName));
        tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(processList[i]->pid)));
        tableWidget->setItem(i, 2, new QTableWidgetItem(QString::fromLocal8Bit(processList[i]->getProcessStateStr())));
        std::vector<int> memoryBlocks = processList[i]->allocatedMemory;
        QString memoryBlocksStr = "";
        for (int j = 0; j < memoryBlocks.size(); ++j)
        {
            memoryBlocksStr += QString::number(memoryBlocks[j]) + ",";
        }
        tableWidget->setItem(i, 3, new QTableWidgetItem(memoryBlocksStr));
    }

    // 解析内存块信息
    memoryWidget->parseMemoryBlocksInfo(show_process_record());

    // 解析磁盘块信息
    diskWidget->parseDiskBlocksInfo(show_disk_block_status());

    // 解析成组链块信息
    groupBlockWidget->parseGroupBlocksInfo(show_group_block_status());

    // 显示任务管理器窗口
    taskManagerWindow->show();
}