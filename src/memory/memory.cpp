/*
 * Created Date: Friday January 12th 2024
 * Author: ArmouredEvil
 * Email: dengpersonal123@gmail.com
 * Company: Personal Coder
 * Description: 内存系统相关接口及函数的具体实现
 * Copyright (c) 2024 Personal Coder
 */
#include <iostream>
#include <algorithm>
#include <sstream>
#include "memory_for_file.h"
#include "memory_for_process.h"
#include "MemoryManager.h"
#include "../disk/disk.h"

//using namespace std;

// // 总计64个内存块
// memoryBlock memory_block[64];
// // 64个内存块被进程调度的记录。数值为调用进程的ID，-1则表明没有进程占用
// std::vector<int> process_memory_record;

// // 模拟内存
// char memory[2560];
// // 内存管理系统存储的fat表
// std::vector<int> fat_list;
// // 目录信息
// char *dir_info;
// // 文件ID和文件起始盘块号的映射
// std::unordered_map<int, int> file_id_block_map;

// // 用于装入文件的八个内存块的块号
// int process_memory_now_id[8];
// // 正在装入内存的文件分块后的各文件页。页最多为1024（40KB/40B）
// std::string page_content[1024];

// 初始化DiskManager类
DiskManager disk;
// 初始化memoryManager类
MemoryManager memory_manager;

// 更新页表
void refreshProcess_page_list()
{
    // 获取内存块调度记录和内存块信息
    std::unordered_map<int, std::unordered_map<int, int>>process_page_list = memory_manager.getProcess_page_list();
    memoryBlock *memory_block = memory_manager.getMemory_block();

    // 更新记录
    for (int i = 0; i < 64; i++)
    {
        //未被占用的内存块无需考虑
        if(memory_block[i].process_id < 0)continue;

        //占用当前内存块的进程没有页表,插入新页表
        if(process_page_list.count(memory_block[i].process_id) == 0)
        {
            std::unordered_map<int, int>temp;
            temp.insert({memory_block[i].page_id, memory_block[i].block_id});
            process_page_list.insert({memory_block[i].process_id, temp});
        }
        //占用当前内存块的进程有页表
        else
        {
            //如果页表中没有当前内存块所存储的页的表项，则更新
            if(process_page_list[memory_block[i].process_id].count(memory_block[i].page_id) == 0)
            {
                process_page_list[memory_block[i].process_id].insert({memory_block[i].page_id, memory_block[i].block_id});
            }
        }
    }

    // 写入新记录
    memory_manager.setProcess_page_list(process_page_list);
}

// // 序列化文件ID和起始盘块号的映射
// std::string encodeMap()
// {
//     std::string ans = "##";
//     for (auto iter : file_id_block_map)
//     {
//         ans += to_std::string(iter.first) + " " + to_std::string(iter.second) + " ";
//     }
//     ans = ans + "##";
//     return ans;
// }

// // 解析序列化后的文件ID和起始盘块号的映射
// void decodeMap(std::string info)
// {
//     // 暂存解析出的数字
//     std::vector<int> temp;
//     // 如果是空映射
//     if (info.size() < 8) // ##1 2 ##
//         return;
//     // 去掉起始和末尾的分隔符##
//     int map_begin = info.find("##") + 2;
//     int map_end = info.rfind("##") - 1;
//     int len = map_end - map_begin + 1;
//     info = info.substr(map_begin, len);

//     // 解析文件ID和起始盘块号的映射
//     std::stringstream str(info);
//     std::string number;
//     while (str >> number)
//     {
//         temp.push_back(stoi(number));
//     }
//     int temp_size = temp.size();
//     for (int i = 0; i < temp_size - 1; i += 2)
//     {
//         file_id_block_map[temp[i]] = temp[i + 1];
//     }
// }

// // 写入目录信息和序列化后的ID和起始盘块号映射
// void WriteDirectoryInfo(char *info)
// {
//     // 将info写入磁盘
//     disk.save_dir_info(info);
// }
// // 读取目录信息和序列化后的ID和起始盘块号映射
// char *ReadDirectoryInfo()
// {
//     // 获取目录信息的块号
//     int block_number = disk.get_dir_info_block_number();
//     // return disk.read_block(block_number);
//     // 读取目录信息块的FAT
//     std::std::vector<int> block_numbers = disk.read_fat(block_number);

//     // todo 进行数据检查，看磁盘返回来的数据对不对
//     // tip 检查内容是否是合法字符
//     // tip 检查内容是否长度合适

//     // 返回读取的块
//     return disk.read_blocks(block_numbers);
// }

//写入目录信息
void WriteDirectoryInfo(char *info)
{
    // 将info写入磁盘
    disk.save_dir_info(info);
    //更新内存系统存储的目录信息
    memory_manager.setDir_info(info);
}

// 返回目录信息
char *ReadDirectoryInfo()
{
    return memory_manager.getDir_info();
}


// 初始化内存块
void initialMemoryBlock()
{
    // 获取内存块的信息
    memoryBlock *memory_block = memory_manager.getMemory_block();
    // 初始化64个内存块
    for (int i = 0; i < 64; i++)
    {
        memory_block[i].block_id = i;
        memory_block[i].begin = i * 40;
    }
    // 写入内存块信息
    memory_manager.setMemory_block(memory_block);

    // 初始化空闲内存块数量
    int free_block_sum = 64;
    memory_manager.setFree_block_sum(free_block_sum);

    // 初始化记录当前进程所占内存块块号的数组
    int *process_memory_now_id;
    std::fill(process_memory_now_id, process_memory_now_id + 64, -1);
    memory_manager.setProcess_memory_now_id(process_memory_now_id);
}

// 初始化内存及相关操作
void initialMemory()
{
    // 初始化内存
    char *memory = memory_manager.getMemory();
    std::fill(memory, memory + 2560, ' ');
    memory_manager.setMemory(memory);

    // 初始化内存块
    initialMemoryBlock();

    // 初始化磁盘
    disk.init_disk();

    // 初始化FAT表
    std::vector<int> fat_list = memory_manager.getFat_list();
    fat_list = disk.get_fat_block_numbers();
    memory_manager.setFat_list(fat_list);

    // 初始化目录信息和文件ID与起始盘块号映射
    char *dir_info = memory_manager.getDir_info();
    dir_info = ReadDirectoryInfo();
    memory_manager.setDir_info(dir_info);
}

// 为进程分配内存块
std::vector<int> assignMemoryBlock(int write_process_id, int assign_block_sum)
{
    std::vector<int> temp;
    // 初始化FAT表，以防FAT表有更新
    std::vector<int> fat_list;
    fat_list = disk.get_fat_block_numbers();
    memory_manager.setFat_list(fat_list);
    // 获取内存块信息
    memoryBlock *memory_block = memory_manager.getMemory_block();
    //获取进程和内存块的映射
    std::unordered_map<int, std::vector<int> > process_memory_block_list = memory_manager.getProcess_memory_block_list();
    // 获取空闲内存块数量
    int free_block_sum = memory_manager.getFree_block_sum();

    //当前进程已申请内存块，否决
    if(process_memory_block_list.count(write_process_id) == 0)
    {
        std::cout<<"进程ID:"<<write_process_id<<" 重复申请内存,错误!"<<std::endl;
        temp.resize(assign_block_sum, -1);
        return temp;
    }
    
    // 申请内存块数量多于空闲内存块数量，否决
    if (assign_block_sum > free_block_sum)
    {
        std::cout<<"内存不足!"<<std::endl;
        temp.resize(assign_block_sum, -1);
        return temp;
    }

    // 遍历所有内存块,分配内存块
    for (int i = 0; i < 64 && assign_block_sum > 0; i++)
    {
        if (memory_block[i].process_id == -1)
        {
            memory_block[i].process_id = write_process_id;
            assign_block_sum--;
            free_block_sum--;
            temp.push_back(i);
        }
    }

    // 更新内存块信息
    memory_manager.setMemory_block(memory_block);
    //更新进程和内存块的映射
    process_memory_block_list.insert({write_process_id, temp});
    memory_manager.setProcess_memory_block_list(process_memory_block_list);
    // 更新空闲内存块数量
    memory_manager.setFree_block_sum(free_block_sum);

    return temp;
}

// 释放进程占用的内存块
void clearMemoryBlock(int clear_process_id)
{
    // 获取内存块信息
    memoryBlock *memory_block = memory_manager.getMemory_block();
    //获取进程和内存块的映射
    std::unordered_map<int, std::vector<int> > process_memory_block_list = memory_manager.getProcess_memory_block_list();
    // 获取空闲内存块数量
    int free_block_sum = memory_manager.getFree_block_sum();

    // 释放内存块
    for (int i = 0; i < 64; i++)
    {
        if (memory_block[i].process_id == clear_process_id)
        {
            memory_block[i].page_id = -1;
            memory_block[i].status = 0;
            memory_block[i].process_id = -1;
            free_block_sum--;
        }
    }
    // 更新内存块信息
    memory_manager.setMemory_block(memory_block);
    //更新进程和内存块的映射
    process_memory_block_list.erase(clear_process_id);
    memory_manager.setProcess_memory_block_list(process_memory_block_list);
    // 更新空闲内存块数量
    memory_manager.setFree_block_sum(free_block_sum);
    // 更新页表
    refreshProcess_page_list();
}

// 将文件页的内容填充到内存中。
void fillMemory(int page_id, int block_id)
{
    // 获取当前文件的内容
    std::string *page_content = memory_manager.getPage_content();
    //获取内存
    char* memory = memory_manager.getMemory();
    // 获取内存块信息
    memoryBlock *memory_block = memory_manager.getMemory_block();

    int page_size = page_content[page_id].size();
    //检查数据是否合法
    if (page_size > 40)
    {
        std::cout<<"分页错误！"<<std::endl;
        return;
    }
    //填充
    int i = 0;  //遍历填充的内容
    int j = memory_block[block_id].begin;   //遍历目标内存
    for (; i < page_size; i++, j++)
    {
        memory[j] = page_content[page_id][i];
    }

    //更新内存
    memory_manager.setMemory(memory);
}

// 调用disk.cpp的函数，将修改后的文件内容传进磁盘，返回文件的起始磁盘块号
int SaveDiskFile(std::string block_content)
{
    //将文件内容存入磁盘
    char *content = new char[block_content.size()];
    strcpy(content, block_content.c_str());
    int disk_block_id = disk.save_file(content);
    // 存储文件后磁盘会更新FAT表，所以更新fat_list
    std::vector<int> fat_list = disk.get_fat_block_numbers();
    memory_manager.setFat_list(fat_list);

    // tip 先修复现有功能，然后这里要把fat表搬入内存，提供更快的交互

    
    return disk_block_id;
}

// 全局置换CLOCK算法，返回调度后装入的内存块块号
// tip 如果按照函数式编程的方式，应该传入所有内存块的状况（可能是内存块结构体数组），以及需要置换的新内存块信息（也可能是内存块结构体数组）
int CLOCK(int page)
{
    // 获取内存块信息
    memoryBlock *memory_block = memory_manager.getMemory_block();
    //获取当前进程所占据的内存块块号
    int* process_memory_now_id = memory_manager.getProcess_memory_now_id();
    //当前进程占据的内存块数量
    int block_sum = sizeof(process_memory_now_id);
    // 装入的内存块的块号
    int flag = 0;

    // 查找页是否已在内存里
    bool flag1 = false;
    for (int j = 0; j < block_sum; j++)
    {
        if (memory_block[process_memory_now_id[j]].page_id == page)
        {
            memory_block[process_memory_now_id[j]].status = 1;
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
    for (int j = 0; j < block_sum; j++)
    {
        if (memory_block[process_memory_now_id[j]].page_id == -1)
        {
            memory_block[process_memory_now_id[j]].page_id = page;
            memory_block[process_memory_now_id[j]].status = 1;
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
    for (int j = 0; j < block_sum; j++)
    {
        if (memory_block[process_memory_now_id[j]].status == 0)
        {
            memory_block[process_memory_now_id[j]].page_id = page;
            memory_block[process_memory_now_id[j]].status = 1;
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
    for (int j = 0; j < block_sum; j++)
    {
        memory_block[process_memory_now_id[j]].status = 0;
    }
    memory_block[process_memory_now_id[0]].page_id = page;

    //更新内存块信息
    memory_manager.setMemory_block(memory_block);
    //更新页表
    refreshProcess_page_list();

    return 0;
}

// 用户写文件内容
void WriteFile(int file_id, std::string file_content, char *write_dir_info, int write_process_id)
{
    // 获取内存块信息
    memoryBlock *memory_block = memory_manager.getMemory_block();
    //获取进程和内存块的映射
    std::unordered_map<int, std::vector<int> > process_memory_block_list = memory_manager.getProcess_memory_block_list();
    //获取文件和磁盘块映射
    std::unordered_map<int, int> file_disk_block_map = memory_manager.getFile_disk_block_map();
    //获取当前进程所占据的内存块块号
    int* process_memory_now_id = memory_manager.getProcess_memory_now_id();
    //获取暂存当前文件的数组
    std::string* page_content = memory_manager.getPage_content();
    
    
    // tip 有个类似于fat表的数据结构：进程id - 内存块号，应该以查找方式，而不是遍历

    //判断write_process_id是否合法
    if(process_memory_block_list.count(write_process_id) == 0)
    {
        std::cout<<"写文件操作所属的进程ID不存在!"<<std::endl;
        return;
    }
    // 通过映射找到哪些内存块负责该进程
    std::vector<int> temp = process_memory_block_list[write_process_id];
    int block_sum = temp.size();
    for(int i = 0; i < block_sum; i++)
    {
        process_memory_now_id[i] = temp[i];
    }

    //  文件分页并装入内存
    int file_size = file_content.size(); // 文件长度
    int page_count;
    if (file_content.size() == 0)
    {
        page_count = 0;
    }
    else
    {
        page_count = file_size / 40; // 文件分块后的页数
    }
    int write_block_id = -1; // 写入的内存块的块号
    for (int i = 0; i < page_count; i++)
    {
        page_content[i] = file_content.substr(i * 40, 40);
        write_block_id = CLOCK(i);
        fillMemory(i, write_block_id);
    }

    // 查找是否有不足40B的尾巴
    int remainder = file_size % 40;
    if (remainder != 0)
    {
        page_content[page_count] = file_content.substr(page_count * 40, remainder);
        write_block_id = CLOCK(page_count);
        fillMemory(page_count, write_block_id);
    }

    // 内存中的文件内容写入磁盘，图省事直接用file_content
    int disk_block_id = SaveDiskFile(file_content);
    // 更新文件id和磁盘块映射
    if (file_disk_block_map.count(file_id) == 1) // 如果映射中已存在该文件，则修改
    {
        file_disk_block_map[file_id] = disk_block_id;
    }
    else // 如果映射中不存在该文件，则添加
    {
        file_disk_block_map.insert(std::pair<int, int>(file_id, disk_block_id));
    }

    // 更新目录信息
    WriteDirectoryInfo(write_dir_info);
    // 清空文件页内容暂存
    std::fill(page_content, page_content + 1024, "NULL");
    memory_manager.setPage_content(page_content);
    //更新文件和磁盘块的映射
    memory_manager.setFile_disk_block_map(file_disk_block_map);
    //更新当前操作文件所属进程占用的内存块
    memory_manager.setProcess_memory_now_id(process_memory_now_id);
}

// 从磁盘读取文件内容
char *ReadDiskFile(int block_id)
{
    //读取并更新FAT表
    std::vector<int> fat_list = memory_manager.getFat_list();
    fat_list = disk.read_fat(block_id);
    memory_manager.setFat_list(fat_list);
    return disk.read_blocks(fat_list);
}

// 读取内存块内容
char *ReadMemoryBlock(int memory_block_id, int size)
{
    // 获取内存块信息
    memoryBlock *memory_block = memory_manager.getMemory_block();
    //获取内存
    char* memory = memory_manager.getMemory();

    // 读取内存块内容
    char *content;
    int i = memory_block[memory_block_id].begin;
    for (int j = 0; j < size; j++)
    {
        content[j] = memory[i + j];
    }
    return content;
}

// 文件管理系统调用，读文件。
std::string ReadFile(int file_id, int read_process_id)
{
    // 获取内存块信息
    memoryBlock *memory_block = memory_manager.getMemory_block();
    //获取进程和内存块的映射
    std::unordered_map<int, std::vector<int> > process_memory_block_list = memory_manager.getProcess_memory_block_list();
    //获取文件和磁盘块映射
    std::unordered_map<int, int> file_disk_block_map = memory_manager.getFile_disk_block_map();
    //获取当前进程所占据的内存块块号
    int* process_memory_now_id = memory_manager.getProcess_memory_now_id();
    //获取暂存当前文件的数组
    std::string* page_content = memory_manager.getPage_content();
    
    //判断read_process_id是否合法
    if(process_memory_block_list.count(read_process_id) == 0)
    {
        std::cout<<"写文件操作所属的进程ID不存在!"<<std::endl;
        return "";
    }
    // 通过映射找到哪些内存块负责该进程
    std::vector<int> temp = process_memory_block_list[read_process_id];
    int block_sum = temp.size();
    for(int i = 0; i < block_sum; i++)
    {
        process_memory_now_id[i] = temp[i];
    }

    // 查找文件id对应的磁盘块id
    int block_id = -1;
    if (file_disk_block_map.count(file_id) == 1)
    {
        block_id = file_disk_block_map[file_id];
    }
    else
    {
        std::cout << "File not found!" << std::endl;
        return "";
    }

    // 从磁盘读出文件内容
    std::string file_content = ReadDiskFile(block_id);

    // tip 在修复了现有功能之后，要真的把内存动用起来

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
        refreshProcess_page_list();
    }
    // 查找是否有不足40B的尾巴
    int remainder = file_size % 40;
    if (remainder != 0)
    {
        page_content[page_count] = file_content.substr(page_count * 40, remainder);
        write_block_id = CLOCK(page_count);
        fillMemory(page_count, write_block_id);
        refreshProcess_page_list();
    }

    // 清空文件页内容暂存
    std::fill(page_content, page_content + 1024, "NULL");
    memory_manager.setPage_content(page_content);
    //更新当前操作文件所属进程占用的内存块
    memory_manager.setProcess_memory_now_id(process_memory_now_id);

    // return ans;
    return file_content;
}

// 用户删除文件
void DeleteFile(int file_id)
{
    //获取文件和磁盘块映射
    std::unordered_map<int, int> file_disk_block_map = memory_manager.getFile_disk_block_map();
    
    // 查找文件id对应的磁盘块id
    int block_id = -1;
    if (file_disk_block_map.count(file_id) == 1)
    {
        block_id = file_disk_block_map[file_id];
    }
    else
    {
        std::cout << "File not found!" << std::endl;
        return;
    }

    //磁盘删文件
    disk.delete_file_info(block_id);

    //更新文件和磁盘块映射
    file_disk_block_map.erase(file_id);
    memory_manager.setFile_disk_block_map(file_disk_block_map);
}

// 返回当前进程对内存块的调度状况
std::vector<int> getProcessRecord()
{
    std::vector<int> ans;
    //获取内存块信息
    memoryBlock* memory_block = memory_manager.getMemory_block();
    //获取内存块调度状况
    int block_sum = sizeof(memory_block);
    for(int i = 0; i < block_sum; i++)
    {
        ans.push_back(memory_block[i].process_id);
    }
    return ans;
}

// 向上传递磁盘提供给QT的磁盘块占用情况
std::vector<bool> memory_get_disk_block_status()
{
    return disk.get_disk_block_status();
}

// 向上传递磁盘提供给QT的成组链块的情况
std::vector<int> memory_get_group_block_status()
{
    return disk.get_group_block_status();
}
