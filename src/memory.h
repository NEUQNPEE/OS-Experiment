/*
 * @Author       : NieFire planet_class@foxmail.com
 * @Date         : 2024-01-05 03:14:05
 * @LastEditors  : NieFire planet_class@foxmail.com
 * @LastEditTime : 2024-01-11 16:24:25
 * @FilePath     : \OS-Experiment\src\memory.h
 * @Description  : 
 * ( ﾟ∀。)只要加满注释一切都会好起来的( ﾟ∀。)
 * Copyright (c) 2024 by NieFire, All Rights Reserved. 
 */
#include <iostream>
#include <unordered_map>

using namespace std;

// 初始化内存及相关操作
void initialMemory();

// 写入目录信息
void WriteDirectoryInfo(char *info);

// 返回目录信息
char *ReadDirectoryInfo();

// 用户写文件内容。修改文件和保存文件均调用这个接口
void WriteFile(int file_id, string file_content, char *dir_info, int write_process_id);

// 用户读文件内容。
string ReadFile(int file_id, int read_process_id);

// 用户删除文件。请传入删除文件的ID
void DeleteFile(int file_id);

