/*
 * Created Date: Friday January 12th 2024
 * Author: ArmouredEvil
 * Email: dengpersonal123@gmail.com
 * Company: Personal Coder
 * Description: 封装内存系统所需的变量
 * Copyright (c) 2024 Personal Coder
 */
#include <iostream>
#include <vector>
#include <unordered_map>

/**
 * 内存块结构体
 */
typedef struct memoryBlock
{
    // 内存块号
    int block_id = -1;
    // 内存块在内存中的起始地址
    int begin = -1;
    // 内存块大小。默认40B
    int block_size = 40;
    // 访问位
    int status = 0;
    // 存储页号
    int page_id = -1;
    // 所属的进程的进程ID
    int process_id = -1;
} memoryBlock;

/**
 * 内存系统调用的所有变量封装到memoryManger类中
 */
class MemoryManager
{
private:
    // 开辟2560B的内存空间
    char *memory = new char[2560];

    // 64个内存块
    memoryBlock *memory_block = new memoryBlock[64];

    // 页表
    //外层是进程ID和表的映射，内层是该进程的页表
    std::unordered_map<int, unordered_map<int, int>> process_page_list;

    //进程和所占用内存块的映射
    std::unordered_map<int, vector<int> > process_memory_block_list;

    // 正在调度的进程所占用的内存块的ID。至多占用64块
    int *process_memory_now_id = new int[64];

    //空闲内存块数量
    int free_block_sum;

    // 正在装入内存的文件分块后的各文件页内容。页数量最多为1024（40KB/40B）
    std::string *page_content = new std::string[1024];

    // 内存管理系统存储的fat表
    std::vector<int> fat_list;

    // 目录信息
    char *dir_info;

    // 文件ID和文件起始盘块号的映射
    std::unordered_map<int, int> file_disk_block_map;

public:
    // 获取内存
    char *getMemory()
    {
        return memory;
    }
    // 写入内存
    void setMemory(char *content)
    {
        memory = content;
    }

    // 获取内存块
    memoryBlock *getMemory_block()
    {
        return memory_block;
    }

    // 写入内存块
    void setMemory_block(memoryBlock *content)
    {
        memory_block = content;
    }

    // 获取页表
    std::unordered_map<int, std::unordered_map<int, int>> getProcess_page_list()
    {
        return process_page_list;
    }

    // 写入页表
    void setProcess_page_list(std::unordered_map<int, std::unordered_map<int, int>> content)
    {
        process_page_list = content;
    }

    //获取进程和内存块的映射
    std::unordered_map<int, std::vector<int> > getProcess_memory_block_list()
    {
        return process_memory_block_list;
    }

    //写入进程和内存块的映射
    void setProcess_memory_block_list(unordered_map<int, vector<int> > content)
    {
        process_memory_block_list = content;
    }

    // 获取正在进行调度的进程占用的内存块
    int *getProcess_memory_now_id()
    {
        return process_memory_now_id;
    }

    // 写入正在进行调度的进程占用的内存块
    void setProcess_memory_now_id(int *content)
    {
        process_memory_now_id = content;
    }

    //获取空闲内存块数量
    int getFree_block_sum()
    {
        return free_block_sum;
    }

    //写入空闲内存块数量
    void setFree_block_sum(int content)
    {
        free_block_sum = content;
    }

    // 获取正在装入内存的文件的各文件页内容
    std::string *getPage_content()
    {
        return page_content;
    }

    // 写入正在装入内存的文件的各文件页内容
    void setPage_content(std::string *content)
    {
        page_content = content;
    }

    // 获取文件分配表
    vector<int> getFat_list()
    {
        return fat_list;
    }

    // 写入文件分配表
    void setFat_list(std::vector<int> content)
    {
        fat_list = content;
    }

    // 获取目录信息
    char *getDir_info()
    {
        return dir_info;
    }

    // 写入目录信息
    void setDir_info(char *content)
    {
        dir_info = content;
    }

    //获取文件和磁盘块映射
    std::unordered_map<int, int> getFile_disk_block_map()
    {
        return file_disk_block_map;
    }

    //写入文件和磁盘块映
    void setFile_disk_block_map(unordered_map<int, int> content)
    {
        file_disk_block_map = content;
    }

};