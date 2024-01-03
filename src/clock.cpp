#include <iostream>
#include <algorithm>
#include "memory.h"
#include "memoryBlock.h"
#include "disk.h"
#include "process_manager.h"

using namespace std;

// 总计64个内存块
memoryBlock memory_block[64];
//64个内存块被进程调度的记录。数值为调用进程的ID，-1则表明没有进程占用
vector<int> process_memory_record;

// 模拟内存
char memory[2560];
// 内存管理系统存储的fat表
vector<int> fat_list;
// 目录信息
char *dir_info;
// 文件ID和文件起始盘块号的映射
unordered_map<int, int> file_id_block_map;

// 用于装入文件的八个内存块的块号
int block_ids[8];
// 正在装入内存的文件分块后的各文件页。页最多为1024（40KB/40B）
string page_content[1024];
// 初始化DiskManager类。如果main函数已初始化，此条需注释
DiskManager disk;

// 写入目录信息
void WriteDirectoryInfo(char *info)
{
    disk.save_dir_info(info);
}

// 返回目录信息
char *ReadDirectoryInfo()
{
    int block_number = disk.get_dir_info_block_number();
    return disk.read_block(block_number);
}

// 初始化内存块
void initialMemoryBlock()
{
    //初始化64个内存块
    for (int i = 0; i < 64; i++)
    {
        memory_block[i].block_id = i;
        memory_block[i].begin = i * 40;
    }
    //初始化64个内存块被进程调度的记录
    process_memory_record.resize(64, -1);
}

// 初始化内存及相关操作
void initialMemory()
{
    // 初始化内存
    fill(memory, memory + 2560, ' ');
    // 初始化内存块
    initialMemoryBlock();
    // 初始化记录当前进程所占八个内存块块号的数组
    fill(block_ids, block_ids + 8, -1);
    // 初始化磁盘
    disk.init_disk();
    // 初始化FAT表
    fat_list = disk.get_fat_block_numbers();
    // 初始化目录信息
    dir_info = "";
}

// 为进程分配八个内存块
vector<int> initialBlock_ids(int write_process_id)
{
    vector<int> temp;
    // 初始化FAT表，以防FAT表有更新
    fat_list = disk.get_fat_block_numbers();
    // 外遍历：确定装文件的内存块。内遍历：搜索未装页的内存块
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j <= 64; j++)
        {
            if (j == 64)
            {
                cout << "警告!已无空闲内存块!内存溢出!";
                fill(block_ids, block_ids + 8, -1);
                generate(block_ids, block_ids + 8, temp);
                return temp;
            }
            if (memory_block[j].page_id < 0)
            {
                block_ids[i] = memory_block[j].block_id;
                break;
            }
        }
    }
    // 八个内存块记录当前进程的ID
    for (int i = 0; i < 8; i++)
    {
        memory_block[block_ids[i]].process_id = write_process_id;
    }

    generate(block_ids, block_ids + 8, temp);
    return temp;
}

// 释放进程占用的八个内存块
void clearBlock_ids(int clear_process_id)
{
    //当前进程是否为释放内存块的进程
    for (int i = 0; i < 8; i++)
    {
        if (memory_block[block_ids[i]].process_id == clear_process_id)
        {
            memory_block[block_ids[i]].page_id = -1;
            memory_block[block_ids[i]].status = 0;
            memory_block[block_ids[i]].process_id = -1;
            block_ids[i] = -1;
        }
    }
    //如果当前进程不是释放内存块的进程，则从所有内存块中寻找并释放
    for(int i = 0; i < 64; i++)
    {
        if(memory_block[i].process_id == clear_process_id)
        {
            memory_block[i].page_id = -1;
            memory_block[i].status = 0;
            memory_block[i].process_id = -1;
        }
    }

    // 顺带把暂存页内容的page_content清空
    fill(page_content, page_content + 1024, "");
    // 顺带把当前进程调度内存块的状况清空
    // clock_record.clear();
}

// 记录内存块调度状况
void recordProcess_memory()
{
    for(int i = 0; i < 64; i++)
    {
        process_memory_record[i] = memory_block[i].process_id;
    }
}

// 将文件页的内容填充到内存中。
void fillMemory(int page_id, int block_id)
{
    int page_size = page_content[page_id].size();
    int i = 0;
    int j = memory_block[block_id].begin;
    for (; i < page_size; i++, j++)
    {
        memory[j] = page_content[page_id][i];
    }
}

// 调用disk.cpp的函数，将修改后的文件内容传进磁盘，返回文件的起始磁盘块号
int SaveDiskFile(string block_content)
{
    char *content = block_content.data();
    int disk_block_id = disk.save_file(content);
    // 存储文件后磁盘会更新FAT表，所以更新fat_list
    fat_list = disk.get_fat_block_numbers();
    return disk_block_id;
}

// 全局置换CLOCK算法，返回调度后装入的内存块块号
int CLOCK(int page)
{
    // 装入的内存块的块号
    int flag = 0;

    // 查找页是否已在内存里
    bool flag1 = false;
    for (int j = 0; j < 8; j++)
    {
        if (memory_block[block_ids[j]].page_id == page)
        {
            memory_block[block_ids[j]].status = 1;
            flag = j;
            flag1 = true;
            break;
        }
    }
    if (flag1 == true)
    {
        return flag;
    }

    // 查找是否有没存页的内存块
    bool flag2 = false;
    for (int j = 0; j < 8; j++)
    {
        if (memory_block[block_ids[j]].page_id == -1)
        {
            memory_block[block_ids[j]].page_id = page;
            memory_block[block_ids[j]].status = 1;
            flag = j;
            flag2 = true;
            break;
        }
    }
    if (flag2 == true)
    {
        return flag;
    }

    // 查找是否有访存位为0的内存块
    bool flag3 = false;
    for (int j = 0; j < 8; j++)
    {
        if (memory_block[block_ids[j]].status == 0)
        {
            memory_block[block_ids[j]].page_id = page;
            memory_block[block_ids[j]].status = 1;
            flag = j;
            flag3 = true;
            break;
        }
    }
    if (flag3 == true)
    {
        return flag;
    }

    // 页面置换
    for (int j = 0; j < 8; j++)
    {
        memory_block[block_ids[j]].status = 0;
        memory_block[block_ids[j]].file_all_in_memory = false; // 发生了页面置换，则文件的所有页不可能同时在内存里
    }
    memory_block[block_ids[0]].page_id = page;
    return 0;
}

// 用户写文件内容
void WriteFile(int file_id, string file_content, char *write_dir_info, int write_process_id)
{
    // 搜索由哪八个内存块负责该进程
    int j = 0;  //遍历block_ids
    for(int i = 0; i < 64; i++) //遍历memory_block
    {
        if(memory_block[i].process_id == write_process_id)
        {
            block_ids[j] = memory_block[i].block_id;
            j++;
        }
        if(j == 8)break;
    }

    string ans = "";
    // 文件分页并装入内存
    int file_size = file_content.size(); // 文件长度
    int page_count = file_size / 40;     // 文件分块后的页数
    int write_block_id = -1;             // 写入的内存块的块号
    for (int i = 0; i < page_count; i++)
    {
        page_content[i] = file_content.substr(i * 40, 40);
        write_block_id = CLOCK(i);
        fillMemory(i, write_block_id);
        recordProcess_memory();
    }

    // 查找是否有不足40B的尾巴
    int remainder = file_size % 40;
    if (remainder != 0)
    {
        page_content[page_count] = file_content.substr(page_count * 40, remainder);
        write_block_id = CLOCK(page_count);
        fillMemory(page_count, write_block_id);
        recordProcess_memory();
    }

    // 内存中的文件内容写入磁盘，图省事直接用file_content
    int disk_block_id = SaveDiskFile(file_content);
    // 更新文件id和磁盘块映射
    if (file_id_block_map.count(file_id) == 1) // 如果映射中已存在该文件，则修改
    {
        file_id_block_map[file_id] = disk_block_id;
    }
    else // 如果映射中不存在该文件，则添加
    {
        file_id_block_map.insert(pair<int, int>(file_id, disk_block_id));
    }
    // 更新目录信息
    disk.save_dir_info(write_dir_info);
    // 释放内存
    //clearBlock_ids();
}

// 从磁盘读取文件内容
char *ReadDiskFile(int block_id)
{
    return disk.read_block(block_id);
}

// 读取内存块中的函数
char *ReadMemoryBlock(int memory_block_id, int size)
{
    char *content;
    int i = memory_block[memory_block_id].begin;
    for (int j = 0; j < size; j++)
    {
        content[j] = memory[i + j];
    }
    return content;
}

// 文件管理系统调用，读文件。
string ReadFile(int file_id, int read_process_id)
{
    // 搜索由哪八个内存块负责该进程
    int j = 0;  //遍历block_ids
    for(int i = 0; i < 64; i++) //遍历memory_block
    {
        if(memory_block[i].process_id == read_process_id)
        {
            block_ids[j] = memory_block[i].block_id;
            j++;
        }
        if(j == 8)break;
    }

    // 查找文件id对应的磁盘块id
    int block_id = -1;
    if (file_id_block_map.count(file_id) == 1)
    {
        block_id = file_id_block_map[file_id];
    }
    else
    {
        cout << "File not found!" << endl;
        return "";
    }

    // 从磁盘读出文件内容
    string file_content = ReadDiskFile(block_id);

    // 其实接下来直接return file_content即可。为了符合指导书的要求，这里仍先装内存，再从内存读出
    char *ans;
    // 文件分页并装入内存
    int file_size = file_content.size(); // 文件长度
    int page_count = file_size / 40;     // 文件分块后的页数
    int write_block_id = -1;             // 写入的内存块的块号
    for (int i = 0; i < page_count; i++)
    {
        page_content[i] = file_content.substr(i * 40, 40);
        write_block_id = CLOCK(i);
        fillMemory(i, write_block_id);
        recordProcess_memory();
        char *temp = ans + i * 40;
        temp = ReadMemoryBlock(write_block_id, 40);
    }
    // 查找是否有不足40B的尾巴
    int remainder = file_size % 40;
    if (remainder != 0)
    {
        page_content[page_count] = file_content.substr(page_count * 40, remainder);
        write_block_id = CLOCK(page_count);
        fillMemory(page_count, write_block_id);
        recordProcess_memory();
        char *temp = ans + page_count * 40;
        temp = ReadMemoryBlock(write_block_id, remainder);
    }

    // 释放内存
    //clearBlock_ids();

    // return ans;
    return file_content;
}

// 用户删除文件
void DeleteFile(int file_id)
{
    // 查找文件id对应的磁盘块id
    int block_id = -1;
    if (file_id_block_map.count(file_id) == 1)
    {
        block_id = file_id_block_map[file_id];
    }
    else
    {
        cout << "File not found!" << endl;
        return;
    }
    disk.delete_file_info(block_id);
}

//返回当前进程对内存块的调度状况
vector<int> getProcessRecord()
{
    return process_memory_record;
}

// 向上传递磁盘提供给QT的磁盘块占用情况
vector<bool> memory_get_disk_block_status()
{
    return disk.get_disk_block_status();
}

// 向上传递磁盘提供给QT的成组链块的情况
vector<int> memory_get_group_block_status()
{
    return disk.get_group_block_status();
}

