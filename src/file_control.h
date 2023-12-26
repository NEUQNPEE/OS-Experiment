#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include <queue>
// 得到当前时间的函数
std::string gettime();
struct Folder;

// 文件结构体
struct File
{
private:
    /*文件属性：
    名称
    类型
    地址
    内容
    大小
    创建时间
    修改时间
    上级文件夹
    */
    std::string name;
    std::string type;
    std::string addr;
    int size = 0;
    std::string Create_time;
    std::string Change_time;
    std::string content = "";
    Folder *dad;

public:
    // 读文件属性函数
    std::string get_Name();
    std::string get_Type();
    std::string get_Addr();
    int get_Size();
    std::string get_Create_time();
    std::string get_Change_time();
    std::string get_Content();
    Folder *get_Dad();

    // 写文件属性函数
    void set_Name_Type(std::string str);
    void set_Content(std::string str);

    // 文件属性改变
    void change_Dad(Folder *folder);
    void change_Addr(std::string str);

    // 构造函数
    File();

    // 拷贝文件
    File *copy_File();

    // 删除文件
    void delete_File();

    // // 文件内容序列化
    // std::string file_serialize();

    // // 文件内容序列化
    // void file_deserialize(std::string str);

    /**
     * tip1：文件索引表在内存的系统区存储，任何访问文件的操作都是传入唯一标识符，内存访问索引表得到起始盘块号
     * tip1：根据fat表，得到文件的所有盘块号，根据盘块号，访问磁盘，得到文件内容，返回给文件系统
     * 
     * tip2：内存提供如下接口：
     * tip2: 1. 存储文件内容，传入唯一标识符，文件内容的字符串，（内存系统会自行解决），文件系统记得更新索引表
     * tip2: 2. 加载文件内容，传入唯一标识符，返回文件内容的字符串
     * tip2: 3. 删除文件函数，传入唯一标识符，删除索引表中的该标识符。同时内存还要通知磁盘释放该文件的所有盘块
     * tip2: 4. 创建文件函数，传入唯一标识符，创建索引表中的该标识符。开始时默认文件内容为空，起始盘块号为-1
     * tip2: 5. 存储目录信息，传入目录信息的序列化字符串，内存系统会自行确定存储位置
     * tip2: 6. 加载目录信息，返回目录信息的序列化字符串（目录信息确定了树形目录如何生成）
    */

    /**
     * tip: 目录信息存储了文件夹和文件的树形结构，但不存任何实质性内容
     */

    // todo 文件内容序列化，将name，addr，content之类需要序列化的内容序列化

    // todo 文件内容反序列化，调用内存的函数，重新生成文件对象

    /**
     * tip3: 文件系统向进程提供如下接口：
     * tip3: 1. 获取文件内容，传入唯一标识符，返回文件对象
     * tip3: 2. 修改文件内容，传入唯一标识符，文件内容，文件管理系统修改文件内容，调用一遍存储文件内容的函数
     * tip3: 3. 删除文件，传入唯一标识符，文件管理系统删除文件，调用一遍内存删除文件的函数
     * tip3: 4. 创建文件，传入唯一标识符，文件管理系统创建文件，调用一遍内存创建文件的函数
     * 
     */
    // 展示文件属性
    void show();
};

// 文件夹结构体
struct Folder
{
private:
    /*文件夹属性
    类型
    名称
    地址
    大小
    文件数目
    文件夹数目
    创建时间
    修改时间
    下级文件夹
    下级文件
    上级文件夹
    */
    std::string name = "新建文件夹";
    std::string type = "文件夹";
    std::string addr;
    int size = 0;
    int File_number = 0;
    int Folder_number = 0;
    std::string Create_time;
    std::string Change_time;
    std::vector<Folder *> Folder_child;
    std::vector<File *> File_child;
    Folder *dad;

public:
    // 读文件内容函数
    std::string get_Name();
    std::string get_Type();
    std::string get_Addr();
    int get_Size();
    int get_File_number();
    int get_Folder_number();
    std::string get_Create_time();
    std::string get_Change_time();
    std::vector<Folder *> get_Folder_child();
    std::vector<File *> get_File_child();
    Folder *get_Dad();

    // 写文件
    void set_Name(std::string str);
    // 修改文件内容
    void change_size(int n);
    void change_File_number(int n);
    void change_Folder_number(int n);
    void change_Change_time(std::string std);
    void change_Folder_child(bool flag, Folder *folder);
    void change_File_child(bool flag, File *file);
    void change_Dad(Folder *folder);
    void change_Addr(std::string str);

    // 构造函数
    Folder();

    // 拷贝文件夹
    Folder *copy_Folder();

    // 添加文件
    void Add_file(File *file, int flag = 1);

    // 添加文件夹
    void Add_folder(Folder *folder, int flag = 1);

    // 删除文件夹
    void delete_folder();

    // 文件夹序列化
    std::string folder_serialize();

    // 文件夹反序列化
    void folder_deserialize(std::string str);

    // 展示文件夹属性
    void show();
};