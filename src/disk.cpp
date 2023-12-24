/*
 * @Author       : NieFire planet_class@foxmail.com
 * @Date         : 2023-12-19 22:06:20
 * @LastEditors  : NieFire planet_class@foxmail.com
 * @LastEditTime : 2023-12-24 20:30:43
 * @FilePath     : \OS-Experiment\src\disk.cpp
 * @Description  : 磁盘管理
 * ( ﾟ∀。)只要加满注释一切都会好起来的( ﾟ∀。)
 * Copyright (c) 2023 by NieFire, All Rights Reserved.
 */
#include "disk.h"
/*
建立一个40KB大小的文件，作为模拟磁盘。
将其逻辑划分为1024块，每块大小40B。
其中900块用于模拟文件区，124块用于模拟兑换分区。
*/

// todo-1 初始化的各值由配置文件读入

// 重载输出运算符以输出磁盘块状态
std::ostream &operator<<(std::ostream &os, const DiskBlockStatus &status)
{
    switch (status)
    {
    case FREE:
        os << "FREE";
        break;
    case ALLOCATED:
        os << "ALLOCATED";
        break;
    case OCCUPIED:
        os << "OCCUPIED";
        break;
    default:
        os << "UNKNOWN";
        break;
    }
    return os;
}

/**
 * 磁盘块的函数实现
 */

DiskBlock::DiskBlock(int size, int number)
{
    this->size = size;
    this->number = number;
    this->status = FREE;
    this->content = new char[size];
}

void DiskBlock::set_content(char *content)
{
    // 检查磁盘块大小是否足够
    if (strlen(content) > size)
    {
        std::cout << "磁盘块大小不足" << std::endl;
        return;
    }

    strcpy(this->content, content);
}

char *DiskBlock::get_content() const
{
    return content;
}

void DiskBlock::set_status(DiskBlockStatus status)
{
    this->status = status;
}

DiskBlockStatus DiskBlock::get_status() const
{
    return status;
}

int DiskBlock::get_number() const
{
    return number;
}

/**
 * 磁盘的函数实现
 */

void Disk::set_disk_size(int size)
{
    disk_space_size = size;
}

int Disk::get_disk_size()
{
    return disk_space_size;
}

void Disk::set_block_size(int size)
{
    block_size = size;
}

int Disk::get_block_size()
{
    return block_size;
}

void Disk::set_block_num(int num)
{
    block_num = num;
}

int Disk::get_block_num()
{
    return block_num;
}

void Disk::set_file_area(int size)
{
    file_area_block_size = size;
}

int Disk::get_file_area()
{
    return file_area_block_size;
}

void Disk::set_exchange_area(int size)
{
    exchange_area_block_size = size;
}

int Disk::get_exchange_area()
{
    return exchange_area_block_size;
}

void Disk::init_blocks()
{
    for (int i = 0; i < block_num; i++)
    {
        DiskBlock block(block_size, i);
        blocks.push_back(block);
    }
}

DiskBlock *Disk::get_block(int n)
{
    return &blocks[n];
}

void Disk::str()
{
    std::cout << "disk_space_size: " << disk_space_size << std::endl;
    std::cout << "block_size: " << block_size << std::endl;
    std::cout << "file_area_size: " << file_area_block_size << std::endl;
    std::cout << "exchange_area_size: " << exchange_area_block_size << std::endl;
}

/**
 * 磁盘构造器的函数实现
 */

DiskBuilder &DiskBuilder::set_disk_size(int size)
{
    disk.set_disk_size(size);
    return *this;
}

DiskBuilder &DiskBuilder::set_block_size(int size)
{
    disk.set_block_size(size);
    return *this;
}

DiskBuilder &DiskBuilder::set_file_area_block_num(int size)
{
    disk.set_file_area(size);
    return *this;
}

DiskBuilder &DiskBuilder::set_exchange_area_block_num(int size)
{
    disk.set_exchange_area(size);
    return *this;
}

DiskBuilder &DiskBuilder::init_blocks()
{
    disk.init_blocks();
    return *this;
}

Disk DiskBuilder::build()
{
    // 已知磁盘空间大小，磁盘块大小，计算磁盘块数
    disk.set_block_num(disk.get_disk_size() / disk.get_block_size());

    init_blocks();
    return disk;
}

/**
 * 磁盘管理器的函数实现
 */

/**
 * 首先是内部成组链块的函数实现
 */

DiskManager::GroupBlock::GroupBlock(int begin, int size)
{
    for (int i = begin; i < begin + size; i++)
    {
        block_numbers.push_back(i);
    }
}

void DiskManager::GroupBlock::add_block(int block_number)
{
    block_numbers.push_back(block_number);
}

void DiskManager::GroupBlock::set_next_group_block(GroupBlock *next_group_block)
{
    this->next_group_block = next_group_block;
}

/**
 * 磁盘管理器自己的的函数实现
 */
DiskManager::DiskManager()
{
    init_disk();
}

void DiskManager::init_disk()
{
    // todo 初始化文件分配表为所有块都是未分配的

    DiskBuilder disk_builder;
    disk = disk_builder.set_disk_size(40 * 1024)
               .set_block_size(40)
               .set_file_area_block_num(900)
               .set_exchange_area_block_num(124)
               .build();

    // 指定第一个块为超级块
    super_block = disk.get_block(SUPER_BLOCK_INDEX);

    // 初始化成组链块,将文件区所有块分为成组链块
    group_block_head = new GroupBlock(SUPER_BLOCK_INDEX, GROUP_BLOCK_SIZE);
    int block_num = disk.get_file_area() - GROUP_BLOCK_SIZE;

    int now_block_num = SUPER_BLOCK_INDEX + GROUP_BLOCK_SIZE;
    GroupBlock *temp_block = group_block_head;
    while (block_num > 0)
    {
        if (block_num > GROUP_BLOCK_SIZE)
        {
            temp_block->set_next_group_block(new GroupBlock(now_block_num, GROUP_BLOCK_SIZE));
            temp_block = temp_block->next_group_block;
            now_block_num += GROUP_BLOCK_SIZE;
            block_num -= GROUP_BLOCK_SIZE;
        }

        if (block_num <= GROUP_BLOCK_SIZE && block_num > 0)
        {
            temp_block->set_next_group_block(new GroupBlock(now_block_num, block_num));
            temp_block = temp_block->next_group_block;
            temp_block->set_next_group_block(nullptr);
            block_num = 0;
        }
    }

    // 初始化兑换区循环队列
    exchange_area_queue = std::queue<int>();
    for (int i = disk.get_file_area(); i < disk.get_file_area() + disk.get_exchange_area(); i++)
    {
        exchange_area_queue.push(i);
    }

    // todo 设置超级块内容
}

char *DiskManager::get_super_block_content() const
{
    return super_block->get_content();
}

void DiskManager::set_super_block_content(char *content)
{
    super_block->set_content(content);
}

void DiskManager::save_file(const char *file_content)
{
    // 计算文件内容长度和所需块数
    int file_content_length = strlen(file_content);
    int file_block_num = file_content_length / disk.get_block_size() + 1;

    // 检查磁盘空间是否足够
    if (file_block_num > disk.get_file_area())
    {
        std::cout << "磁盘空间不足" << std::endl;
        return;
    }

    // 分配块
    std::vector<int> block_ids = allocate_file_area_blocks(file_block_num);

    // 将文件内容写入块
    for (int i = 0; i < file_block_num; i++)
    {
        disk.get_block(block_ids[i])->set_content((char *)file_content + i * disk.get_block_size());
    }
}

void DiskManager::str()
{
    // 输出成组链块分组情况
    GroupBlock *temp_block = group_block_head;
    int i = 1;
    while (temp_block != nullptr)
    {
        std::cout << "第" << i << "组成组链块：" << std::endl;
        for (int j = 0; j < temp_block->block_numbers.size(); j++)
        {
            std::cout << temp_block->block_numbers[j] << " ";
        }
        std::cout << std::endl;

        i++;
        temp_block = temp_block->next_group_block;
    }
}

void DiskManager::disk_str()
{
    disk.str();
}

// 兑换区分配盘块, 返回盘块号
int DiskManager::allocate_exchange_area_blocks()
{
    // 直接去队列头取
    int block_number = exchange_area_queue.front();
    exchange_area_queue.pop();
    return block_number;
}

// 从成组链块中分配n个块
std::vector<int> DiskManager::allocate_file_area_blocks(int n)
{
    std::vector<int> block_ids(n);

    int i = 0;
    while (i < n)
    {
        GroupBlock *temp_block = group_block_head;

        // 如果该成组链块的块数大于等于n，直接分配
        if (temp_block->block_numbers.size() >= n)
        {
            for (int j = 0; j < n; j++)
            {
                block_ids[i] = temp_block->block_numbers[j];
                i++;
            }
            // 删除分配的块
            delete_blocks_in_group_block(n);
            return block_ids;
        }

        // 如果该成组链块的块数小于n，将该成组链块的块分配完
        for (int j = 0; j < temp_block->block_numbers.size(); j++)
        {
            block_ids[i] = temp_block->block_numbers[j];
            i++;
        }

        n -= temp_block->block_numbers.size();

        // 删除该成组链块
        delete_group_block();
    }
}

// 删除整个成组链块
void DiskManager::delete_group_block()
{
    GroupBlock *temp = group_block_head;
    group_block_head = group_block_head->next_group_block;
    delete temp;
}

// 删除前n个块
void DiskManager::delete_blocks_in_group_block(int n)
{
    group_block_head->block_numbers.erase(group_block_head->block_numbers.begin(), group_block_head->block_numbers.begin() + n);
}

// 释放n个块
void DiskManager::free_blocks(std::vector<int> block_numbers)
{
    // 从头指针指向的块开始释放
    GroupBlock *temp_block = group_block_head;

    int i = 0;
    while (i < block_numbers.size())
    {
        // 只要不满成组链块大小上限，就一直往头插入
        while (temp_block->block_numbers.size() < GROUP_BLOCK_SIZE)
        {
            temp_block->block_numbers.insert(temp_block->block_numbers.begin(), block_numbers[block_numbers.size() - i - 1]);
            i++;

            // 如果块已经释放完了，就返回
            if (i == block_numbers.size())
            {
                return;
            }
        }

        // 如果满了，就新建一个成组链块
        GroupBlock *new_group_block = new GroupBlock(block_numbers[block_numbers.size() - i - 1], 1);

        // 将新建的成组链块插入到链表中
        new_group_block->next_group_block = temp_block;
        new_group_block->prev_group_block = temp_block->prev_group_block;
        temp_block->prev_group_block = new_group_block;
        group_block_head = new_group_block;

        temp_block = new_group_block;
        i++;
    }
}

// 向某个盘块写入内容
void DiskManager::write_block(int block_number, char *content)
{
    // 检查盘块大小是否足够
    if (strlen(content) > disk.get_block_size())
    {
        std::cout << "盘块大小不足" << std::endl;
        return;
    }

    disk.get_block(block_number)->set_content(content);
}

// 从某个盘块读取内容
char *DiskManager::read_block(int block_number)
{
    return disk.get_block(block_number)->get_content();
}

// 向多个盘块写入内容
void DiskManager::write_blocks(std::vector<int> block_numbers, char *content)
{
    if (strlen(content) > disk.get_block_size() * block_numbers.size())
    {
        std::cout << "盘块大小不足" << std::endl;
        return;
    }

    char *temp_content = new char[disk.get_block_size()];

    int i = 0;
    
    while(strlen(content) > 0){
        strncpy(temp_content, content, disk.get_block_size() - 1);
        temp_content[disk.get_block_size() - 1] = '\0';
        write_block(block_numbers[i], temp_content);
        block_numbers.pop_back();
        if(strlen(content) > disk.get_block_size() - 1)
        {
            content += disk.get_block_size() - 1;
        }
        else
        {
            content += strlen(content);
        }
        i++;
    }
}

// 从多个盘块读取内容
char *DiskManager::read_blocks(std::vector<int> block_numbers)
{
    char *content = new char[disk.get_block_size() * block_numbers.size()];
    for (int i = 0; i < block_numbers.size(); i++)
    {
        strncpy(content + i * disk.get_block_size(), read_block(block_numbers[i]), disk.get_block_size() - 1);
    }
    return content;
}

// int main()
// {
//     DiskManager disk_manager;

//     disk_manager.init_disk();

//     disk_manager.disk_str();

//     disk_manager.str();

//     // 开始测试
//     std::cout << "开始测试" << std::endl;

//     //测试分配块
//     std::cout << "测试分配块" << std::endl;
//     std::vector<int> block_ids_1 = disk_manager.allocate_file_area_blocks(10);
//     for (int i = 0; i < 10; i++)
//     {
//         std::cout << block_ids_1[i] << " ";
//     }
//     std::cout << std::endl;

//     // 输出成组链块分组情况
//     disk_manager.str();

//     // 一次分配200块
//     std::cout << "一次分配200块" << std::endl;
//     std::vector<int> block_ids_2 = disk_manager.allocate_file_area_blocks(200);
//     disk_manager.str();

//     // 向block_ids_1写入380个字符
//     std::cout << "向block_ids_1写入380个字符" << std::endl;
//     char *content_1 = new char[381];
//     const char chars[] = "abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789 ";
//     int char_index = 0;
    
//     for (int i = 0; i < 380; i++) {
//         content_1[i] = chars[char_index];
//         char_index = (char_index + 1) % 65;
//     }
    
//     content_1[380] = '\0';
//     std::cout << "content_1的长度为：" << strlen(content_1) << std::endl;

//     std::cout << "content_1: " << content_1 << std::endl;

//     disk_manager.write_blocks(block_ids_1, content_1);

//     // 显示写入结果
//     std::cout << "显示写入结果" << std::endl;
//     std::cout << disk_manager.read_blocks(block_ids_1) << std::endl;

//     // // 释放block_ids_1
//     // std::cout << "释放block_ids_1" << std::endl;
//     // disk_manager.free_blocks(block_ids_1, 10);
//     // disk_manager.str();

//     // // 释放block_ids_2
//     // std::cout << "释放block_ids_2" << std::endl;
//     // disk_manager.free_blocks(block_ids_2, 200);
//     // disk_manager.str();

//     return 0;
// }
