#include <iostream>
#include <unordered_map>

using namespace std;

// 模拟内存
char memory[2560];
// 内存管理系统存储的fat表
vector<int> fat_list;
// 目录信息
char *dir_info;
// 文件ID和文件起始盘块号的映射
unordered_map<string, int> file_id_block_map;

// 初始化内存及相关操作
void initialMemory();

// 写入目录信息
void WriteDirectoryInfo(char *info);

// 返回目录信息
char *ReadDirectoryInfo();

// 用户写文件内容。修改文件和保存文件均调用这个接口
void WriteFile(string file_id, string file_content, char *dir_info);

// 用户读文件内容。此接口会把存在于FAT表中的所有文件读取
string ReadFile(string file_id);

// 用户删除文件。请传入删除文件的ID
void DeleteFile(string file_id);
