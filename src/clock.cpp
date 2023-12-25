#include <iostream>
#include <algorithm>
#include "memory.h"
#include "memoryBlock.h"
#include "disk.h"

using namespace std;
/*
 * 文件管理系统才知道一个文件使用那八个内存块
 */

// 用于装入文件的八个内存块的块号
int block_ids[8];
// 正在装入内存的文件分块后的各文件页。页最多为1024（40KB/40B）
string page_content[1024];
// 初始化DiskManager类。如果main函数已初始化，此条需注释
DiskManager disk;

// 初始化内存及相关操作
void initialMemory()
{
    //初始化内存
    fill(memory, memory + 2560, ' ');
    //初始化内存块
    initialMemoryBlock();
    // 初始化磁盘
    disk.init_disk();
    // 初始化FAT表
    fat_list = disk.get_fat_block_numbers();
    //初始化文件信息
    file_info = getFileInfo();
    //初始化目录信息
    dir_info = getDirectoryInfo();
}

// 返回文件信息
char *getFileInfo()
{
    int block_number = disk.get_file_info_block_number();
    return disk.read_block(block_number);
}

// 返回目录信息
char *getDirectoryInfo()
{
    int block_number = disk.get_dir_info_block_number();
    return disk.read_block(block_number);
}

// 决定由哪八个内存块装入文件
void initialBlock_ids()
{
    // 外遍历：确定装文件的内存块。内遍历：搜索未装页的内存块
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j <= 64; j++)
        {
            if (j == 64)
            {
                cout << "警告!已无空闲内存块!内存溢出!";
                exit(0);
            }
            if (memory_block[j].page_id < 0)
            {
                block_ids[i] = memory_block[j].block_id;
                break;
            }
        }
    }
}

// 释放装文件的八个内存块
void clearBlock_ids()
{
    for (int i = 0; i < 8; i++)
    {
        memory_block[block_ids[i]].page_id = -1;
        memory_block[block_ids[i]].status = 0;
        memory_block[block_ids[i]].file_all_in_memory = true;
    }
    // 顺带把暂存页内容的page_content清空
    fill(page_content, page_content + 1024, "");
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

// 调用disk.cpp的函数，将修改后的文件内容传进磁盘
void SaveDiskFile(string block_content)
{
    char *content = block_content.data();
    int disk_block_id = disk.save_file(content);
    //存储文件后磁盘会更新FAT表，所以更新fat_list
    fat_list = disk.get_fat_block_numbers();
}

// 全局置换CLOCK算法。雏形已有，实现细节还需商讨后完善
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

// 这个是文件管理系统调用的函数，把用户修改过的新的文件数据传回内存
// 即：用户写文件
void WriteFile(string file_content, char *file_info, char *dir_info)
{
    // 搜索由哪八个内存块负责装文件
    initialBlock_ids();

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
    SaveDiskFile(file_content);
    // 更新文件信息
    disk.save_file_info(file_info);
    // 更新目录信息
    disk.save_dir_info(dir_info);
    // 释放内存
    clearBlock_ids();
}

// 根据FAT表从磁盘读取文件内容
char *ReadDiskFile()
{
    return disk.read_blocks(fat_list);
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
char *ReadFile()
{
    // 搜索由哪八个内存块负责装文件
    initialBlock_ids();
    // 从磁盘读出文件内容
    string file_content = ReadDiskFile();

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
        char *temp = ans + page_count * 40;
        temp = ReadMemoryBlock(write_block_id, remainder);
    }
    return ans;
    // return file_content;
}

/*
//以下为22日晚讨论出的框架。因思路有所改变，暂时搁置不用

// 文件管理系统不知道文件所使用的内存块，说明内存肯定没有这个文件，调用这个
// 从磁盘读入文件
int* readDiskFile(int file_id)
{
    // 调用磁盘管理系统的函数，传入file_id
    // 让磁盘把文件找到并返回文件的起始盘块号

    // 然后调用读取FAT表的函数，根据FAT表确定需要读入的所有磁盘块

    // 调用ReadDiskBlock

    // 返回装入的内存块块号，以数组形式返回
}

// 读入磁盘块
void ReadDiskBlock(int block_id)
{
    // 调用磁盘系统提供的函数，传入block_id，得到一个块

    // 选择放进那个内存块
}

// 文件管理系统要求保存某个文件
void SaveFile(int* block_ids,char* file)
{
    // 这传入的是文件所用到的内存块以及整个文件序列化后的char数组
    // 按照内存块大小吧file切分，依次调用WriteMemoryBolcks把内容写进内存块
    // 再调用WriteDiskBlock把这些块存入磁盘
}

// 文件管理系统管往哪个内存块写数据，内存管理系统不管这个
void WriteMemoryBolcks(int block_id,char* data)
{

}

// 文件管理系统调用该函数，让内存管理系统把某个文件的内存块存磁盘
// 文件管理系统知道要存哪八个块
void WriteDiskBlock(int* block_ids)
{
    // 这里调用磁盘系统的函数，把block_ids这几个内存块写回磁盘块
}
*/
