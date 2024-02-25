/*
 * @Author       : NieFire planet_class@foxmail.com
 * @Date         : 2024-01-03 20:15:46
 * @LastEditors  : NieFire planet_class@foxmail.com
 * @LastEditTime : 2024-02-25 14:15:03
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
        TextFileWindow *textFileWindow = new TextFileWindow(this);
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
    status_btn = new StatusBar(this);

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
        showTaskManager();
    });

    // 显示任务管理器窗口
    task->show(); });

    /**
     * 左下角WIN图标部分，目前为一个Button,连接着win_window
     */
    win_btn = new Win(this);
    win_btn->show();

    connect(win_btn, &QPushButton::clicked, [=]()
            { showWinWindow(); });

    /**
     * 右下角时间部分，目前为一个Label
     */
    timeLabel = new DateTimeLabel(this);
}

HelloWorld::~HelloWorld()
{
    delete ui;
}

void HelloWorld::close()
{
}

void HelloWorld::notify(QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        // 将全局坐标转换为相对于窗口的坐标
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        QPointF pos = this->mapFromGlobal(mouseEvent->globalPosition());
        // debug 检查坐标是否正确
        int x = pos.x();
        int y = pos.y();
        QMouseEvent *newEvent = new QMouseEvent(mouseEvent->type(), pos, mouseEvent->button(), mouseEvent->buttons(), mouseEvent->modifiers());

        if (mouseEvent->button() == Qt::LeftButton)
        {
            emit globalLeftClicked(newEvent);
        }
        if (mouseEvent->button() == Qt::RightButton)
        {
            emit globalRightClicked(newEvent);
        }

        emit globalClicked(newEvent);
    }
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
    TaskManagerWidget *taskManagerWindow = new TaskManagerWidget(this);

    taskManagerWindow->updateProcessData();
    taskManagerWindow->updateMemoryData(show_process_record());
    taskManagerWindow->updateDiskData(show_disk_block_status());
    taskManagerWindow->updateGroupBlockData(show_group_block_status());

    // 显示任务管理器窗口
    taskManagerWindow->show();
}

void HelloWorld::showWinWindow()
{
    // 传入win_btn的位置
    win_window = new WinWindow(this, win_btn->pos().x(), win_btn->pos().y());

    void (HelloWorld::*globalLeftClicked)(QMouseEvent *) = &HelloWorld::globalLeftClicked;
    void (WinWindow::*globalMousePressEvent)(QMouseEvent *) = &WinWindow::globalMousePressEvent;
    connect(this, globalLeftClicked, win_window, globalMousePressEvent);

    win_window->show();
}