#include <iostream>

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
} memoryBlock;

// 总计64个内存块
memoryBlock memory_block[64];

// 初始化内存块
void initialMemoryBlock()
{
    for (int i = 0; i < 64; i++)
    {
        memory_block[i].block_id = i;
        memory_block[i].begin = i * 40;
    }
}