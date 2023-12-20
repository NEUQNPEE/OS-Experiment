/*
 * @Author       : NieFire planet_class@foxmail.com
 * @Date         : 2023-12-19 22:06:20
 * @LastEditors  : NieFire planet_class@foxmail.com
 * @LastEditTime : 2023-12-21 02:11:32
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

// 磁盘块
struct DiskBlock
{
    // 磁盘块大小
    int size;

    // 磁盘块号
    int number;

    // 磁盘块状态
    DiskBlockStatus status;

    // 磁盘块内容
    char *content;

    DiskBlock(int size, int number)
    {
        this->size = size;
        this->number = number;
        this->status = FREE;
    }

    // 设置磁盘块内容
    void set_content(char *content)
    {
        // 检查磁盘块大小是否足够
        if (sizeof(content) > size)
        {
            std::cout << "磁盘块大小不足" << std::endl;
            return;
        }

        this->content = content;
    }

    // 获取磁盘块内容
    char *get_content()
    {
        return content;
    }

    // 设置磁盘块状态
    void set_status(DiskBlockStatus status)
    {
        this->status = status;
    }

    // 获取磁盘块状态
    int get_status()
    {
        return status;
    }

    // 获取磁盘块号
    int get_number()
    {
        return number;
    }
};

// 磁盘结构体，用于模拟磁盘
struct Disk
{
private:
    // 磁盘空间（这只是个数字，实际空间是按块分配的）
    int disk_space_size;

    // 磁盘块大小
    int block_size;

    // 文件区块数
    int file_area_block_size;

    // 文件区磁盘块
    std::vector<DiskBlock> file_area_blocks;

    // 兑换分区块数
    int exchange_area_block_size;

    // 兑换分区磁盘块
    std::vector<DiskBlock> exchange_area_blocks;

public:
    // 设置磁盘空间大小
    void set_disk_size(int size)
    {
        disk_space_size = size;
    }

    // 获取磁盘空间大小
    int get_disk_size()
    {
        return disk_space_size;
    }

    // 设置磁盘块大小
    void set_block_size(int size)
    {
        block_size = size;
    }

    // 获取磁盘块大小
    int get_block_size()
    {
        return block_size;
    }

    // 设置文件区块数
    void set_file_area(int size)
    {
        file_area_block_size = size;
    }

    // 获取文件区块数
    int get_file_area()
    {
        return file_area_block_size;
    }

    // 设置文件区磁盘块
    void set_file_area_blocks()
    {
        for (int i = 0; i < file_area_block_size; i++)
        {
            DiskBlock block(block_size, i);
            file_area_blocks.push_back(block);
        }
    }

    // 初始化文件区
    void init_file_area(int size)
    {
        set_file_area(size);
        set_file_area_blocks();
    }

    // 设置兑换分区块数
    void set_exchange_area(int size)
    {
        exchange_area_block_size = size;
    }

    // 获取兑换分区块数
    int get_exchange_area()
    {
        return exchange_area_block_size;
    }

    // 设置兑换分区磁盘块
    void set_exchange_area_blocks()
    {
        for (int i = 0; i < exchange_area_block_size; i++)
        {
            DiskBlock block(block_size, i);
            exchange_area_blocks.push_back(block);
        }
    }

    // 初始化兑换分区
    void init_exchange_area(int size)
    {
        set_exchange_area(size);
        set_exchange_area_blocks();
    }

    void str()
    {
        std::cout << "disk_space_size: " << disk_space_size << std::endl;
        std::cout << "block_size: " << block_size << std::endl;
        std::cout << "file_area_size: " << file_area_block_size << std::endl;
        std::cout << "exchange_area_size: " << exchange_area_block_size << std::endl;
        // 看一下文件区的前三个块和兑换分区的前三个块
        std::cout << "file_area_blocks: " << std::endl;
        for (int i = 0; i < 3; i++)
        {
            std::cout << "block_size: " << file_area_blocks[i].size << std::endl;
            std::cout << "block_number: " << file_area_blocks[i].number << std::endl;
            std::cout << "block_status: " << file_area_blocks[i].status << std::endl;
            std::cout << "block_content: " << file_area_blocks[i].content << std::endl;
        }
        std::cout << "exchange_area_blocks: " << std::endl;
        for (int i = 0; i < 3; i++)
        {
            std::cout << "block_size: " << exchange_area_blocks[i].size << std::endl;
            std::cout << "block_number: " << exchange_area_blocks[i].number << std::endl;
            std::cout << "block_status: " << exchange_area_blocks[i].status << std::endl;
            std::cout << "block_content: " << exchange_area_blocks[i].content << std::endl;
        }
    }
};

// 磁盘构造器
struct DiskBuilder
{
    Disk disk;

    DiskBuilder &set_disk_size(int size)
    {
        disk.set_disk_size(size);
        return *this;
    }

    DiskBuilder &set_block_size(int size)
    {
        disk.set_block_size(size);
        return *this;
    }

    DiskBuilder &init_file_area(int size)
    {
        disk.init_file_area(size);
        return *this;
    }

    DiskBuilder &init_exchange_area(int size)
    {
        disk.init_exchange_area(size);
        return *this;
    }

    Disk build()
    {
        return disk;
    }
};

// int main()
// {
//     DiskBuilder disk_builder;
//     Disk disk = disk_builder.set_disk_size(40 * 1024)
//                     .set_block_size(40)
//                     .init_file_area(900)
//                     .init_exchange_area(124)
//                     .build();
//     disk.str();

//     return 0;
// }
