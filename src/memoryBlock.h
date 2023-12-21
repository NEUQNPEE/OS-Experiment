#include <iostream>

using namespace std;

// 模拟的分块后的内存块
typedef struct memoryBlock
{
    // 内存块在内存中的起始地址
    int begin = -1;
    // 内存块大小。默认40B
    int size = 40;
    // 访问位
    int status = 0;
    // 存储页号
    int pageNumber = -1;
} memoryBlock;

// 总计64个内存块
memoryBlock block[64];

// 初始化内存块
void initialBlock()
{
    for (int i = 0; i < 64; i++)
    {
        block[i].begin = i * 40;
    }
}