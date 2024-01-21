#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include <queue>
#include <sstream>
#include <map>

#include "memory_for_file.h"
// 得到当前时间的函数
std::string gettime();

// 将字符串转化为char数组
char *gotoChar(std::string str);

// 将char数组转化为字符串
std::string gotoString(char *str);

// 文件夹结构体声明
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
    int ID;
    std::string name;
    std::string addr;
    int size = 0;
    std::string Create_time;
    std::string Change_time;
    Folder *dad;

public:
    // 读文件属性函数
    int get_ID();
    std::string get_Name();
    std::string get_Addr();
    int get_Size();
    std::string get_Create_time();
    std::string get_Change_time();
    Folder *get_Dad();

    // 写文件属性函数
    void set_Name_Type(std::string str);
    void set_ID();
    void set_Content(std::string str);

    // 文件属性改变
    void change_Dad(Folder *folder);
    void change_Addr(std::string str);

    // 构造函数
    File(std::string str);

    // 删除文件
    void delete_File();

    // 文件属性序列化
    std::string file_serialize();

    // 文件属性反序列化
    void file_deserialize(std::string str);

    // 展示文件属性
    std::vector<std::string> show();
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
    std::string name;
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

    // 设置文件名称
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
    Folder(std::string str);

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
    std::vector<std::string> show();
};
// 树形目录序列化生成
void tree_dir_ser_gen(std::string *str);

// 树形目录序列化
std::string tree_dir_ser();

// 树形目录反序列化生成
void tree_dir_diser_gen(std::queue<int> type, std::queue<int> child_num, std::queue<std::string> inform);

// 树形目录反序列化
void tree_dir_diser(std::string str);

// 初始化函数
Folder *init_root();

// 判断文件夹是否重名
bool folder_is_repeat(Folder *folder, std::string str);

// 判断文件是否重名接口
bool file_is_repeat(Folder *folder, std::string str);

// 添加文件接口
File *folder_add_file(Folder *folder, std::string str);

// 添加文件夹接口
Folder *folder_add_folder(Folder *folder, std::string str);

// 删除文件夹
void delete_folder(Folder *folder);

// 删除文件
void delete_file(File *file);

// 改文件夹名接口
Folder *folder_change_name(Folder *folder, std::string name);

// 改文件名
File *file_change_name(File *file, std::string name);

// 改文件内容
File *file_change_content(File *file, std::string content, int p_id);

// 获得文件夹的子文件夹
std::vector<Folder *> get_folder_child(Folder *folder);

// 获得文件夹的子文件
std::vector<File *> get_file_child(Folder *folder);

// 打开文件
std::string look_file_content(File *file, int p_id);

// 关闭文件
void close_file(File *file, int p_id);

// 查找打开文件的进程
std::vector<int> look_open_file(File *file);

// 查看文件属性
std::vector<std::string> look_file(File *file);

// 查看文件夹属性
std::vector<std::string> look_folder(Folder *folder);
