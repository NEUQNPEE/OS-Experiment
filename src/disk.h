/*
 * @Author       : NieFire planet_class@foxmail.com
 * @Date         : 2023-12-22 19:05:52
 * @LastEditors  : NieFire planet_class@foxmail.com
 * @LastEditTime : 2023-12-24 20:30:55
 * @FilePath     : \OS-Experiment\src\disk.h
 * @Description  : 
 * ( ﾟ∀。)只要加满注释一切都会好起来的( ﾟ∀。)
 * Copyright (c) 2023 by NieFire, All Rights Reserved. 
 */
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <cstring>
#include <intrin.h>
#include <algorithm>
#include <map>
#include <queue>

// 磁盘块状态枚举
enum DiskBlockStatus
{
    // 空闲
    FREE,
    // 已分配
    ALLOCATED,
    // 已占用
    OCCUPIED
};

// 重载输出运算符以输出磁盘块状态
std::ostream &operator<<(std::ostream &os, const DiskBlockStatus &status);

/**
 * 磁盘块
 */
struct DiskBlock
{
private:
    // 磁盘块大小
    int size;

    // 磁盘块号
    int number;

    // 磁盘块状态
    DiskBlockStatus status;

    // 磁盘块内容
    char *content;

public:
    // 构造函数
    DiskBlock(int size, int number);

    // 设置磁盘块内容
    void set_content(char *content);

    // 获取磁盘块内容
    char *get_content() const;

    // 设置磁盘块状态
    void set_status(DiskBlockStatus status);

    // 获取磁盘块状态
    DiskBlockStatus get_status() const;
    
    // 获取磁盘块号
    int get_number() const;
};

/**
 * 磁盘结构体
 */
struct Disk
{
private:
    // 磁盘空间（这只是个数字，实际空间是按块分配的）
    int disk_space_size;

    // 磁盘块大小
    int block_size;

    // 磁盘块数
    int block_num;

    // 文件区块数
    int file_area_block_size;

    // 兑换分区块数
    int exchange_area_block_size;

    // 磁盘块
    std::vector<DiskBlock> blocks;

public:
    // 设置磁盘空间大小
    void set_disk_size(int size);

    // 获取磁盘空间大小
    int get_disk_size();

    // 设置磁盘块大小
    void set_block_size(int size);

    // 获取磁盘块大小
    int get_block_size();

    // 设置磁盘块数
    void set_block_num(int num);

    // 获取磁盘块数
    int get_block_num();

    // 设置文件区块数
    void set_file_area(int size);

    // 获取文件区块数
    int get_file_area();

    // 设置兑换分区块数
    void set_exchange_area(int size);

    // 获取兑换分区块数
    int get_exchange_area();

    // 初始化所有磁盘块
    void init_blocks();

    // 获取盘块号为n的盘块
    DiskBlock *get_block(int n);

    // 输出调试信息
    void str();

};

/**
 * 磁盘构造器
 */
struct DiskBuilder
{
private:
    Disk disk;

public:
    // 设置磁盘空间大小
    DiskBuilder &set_disk_size(int size);

    // 设置磁盘块大小
    DiskBuilder &set_block_size(int size);

    // 设置文件区块数
    DiskBuilder &set_file_area_block_num(int size);

    // 设置兑换分区块数
    DiskBuilder &set_exchange_area_block_num(int size);

    // 初始化所有磁盘块
    DiskBuilder &init_blocks();

    // 构造链尾
    Disk build();
};

/**
 * 磁盘管理类
 * 使用成组链接法管理空闲块
 */
class DiskManager
{
private:
    // 磁盘
    Disk disk;
    // 超级块索引
    const int SUPER_BLOCK_INDEX = 0;

    // 超级块
    DiskBlock *super_block;

    // 成组链块大小
    const int GROUP_BLOCK_SIZE = 128;

    // 文件分配表
    std::map<int, int> file_allocation_table;

    // 成组链块
    struct GroupBlock
    {
        std::vector<int> block_numbers;
        // 前一个成组链块，便于删除
        GroupBlock *prev_group_block;
        // 下一个成组链块
        GroupBlock *next_group_block;

        // 初始化
        GroupBlock(int begin, int size);

        // 添加盘块号
        void add_block(int block_number);

        // 设置下一个成组链块
        void set_next_group_block(GroupBlock *next_group_block);
    };

    // 成组链块头
    GroupBlock *group_block_head;

    // 兑换区用的循环队列，用于存储空闲块
    std::queue<int> exchange_area_queue;
    

public:
    // 构造函数
    DiskManager();

    // 初始化磁盘
    void init_disk();

    // 获取超级块内容
    char *get_super_block_content() const;

    // 设置超级块内容
    void set_super_block_content(char *content);

    // 保存文件
    void save_file(const char *file_content);

    // 输出调试信息
    void str();

    // 输出磁盘的调试信息
    void disk_str();

    // 兑换区分配盘块, 返回盘块号
    int allocate_exchange_area_blocks();

    // 文件区分配盘块，返回盘块号
    std::vector<int> allocate_file_area_blocks(int n);

    // 删除成组链块
    void delete_group_block();

    // 删除成组链块中的n个盘块
    void delete_blocks_in_group_block(int n);

    // 释放n个盘块
    void free_blocks(std::vector<int> block_numbers);

    // 向某个盘块写入内容
    void write_block(int block_number, char *content);

    // 从某个盘块读取内容
    char *read_block(int block_number);

    // 向多个盘块写入内容
    void write_blocks(std::vector<int> block_numbers, char *content);

    // 从多个盘块读取内容
    char *read_blocks(std::vector<int> block_numbers);
};
