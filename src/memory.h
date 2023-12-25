#include <iostream>

using namespace std;

// 模拟内存
char memory[2560];
//内存管理系统存储的fat表
vector<int> fat_list;
//文件信息
char* file_info;
//目录信息
char* dir_info;

//初始化内存及相关操作
void initialMemory();

// 返回文件信息
char *getFileInfo();

// 返回目录信息
char *getDirectoryInfo();

// 用户写文件
void WriteFile(string file_content, char *file_info, char *dir_info);

// 用户读文件
char *ReadFile();
