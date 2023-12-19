# vscode配置教程

## 1. 安装插件

需要插件：

* C/C++
* C/C++ Extension Pack
* CMake
* CMake Tools
* Qt Configure
* Qt tools

## 2. 配置环境变量

打开系统环境变量Path：

![Alt text](images/image.png)

添加如下两条：

E:\QT\Tools\mingw1120_64\bin
E:\QT\Tools\CMake_64\bin

（修改为自己的路径）

## 新建项目

项目已经建立完毕。你不必新建项目，但要走一遍 配置Qt安装目录 和 配置CMake 的流程。


### 配置Qt安装目录 

ctrl+shift+p打开命令面板，输入：

    >QtConfigure:Set Qt Dir

指定你的Qt安装目录。

### 创建Qt项目

输入：

    >QtConfigure:New Project

新建项目名称、选择编译套件（选mingw），选择构建工具（选cmake），是否带ui文件（选yes）。

这时候愚蠢的cmake会默认给你搞qt5，我们要改成qt6。因此找到项目根目录下的CMakeLists.txt，修改如下：

找到

    find_package(Qt5 COMPONENTS Widgets REQUIRED)

改成
    
    find_package(Qt6 COMPONENTS Widgets REQUIRED)。

找到

    target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets)

改成

    target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)

保存。

### 配置CMake

ctrl+shift+p打开命令面板，输入：

    > CMake:select a kit

选择你的编译套件。应该是GCC 11.2.0 x86_64-w64-mingw32。如果无选项可以使用Qt:Scan for Qt kit搜索，或重启vscode再搜索再选择。

### 修改QT路径

请修改以下内容：

* 根目录下的CMakeLists.txt：

```cmake
set(CMAKE_PREFIX_PATH "e:/QT/6.2.4/mingw_64") # Qt Kit Dir
```

这一行的路径应该为你自己QT的路径.

* .vscode文件夹下的launch.json：

```json
"environment": [
                {
                    "name": "PATH",
                    "value": "e:/QT/6.2.4/mingw_64/bin"
                }
            ],
            "externalConsole": false,
            "miDebuggerPath": "e:/QT/Tools/mingw1120_64/bin/gdb.exe",
            "visualizerFile": "c:\\Users\\tdfsbrx\\AppData\\Roaming\\Code\\User\\workspaceStorage\\d54ea10c6683dece2c54d94e32dd2227\\tonka3000.qtvsctools\\qt.natvis.xml"
```

value的路径改为你自己QT的路径,miDebuggerPath改为你自己QT的路径，visualizerFile应该是自动修改为你自己的路径了。

* .vscode文件夹下的settings.json：

```json
{
    "qtConfigure.qtKitDir": "e:/QT/6.2.4/mingw_64"
}
```

这一行的路径应该改为你自己的路径.

* .vscode文件夹下的tasks.json：


```json

 "label": "C/C++: g++.exe 生成活动文件",
        "command": "E:\\MinGW\\mingw64\\bin\\g++.exe",
```

command的路径应该改为你自己的路径.

## 运行

直接去main.cpp里面按F5运行。应该会跳出来一个窗口。

什么？你说上面include全是红波浪线？别管，不影响运行。

## 打开UI设计器

ctrl+shift+p打开命令面板，输入：

    > QtConfigure:Open Qt Designer
