#include <iostream>
#include <vector>

using namespace std;

// 模拟的分块后的内存块
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
    //所存文件的所有页是否都在内存里
    bool file_all_in_memory = true;
    //所属的进程的进程ID
    int process_id = -1;
} memoryBlock;

// 初始化内存块
void initialMemoryBlock()
{
    //初始化64个内存块
    for (int i = 0; i < 64; i++)
    {
        memory_block[i].block_id = i;
        memory_block[i].begin = i * 40;
    }
    //初始化64个内存块被进程调度的记录
    process_memory_record.resize(64, -1);
}

//创建进程时分配内存块,返回八个内存块的块号。如果都是-1，则表明内存已满
vector<int> initialBlock_ids(int write_block_id);

//撤销进程时释放内存块
void clearBlock_ids(int clear_process_id);

//返回当前进程对内存块的调度状况
vector<int> getProcessRecord()
{
    return process_memory_record;
}

// 向上传递磁盘提供给QT的磁盘块占用情况
vector<bool> memory_get_disk_block_status()
{
    return disk.get_disk_block_status();
}

// 向上传递磁盘提供给QT的成组链块的情况
vector<int> memory_get_group_block_status()
{
    return disk.get_group_block_status();
}


