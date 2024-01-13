/*
 * @Author       : NieFire planet_class@foxmail.com
 * @Date         : 2023-12-19 22:06:20
 * @LastEditors  : NieFire planet_class@foxmail.com
 * @LastEditTime : 2024-01-13 16:54:37
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
    // 初始化磁盘块内容, 用空字符填充
    for (int i = 0; i < size; i++)
    {
        content[i] = '\0';
    }
}

void DiskBlock::set_content(char *content)
{
    // 检查content是否合法
    if (content == nullptr)
    {
        std::cout << "content为空" << std::endl;
        return;
    }

    // 如果开头就是'\0'，说明没内容，直接写40个'\0'进去
    if (content[0] == '\0')
    {
        for (int i = 0; i < 40; i++)
        {
            this->content[i] = '\0';
        }
        return;
    }

    // 检查content大小，如果小于磁盘块大小，就用空字符填充
    size_t length = strlen(content);
    // 转换为int类型
    int int_length = static_cast<int>(length);
    if (int_length < 40)
    {
        strcpy(this->content, content);
        return;
    }
    // 强行将content的末尾置为'\0'
    content[40 - 1] = '\0';

    // 检查磁盘块大小是否足够
    if (strlen(content) > 40)
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
    DiskBuilder disk_builder;
    disk = disk_builder.set_disk_size(40 * 1024)
               .set_block_size(40)
               .set_file_area_block_num(900)
               .set_exchange_area_block_num(124)
               .build();

    // 初始化成组链块,将文件区所有块分为成组链块

    // 首先初始化头指针
    group_block_head = new GroupBlock(124, GROUP_BLOCK_SIZE);

    // 计算成组链块数量(不包括头指针)
    int block_num = disk.get_file_area() - GROUP_BLOCK_SIZE;

    // 当前初始化的成组链块的块号
    int now_block_num = GROUP_BLOCK_SIZE + 124;
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

    // 读取fat表，计算fat表盘块号
    // todo 记得回来做向上取整
    int fat_size = disk.get_file_area() * sizeof(int) * 2 / disk.get_block_size();

    // 给fat表分配盘块（默认的）
    fat_block_numbers = allocate_file_area_blocks(fat_size);

    load_info_from_txt();

    // 读取fat表
    load_fat();

    // 读取超级块
    load_super_block();
}

int DiskManager::save_file(const char *file_content)
{
    char *temp_content = new char[strlen(file_content) + 1];
    strcpy(temp_content, file_content);

    // 计算文件内容长度和所需块数
    int file_content_length = strlen(temp_content);
    int file_block_num = file_content_length / disk.get_block_size() + 1;

    // 检查磁盘空间是否足够
    if (file_block_num > disk.get_file_area())
    {
        std::cout << "磁盘空间不足" << std::endl;
        return -1;
    }

    // 分配块
    std::vector<int> block_ids = allocate_file_area_blocks(file_block_num);

    // 将文件内容写入块
    write_blocks(block_ids, temp_content);

    // 更新fat表
    for (int i = 0; i < block_ids.size() - 1; i++)
    {
        file_allocation_table.insert(std::pair<int, int>(block_ids[i], block_ids[i + 1]));
    }
    file_allocation_table.insert(std::pair<int, int>(block_ids[block_ids.size() - 1], -1));

    // todo 内存系统要更新fat表
    save_info_in_txt();

    // 返回文件起始盘块号
    return block_ids[0];
}

void DiskManager::delete_file_info(int block_number)
{
    // 释放文件占用的块
    std::vector<int> block_numbers;
    block_numbers.push_back(block_number);
    int next_block_number = file_allocation_table[block_number];
    while (next_block_number != -1)
    {
        block_numbers.push_back(next_block_number);
        next_block_number = file_allocation_table[next_block_number];
    }
    free_blocks(block_numbers);

    // 从fat表中删除文件信息
    delete_fat(block_number);
}

int DiskManager::update_file_info(const char *file_info, int block_number)
{
    // 释放文件占用的块
    std::vector<int> block_numbers;
    block_numbers.push_back(block_number);
    int next_block_number = file_allocation_table[block_number];
    while (next_block_number != -1)
    {
        block_numbers.push_back(next_block_number);
        next_block_number = file_allocation_table[next_block_number];
    }
    free_blocks(block_numbers);

    // 从fat表中删除文件信息
    delete_fat(block_number);

    // 重新存入文件信息
    return save_file(file_info);
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

// 给内存系统提供的兑换区接口，传入数据，返回盘块号
int DiskManager::exchange_one_block(char *content)
{
    // 先分配盘块
    int block_number = allocate_exchange_area_blocks();

    // 写入数据
    write_block(block_number, content);

    // 返回盘块号
    return block_number;
}

// 从成组链块中分配n个块
std::vector<int> DiskManager::allocate_file_area_blocks(int n)
{
    std::vector<int> block_ids(n);
    int need_num = n;
    int i = 0;
    while (i < n)
    {
        GroupBlock *temp_block = group_block_head;

        // 如果该成组链块的块数大于等于n，直接分配
        if (temp_block->block_numbers.size() >= need_num)
        {
            for (int j = 0; j < need_num; j++)
            {
                block_ids[i] = temp_block->block_numbers[j];
                i++;
            }
            // 删除分配的块
            delete_blocks_in_group_block(need_num);
            return block_ids;
        }

        // 如果该成组链块的块数小于n，将该成组链块的块分配完
        for (int j = 0; j < temp_block->block_numbers.size(); j++)
        {
            block_ids[i] = temp_block->block_numbers[j];
            i++;
        }

        need_num -= temp_block->block_numbers.size();

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
    std::cout << "content: " << content << std::endl;
    if (strlen(content) > disk.get_block_size() * block_numbers.size())
    {
        std::cout << "盘块大小不足" << std::endl;
        return;
    }

    char *temp_content = new char[disk.get_block_size()];

    int i = 0;

    while (strlen(content) > 0)
    {
        strncpy(temp_content, content, disk.get_block_size() - 1);
        temp_content[disk.get_block_size() - 1] = '\0';
        int t = block_numbers[i];
        write_block(block_numbers[i], temp_content);
        // block_numbers.pop_back();
        if (strlen(content) > disk.get_block_size() - 1)
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
    int size = block_numbers.size();
    int num =(disk.get_block_size()-1) * block_numbers.size() + 1;
    char *content = new char[(disk.get_block_size()-1) * block_numbers.size() + 1];
    for (int i = 0; i < block_numbers.size(); i++)
    {
        int t = block_numbers[i];
        strncpy(content + i * (disk.get_block_size()-1), read_block(block_numbers[i]), disk.get_block_size() - 1);
    }
    // 添加结束符
    content[(disk.get_block_size()-1) * block_numbers.size()] = '\0';
    return content;
}

// // 存入文件信息
// void DiskManager::save_file_info(const char *file_info)
// {
//     int begin_block_number = save_file(file_info);
//     file_info_block_number = begin_block_number;

//     // 更新超级块
//     update_super_block();
// }

// 存入目录信息
void DiskManager::save_dir_info(const char *dir_info)
{
    // tip 在得知序列化格式之后可以进行合法性检测

    // 释放原来的目录信息占用的块
    if (dir_info_block_number != -1)
    {
        std::vector<int> block_numbers;
        block_numbers.push_back(dir_info_block_number);

        // 查询fat表有没有以dir_info_block_number为起始盘块号的文件
        auto at = file_allocation_table.find(dir_info_block_number);
        if (at != file_allocation_table.end())
        {
            int next_block_number = file_allocation_table[dir_info_block_number];
            while (next_block_number != -1)
            {
                block_numbers.push_back(next_block_number);
                next_block_number = file_allocation_table[next_block_number];
            }
        }

        // 释放块
        free_blocks(block_numbers);

        delete_fat(dir_info_block_number);

        int begin_block_number = save_file(dir_info);
        dir_info_block_number = begin_block_number;

        // 存入fat表
        save_fat();

        // 更新超级块
        update_super_block();
    }
}

// // 获取文件信息起始盘块号
// int DiskManager::get_file_info_block_number()
// {
//     return file_info_block_number;
// }

// 获取目录信息起始盘块号
int DiskManager::get_dir_info_block_number()
{
    return dir_info_block_number;
}

// 获取fat盘块号
std::vector<int> DiskManager::get_fat_block_numbers()
{
    return fat_block_numbers;
}

// fat序列化
void DiskManager::save_fat()
{
    // 计算fat表大小
    int fat_size = file_allocation_table.size() * (sizeof(int) * 2 + 3);
    char *fat_content = new char[fat_size + 1];
    fat_content[0] = '\0';

    // 将fat表序列化，以大括号分割，每一对key-value之间用空格分割
    for (const auto &pair : file_allocation_table)
    {
        char temp[sizeof(int) * 2 + 3];
        sprintf(temp, "{%d %d}", pair.first, pair.second);
        strncat(fat_content, temp, sizeof(int) * 2 + 3);
    }
    // 添加结束符
    fat_content[fat_size] = '\0';

    // 将fat表写入磁盘
    write_blocks(fat_block_numbers, fat_content);

    // 持久化
    save_info_in_txt();

    // 释放内存
    delete[] fat_content;
}

// fat反序列化
void DiskManager::load_fat()
{
    // 读取fat表
    char *fat_content = read_blocks(fat_block_numbers);

    // 清空fat表
    file_allocation_table.clear();

    // 将fat表反序列化
    int i = 0;
    while (true)
    {
        // 跳过空格
        while (fat_content[i] == ' ')
        {
            i++;
        }

        // 如果遇到结束符，就退出
        if (fat_content[i] == '\0')
        {
            break;
        }

        // 读取key
        int key = 0;
        // 跳过{
        while (fat_content[i] == '{')
        {
            i++;
        }

        while (fat_content[i] != ' ')
        {
            key = key * 10 + fat_content[i] - '0';
            i++;
        }

        // 跳过空格
        while (fat_content[i] == ' ')
        {
            i++;
        }

        // 读取value
        int value = 0;
        // 如果是负数
        if (fat_content[i] == '-')
        {
            i++;
            while (fat_content[i] != '}')
            {
                value = value * 10 + fat_content[i] - '0';
                i++;
            }
            value = -value;
        }
        else
        {
            // 跳过}
            while (fat_content[i] != '}')
            {
                value = value * 10 + fat_content[i] - '0';
                i++;
            }
        }

        i++;

        // 将key-value对插入fat表
        file_allocation_table.insert(std::pair<int, int>(key, value));
    }
}

// 从fat中删除n条记录，传入起始盘块号
void DiskManager::delete_fat(int block_number)
{
    // 从起始盘块号开始，删除n条记录
    int next_block_number = file_allocation_table[block_number];
    while (next_block_number != -1)
    {
        file_allocation_table.erase(block_number);
        block_number = next_block_number;
        next_block_number = file_allocation_table[block_number];
    }
    file_allocation_table.erase(block_number);
}

// 获取文件分配表
std::map<int, int> DiskManager::get_file_allocation_table()
{
    return file_allocation_table;
}

// 读取超级块
void DiskManager::load_super_block()
{
    // // 读取超级块内容，里面就两个int：文件信息起始盘块号和目录信息起始盘块号
    // char* super_block_content = read_block(fat_block_numbers.size() + 1);
    // // 解析超级块内容
    // int i = 0;
    // int file_info_block_number = 0;
    // int dir_info_block_number = 0;
    // while (super_block_content[i] != ' ')
    // {
    //     file_info_block_number = file_info_block_number * 10 + super_block_content[i] - '0';
    //     i++;
    // }

    // // 跳过空格
    // i++;

    // while (super_block_content[i] != '\0')
    // {
    //     dir_info_block_number = dir_info_block_number * 10 + super_block_content[i] - '0';
    //     i++;
    // }

    //// 更新文件信息起始盘块号和目录信息起始盘块号
    // this->file_info_block_number = file_info_block_number;

    // 读取超级块内容，里面就一个int：目录信息起始盘块号
    // 目前超级块定死304号盘块，目录信息定死在305号盘块

    // tip 需要灵活指定超级块是那一块
    
    char *super_block_content = read_block(304);
    // 解析超级块内容
    int i = 0;
    int number = 0;
    while (super_block_content[i] != '\0')
    {
        number = number * 10 + super_block_content[i] - '0';
        i++;
    }

    // 更新目录信息起始盘块号
    this->dir_info_block_number = number;

    // 为超级块分配一个盘块
    allocate_file_area_blocks(1);

    // 根据目录信息起始盘块号读fat表，确认分配数量
    std::vector<int> dir_use_blocks = read_fat(dir_info_block_number);
    int dir_use_blocks_num = dir_use_blocks.size();
    allocate_file_area_blocks(dir_use_blocks_num);
}

// 更新超级块
void DiskManager::update_super_block()
{
    // //将文件信息起始盘块号和目录信息起始盘块号写入超级块
    // char* super_block_content = new char[2 * sizeof(int) + 2];
    // sprintf(super_block_content, "%d %d", file_info_block_number, dir_info_block_number);
    // write_block(fat_block_numbers.size() + 1, super_block_content);

    // 将目录信息起始盘块号写入超级块（超级块里就一个int）
    char *super_block_content = new char[sizeof(int) + 1];
    sprintf(super_block_content, "%d", dir_info_block_number);
    write_block(304, super_block_content);
}

// 模拟用持久化
void DiskManager::save_info_in_txt()
{
    // 将磁盘信息写入txt
    // 如果没有文件就创建，如果有就清空
    std::ofstream disk_info_file("D:/OS/OS-Experiment/src/disk_info.txt", std::ios::out | std::ios::trunc | std::ios::binary);
    for (int i = 0; i < disk.get_file_area(); i++)
    {
        disk_info_file.write(disk.get_block(i)->get_content(), disk.get_block_size());
        disk_info_file.write("\n", 1);
    }
}

// 模拟用加载
void DiskManager::load_info_from_txt()
{
    // 从txt中读取磁盘信息
    std::ifstream disk_info_file("D:/OS/OS-Experiment/src/disk_info.txt", std::ios::in | std::ios::binary);
    if (!disk_info_file)
    {
        std::cout << "打开文件失败" << std::endl;
        return;
    }
    char *temp_content = new char[disk.get_block_size() + 1];

    // 注意模拟内容的格式，每40个字符后面跟一个换行符，共计1024行
    // 读入时每次读入41个字符，将第40个字符替换为'\0'（每个磁盘块最后一个字符为'\0'）
    // 读取文件内容，先读兑换区
    for (int i = 0; i < disk.get_exchange_area(); i++)
    {
        disk_info_file.read(temp_content, disk.get_block_size() + 1);
        temp_content[disk.get_block_size()] = '\0';
        disk.get_block(i)->set_content(temp_content);
    }
    // 再读文件区
    for (int i = 0; i < disk.get_file_area(); i++)
    {
        disk_info_file.read(temp_content, disk.get_block_size() + 1);
        temp_content[disk.get_block_size()] = '\0';
        disk.get_block(i + disk.get_exchange_area())->set_content(temp_content);
    }
}

std::vector<int> DiskManager::read_fat(int block_number)
{
    std::vector<int> block_numbers;
    block_numbers.push_back(block_number);
    auto at = file_allocation_table.find(dir_info_block_number);
    if (at != file_allocation_table.end())
    {
        int next_block_number = file_allocation_table[dir_info_block_number];
        while (next_block_number != -1)
        {
            block_numbers.push_back(next_block_number);
            next_block_number = file_allocation_table[next_block_number];
        }
    }
    return block_numbers;
}

std::vector<bool> DiskManager::get_disk_block_status()
{
    std::vector<bool> disk_block_status = std::vector<bool>(1024);
    // 先将所有块置为true
    for (int i = 0; i < 1024; i++)
    {
        disk_block_status[i] = true;
    }

    // 前124块兑换区块置为false
    for (int i = 0; i < 124; i++)
    {
        disk_block_status[i] = false;
    }

    // 其实是读成组链块的状态
    GroupBlock *temp_block = group_block_head;
    while (temp_block != nullptr)
    {
        for (int block_number : temp_block->block_numbers)
        {
            // 空闲就置为false
            disk_block_status[block_number] = false;
        }

        // 下一组
        temp_block = temp_block->next_group_block;
    }

    return disk_block_status;
}

// qt所需的成组链块情况，返回1024+8大小的int数组（std标准数组），-2表示终止，-1表示组块结束
std::vector<int> DiskManager::get_group_block_status()
{
    // 固定其1024+8大小
    std::vector<int> group_block_status = std::vector<int>(1024 + 8);
    // 先将所有块置为-2
    for (int i = 0; i < 1024 + 8; i++)
    {
        group_block_status[i] = -2;
    }

    // 读成组链块
    GroupBlock *temp_block = group_block_head;
    int i = 0;
    while (temp_block != nullptr)
    {
        for (int block_number : temp_block->block_numbers)
        {
            group_block_status[i] = block_number;
            i++;
        }

        // 下一组
        temp_block = temp_block->next_group_block;
        // 用-1标记组块结束，这也就是为什么要多出8个空间的原因
        group_block_status[i] = -1;
        i++;
    }

    return group_block_status;
}

// int main()
// {
//     DiskManager disk_manager;

//     disk_manager.disk_str();

//     disk_manager.str();

//     // 开始测试
//     std::cout << "开始测试" << std::endl;

//     // 测试分配块
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

//     for (int i = 0; i < 380; i++)
//     {
//         content_1[i] = chars[char_index];
//         char_index = (char_index + 1) % 65;
//     }

//     content_1[380] = '\0';
//     std::cout << "content_1的长度为：" << strlen(content_1) << std::endl;

//     std::cout << "content_1: " << content_1 << std::endl;

//     disk_manager.write_blocks(block_ids_1, content_1);

//     // 测试文件存储
//     std::cout << "测试文件存储" << std::endl;
//     disk_manager.save_file(content_1);

//     // 显示写入结果
//     std::cout << "显示写入结果" << std::endl;
//     std::cout << disk_manager.read_blocks(block_ids_1) << std::endl;

//     // 测试文件分配表
//     disk_manager.save_fat();

//     // 查看fat表
//     std::cout << "查看fat表的存储结果" << std::endl;
//     std::cout << disk_manager.read_blocks(disk_manager.get_fat_block_numbers()) << std::endl;

//     // 把fat表清空
//     disk_manager.get_file_allocation_table().clear();

//     // 重新读取fat表
//     disk_manager.load_fat();

//     // 对比fat表是否一致
//     std::cout << "对比fat表是否一致" << std::endl;
//     std::cout << disk_manager.read_blocks(disk_manager.get_fat_block_numbers()) << std::endl;

//     // 释放block_ids_1
//     std::cout << "释放block_ids_1" << std::endl;
//     disk_manager.free_blocks(block_ids_1);
//     disk_manager.str();

//     // 释放block_ids_2
//     std::cout << "释放block_ids_2" << std::endl;
//     disk_manager.free_blocks(block_ids_2);
//     disk_manager.str();

//     disk_manager.save_info_in_txt();

//     // disk_manager.load_info_from_txt();

//     // std::cout << "查看fat表的存储结果" << std::endl;
//     // std::cout << disk_manager.read_blocks(disk_manager.get_fat_block_numbers()) << std::endl;

//     return 0;
// }
