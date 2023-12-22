/*
 * @Author       : NieFire planet_class@foxmail.com
 * @Date         : 2023-12-19 22:06:20
 * @LastEditors  : NieFire planet_class@foxmail.com
 * @LastEditTime : 2023-12-21 19:00:36
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

    // 磁盘块数
    int block_num;

    // 文件区块数
    int file_area_block_size;

    // 磁盘块
    std::vector<DiskBlock> blocks;

    // 文件区磁盘块(放指针)
    std::vector<DiskBlock *> file_area_blocks;

    // 兑换分区块数
    int exchange_area_block_size;

    // 兑换分区磁盘块(放指针)
    std::vector<DiskBlock *> exchange_area_blocks;

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

        // 已知磁盘空间大小，磁盘块大小，计算磁盘块数
        set_block_num(disk_space_size / block_size);
    }

    // 获取磁盘块大小
    int get_block_size()
    {
        return block_size;
    }

    // 设置磁盘块数
    void set_block_num(int num)
    {
        block_num = num;
    }

    // 获取磁盘块数
    int get_block_num()
    {
        return block_num;
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
            DiskBlock *block = &blocks[i];
            file_area_blocks.push_back(block);
        }
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
        for (int i = file_area_block_size; i < file_area_block_size + exchange_area_block_size; i++)
        {
            DiskBlock *block = &blocks[i];
            exchange_area_blocks.push_back(block);
        }
    }

    // 初始化所有磁盘块
    void init_blocks()
    {
        for (int i = 0; i < block_num; i++)
        {
            DiskBlock block(block_size, i);
            blocks.push_back(block);
        }

        set_file_area_blocks();
        set_exchange_area_blocks();
    }

    // 获取盘块号为n的盘块
    DiskBlock *get_block(int n)
    {
        return &blocks[n];
    }

    void str()
    {
        std::cout << "disk_space_size: " << disk_space_size << std::endl;
        std::cout << "block_size: " << block_size << std::endl;
        std::cout << "file_area_size: " << file_area_block_size << std::endl;
        std::cout << "exchange_area_size: " << exchange_area_block_size << std::endl;
        // 看一下文件区的前三个块和兑换分区的前三个块
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

    DiskBuilder &set_file_area_block_num(int size)
    {
        disk.set_file_area(size);
        return *this;
    }

    DiskBuilder &set_exchange_area_block_num(int size)
    {
        disk.set_exchange_area(size);
        return *this;
    }

    DiskBuilder &init_blocks()
    {
        disk.init_blocks();
        return *this;
    }

    Disk build()
    {
        return disk;
    }
};

/*
 * 磁盘管理类
 * 使用成组链接法管理空闲块
 */
class DiskManager
{
public:
    // 磁盘
    Disk disk;

private:
    // 超级块索引
    const int SUPER_BLOCK_INDEX = 0;

    // 超级块
    DiskBlock *super_block;

    // 成组链块大小
    const int GROUP_BLOCK_SIZE = 128;

    // 成组链块
    struct GroupBlock
    {
        // 存储的盘块号
        std::vector<int> block_numbers;

        // 下一个成组链块
        GroupBlock *next_group_block;

        // 初始化
        GroupBlock(int begin, int size)
        {
            for (int i = begin; i < begin + size; i++)
            {
                block_numbers.push_back(i);
            }
        }

        // 添加盘块号
        void add_block(int block_number)
        {
            block_numbers.push_back(block_number);
        }

        // 设置下一个成组链块
        void set_next_group_block(GroupBlock *next_group_block)
        {
            this->next_group_block = next_group_block;
        }
    };

    // 成组链块唯一指针
    GroupBlock *group_block;

public:
    // 初始化磁盘
    void init_disk()
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
        group_block = new GroupBlock(SUPER_BLOCK_INDEX, GROUP_BLOCK_SIZE);
        int block_num = disk.get_block_num() - GROUP_BLOCK_SIZE;

        int now_block_num = SUPER_BLOCK_INDEX + GROUP_BLOCK_SIZE;
        GroupBlock *temp_block = group_block;
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

    // 获取超级块内容
    char *get_super_block_content()
    {
        return super_block->get_content();
    }

    // 设置超级块内容
    void set_super_block_content(char *content)
    {
        super_block->set_content(content);
    }

    /*
    * 比如说内存调用我的函数，要求放入一个文件

    */

    // 放入文件函数
    void save_file(char *file_content)
    {
        // 计算文件内容长度和所需块数
        int content_length = strlen(file_content);
        int required_blocks = (content_length + 39) / 40; // 向上取整

        // 根据所需块数为文件分配磁盘块
        for (int i = 0; i < required_blocks; ++i)
        {
            int block_number = diskManager.allocateBlock();

            if (block_number == -1)
            {
                std::cout << "not enough disk space!" << std::endl;
                return;
            }

            // 获取并设置磁盘块内容
            DiskBlock *block = diskManager.get_block(block_number);
            if (block)
            {
                // 从file_content适当位置开始复制
                strncpy(block->content, file_content + i * 40, 40);
                block->allocated = true; // 设置为已分配

                // 更新文件分配表，将对应的块标记为已分配
                file_allocation_table[block_number] = true; // 假设file_allocation_table是DiskManager的成员变量
            }
        }
    }

    /**
     * 内存管理系统把磁盘块读入，会记录读入的磁盘块号
     */

    // 取出文件函数
    // 内存系统要求兑换分区存入内容,传入一个块的内容，同时传入块号
    void store_content_in_exchange_area(const std::string &content, int block_number)
    {
        DiskBlock *exchange_block = disk.get_block(block_number);
        if (exchange_block)
        {
            strncpy(exchange_block->content, content.c_str(), 40); // 假设每个磁盘块的大小为40个字符
            exchange_block->allocated = true;                      // 设置为已分配（如果需要）
        }
    }

    // 内存系统要求向兑换分区存入内容,传入n个块的内容，同时传入块号
    void store_contents_in_exchange_area(const std::vector<std::string> &contents, int start_block_number)
    {
        int num_blocks = contents.size();
        for (int i = 0; i < num_blocks; ++i)
        {
            store_content_in_exchange_area(contents[i], start_block_number + i);
        }
    }

    // 内存系统要求从兑换分区取出内容,取出一个块的内容，同时取回块号
    std::pair<std::string, int> retrieve_content_from_exchange_area(int block_number)
    {
        DiskBlock *exchange_block = disk.get_block(block_number);
        if (exchange_block && exchange_block->allocated)
        {
            return {std::string(exchange_block->content, 40), block_number}; // 返回块内容和块号
        }
        return {"", -1}; // 返回空内容和无效块号
    }

    // 内存系统要求从兑换分区取出内容,取出n个块的内容，同时取回块号
    std::vector<std::pair<std::string, int>> retrieve_contents_from_exchange_area(int start_block_number, int num_blocks)
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
};

// int main()
// {
//     DiskManager disk_manager;

//     disk_manager.init_disk();

//     disk_manager.disk.str();

//     return 0;
// }
