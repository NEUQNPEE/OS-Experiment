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

//写入文件信息
void writeFileInfo(char *info);

//写入目录信息
void writeDirectoryInfo(char *info);

// 返回文件信息
char *getFileInfo();

// 返回目录信息
char *getDirectoryInfo();

// 用户写文件。修改文件和保存文件均调用这个接口
void WriteFile(string file_content, char *file_info, char *dir_info);

// 用户读文件。此接口会把存在于FAT表中的所有文件读取
char *ReadFile();

//用户删除文件。请传入删除文件的盘块号
void DeleteFile(int file_block_number);
