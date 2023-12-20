/*
 * @Author       : NieFire planet_class@foxmail.com
 * @Date         : 2023-12-19 22:06:20
 * @LastEditors  : NieFire planet_class@foxmail.com
 * @LastEditTime : 2023-12-20 21:42:38
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
// 磁盘结构体，用于模拟磁盘
struct Disk
{
private:
    // 磁盘空间
    char *disk_space;
    int disk_space_size;

    // 磁盘块大小
    int block_size;

    // 磁盘分区指针以及块数
    char *file_area_head;
    int file_area_size;
    char *exchange_area_head;
    int exchange_area_size;

public:
    // 设置磁盘空间大小
    void set_disk_size(int size)
    {
        disk_space_size = size;
        disk_space = new char[size];
    }

    // 获取磁盘空间大小
    int get_disk_size()
    {
        return disk_space_size;
    }

    // 设置磁盘块大小
    void set_block_size(int size)
    {
        this->block_size = size;
    }

    // 获取磁盘块大小
    int get_block_size()
    {
        return block_size;
    }

    // 设置文件区分区指针以及块数
    void set_file_area(int head, int size)
    {
        this->file_area_head = disk_space + head;
        this->file_area_size = size;
    }

    // 获取文件区盘块数量
    int get_file_area_size()
    {
        return file_area_size;
    }

    // 向文件区写入数据
    void write_file_area(int n, char *data)
    {
        // TODO memcpy(file_area_head + n * block_size, data, block_size);
    }

    // 获取文件区第n块的内容
    char *get_file_area_block(int n)
    {
        return file_area_head + n * block_size;
    }

    // 设置兑换分区分区指针以及块数
    void set_exchange_area(int head, int size)
    {
        this->exchange_area_head = disk_space + head;
        this->exchange_area_size = size;
    }

    // 获取兑换分区盘块数量
    int get_exchange_area_size()
    {
        return exchange_area_size;
    }

    // 获取兑换分区第n块的内容
    char *get_exchange_area_block(int n)
    {
        return exchange_area_head + n * block_size;
    }

    void str()
    {
        std::cout << "disk_space_size: " << disk_space_size << std::endl;
        std::cout << "block_size: " << block_size << std::endl;
        std::cout << "file_area_size: " << file_area_size << std::endl;
        std::cout << "exchange_area_size: " << exchange_area_size << std::endl;
    }
};

// 磁盘构造器
struct DiskBuilder
{
    Disk disk;

    DiskBuilder& set_disk_size(int disk_size)
    {
        disk.set_disk_size(disk_size);
        return *this;
    }

    DiskBuilder& set_block_size(int block_size)
    {
        disk.set_block_size(block_size);
        return *this;
    }

    DiskBuilder& set_file_area(int file_area_head, int file_area_size)
    {
        disk.set_file_area(file_area_head, file_area_size);
        return *this;
    }

    DiskBuilder& set_exchange_area(int exchange_area_head, int exchange_area_size)
    {
        disk.set_exchange_area(exchange_area_head, exchange_area_size);
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
//     Disk disk = disk_builder.set_disk_size(40*1024)
//                             .set_block_size(40)
//                             .set_file_area(0, 900)
//                             .set_exchange_area(900, 124)
//                             .build();
//     disk.str();

//     return 0;
// }
