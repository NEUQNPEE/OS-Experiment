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

    // 文件内容序列化
    std::string file_serialize();

    // 文件内容序列化
    void file_deserialize(std::string str);

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