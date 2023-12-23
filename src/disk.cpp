/*
 * @Author       : NieFire planet_class@foxmail.com
 * @Date         : 2023-12-19 22:06:20
 * @LastEditors  : NieFire planet_class@foxmail.com
 * @LastEditTime : 2023-12-24 02:59:12
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
}

void DiskBlock::set_content(char *content)
{
    // 检查磁盘块大小是否足够
    if (sizeof(content) > size)
    {
        std::cout << "磁盘块大小不足" << std::endl;
        return;
    }

    this->content = content;
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

    // 已知磁盘空间大小，磁盘块大小，计算磁盘块数
    set_block_num(disk_space_size / block_size);
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

void Disk::set_file_area_blocks()
{
    for (int i = 0; i < file_area_block_size; i++)
    {
        DiskBlock *block = &blocks[i];
        file_area_blocks.push_back(block);
    }
}

void Disk::set_exchange_area(int size)
{
    exchange_area_block_size = size;
}

int Disk::get_exchange_area()
{
    return exchange_area_block_size;
}

void Disk::set_exchange_area_blocks()
{
    for (int i = file_area_block_size; i < file_area_block_size + exchange_area_block_size; i++)
    {
        DiskBlock *block = &blocks[i];
        exchange_area_blocks.push_back(block);
    }
}

void Disk::init_blocks()
{
    for (int i = 0; i < block_num; i++)
    {
        DiskBlock block(block_size, i);
        blocks.push_back(block);
    }

    set_file_area_blocks();
    set_exchange_area_blocks();
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
    // 初始化文件分配表为所有块都是未分配的
    int total_blocks = disk.get_block_num();
    file_allocation_table.resize(total_blocks, false); // 所有块初始化为false，表示未分配

    DiskBuilder disk_builder;
    disk = disk_builder.set_disk_size(40 * 1024)
               .set_block_size(40)
               .set_file_area_block_num(900)
               .set_exchange_area_block_num(124)
               .init_blocks()
               .build();

    // 指定第一个块为超级块
    super_block = disk.get_block(SUPER_BLOCK_INDEX);

    // 初始化成组链块
    group_block_head = new GroupBlock(SUPER_BLOCK_INDEX, GROUP_BLOCK_SIZE);
    int block_num = disk.get_block_num() - GROUP_BLOCK_SIZE;

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

    // todo 设置超级块内容
}

char* DiskManager::get_super_block_content() const
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
    int *block_ids = allocate_blocks(file_block_num);

    // 将文件内容写入块
    for (int i = 0; i < file_block_num; i++)
    {
        disk.get_block(block_ids[i])->set_content((char *)file_content + i * disk.get_block_size());
    }
}

void DiskManager::store_content_in_exchange_area(const std::string &content, int block_number)
{
    DiskBlock *exchange_block = disk.get_block(block_number);
    if (exchange_block)
    {
        strncpy(exchange_block->get_content(), content.c_str(), 40); // 假设每个磁盘块的大小为40个字符
        exchange_block->set_status(ALLOCATED);                        // 设置为已分配（如果需要）
    }
}

void DiskManager::store_contents_in_exchange_area(const std::vector<std::string> &contents, int start_block_number)
{
    int num_blocks = contents.size();
    for (int i = 0; i < num_blocks; ++i)
    {
        store_content_in_exchange_area(contents[i], start_block_number + i);
    }
}

std::pair<std::string, int> DiskManager::retrieve_content_from_exchange_area(int block_number)
{
    DiskBlock *exchange_block = disk.get_block(block_number);
    if (exchange_block && exchange_block->get_status() == ALLOCATED)
    {
        return {std::string(exchange_block->get_content(), 40), block_number}; // 返回块内容和块号
    }
    return {"", -1}; // 返回空内容和无效块号
}

std::vector<std::pair<std::string, int>> DiskManager::retrieve_contents_from_exchange_area(int start_block_number, int num_blocks)
{
    std::vector<std::pair<std::string, int>> contents;
    for (int i = 0; i < num_blocks; ++i)
    {
        auto [content, block_num] = retrieve_content_from_exchange_area(start_block_number + i);
        if (!content.empty())
        {
            contents.push_back({content, block_num});
        }
    }
    return contents;
}

void DiskManager::str()
{
    // 输出成组链块分组情况
    GroupBlock *temp_block = group_block_head;
    int i = 1;
    while (temp_block != nullptr)
    {
        std::cout << "第" << i << "组从";
        std::cout << temp_block->block_numbers[0] << "到" << temp_block->block_numbers[temp_block->block_numbers.size() - 1] << "块";
        std::cout << "\n";
        i++;
        temp_block = temp_block->next_group_block;
    }
}

// 从成组链块中分配n个块
int *DiskManager::allocate_blocks(int n)
{
    // 从头指针指向的块开始分配
    GroupBlock *temp_block = group_block_head;
    int *block_ids = new int[n];
    int i = 0;
    while (temp_block != nullptr)
    {
        // 如果该成组链块的块数大于等于n，直接分配
        if (temp_block->block_numbers.size() >= n)
        {
            for (int j = 0; j < n; j++)
            {
                block_ids[i] = temp_block->block_numbers[j];
                i++;
            }
            // 删除分配的块
            delete_blocks_in_group_block(temp_block, n);
            return block_ids;
        }

        // 如果该成组链块的块数小于n，将该成组链块的块分配完

        for (int j = 0; j < temp_block->block_numbers.size(); j++)
        {
            block_ids[i] = temp_block->block_numbers[j];
            i++;
        }
        // 删除该成组链块
        delete_group_block(temp_block);
        n -= temp_block->block_numbers.size();
    }
}

// 删除某个成组链块，传入的参数是该成组链块的指针
void DiskManager::delete_group_block(GroupBlock *group_block)
{
    // 如果该成组链块是头指针，头指针要指向下一个成组链块
    if (group_block == group_block_head)
    {
        group_block_head = group_block->next_group_block;
        delete group_block;
        return;
    }

    // 如果该成组链块不是头指针
    group_block->prev_group_block->next_group_block = group_block->next_group_block;
    group_block->next_group_block->prev_group_block = group_block->prev_group_block;
    delete group_block;
    return;
}

// 删除某个成组链块的前n个块
void DiskManager::delete_blocks_in_group_block(GroupBlock *group_block, int n)
{
    if (group_block->block_numbers.size() < n)
    {
        std::cout << "删除块数大于成组链块块数" << std::endl;
        return;
    }

    group_block->block_numbers.erase(group_block->block_numbers.begin(), group_block->block_numbers.begin() + n);
}
// int main()
// {
//     DiskManager disk_manager;

//     disk_manager.init_disk();

//     disk_manager.disk.str();

//     return 0;
// }
