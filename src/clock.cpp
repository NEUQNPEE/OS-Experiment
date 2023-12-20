#include <iostream>
#include <algorithm>
#include "memory.h"

using namespace std;

// 模拟的给文件分配的8个内存块，详见Memory.h。具体实现需要商讨后确定
memory mem[8];

// 模拟的放入内存的文件分块后的页序列。具体实现需要商讨后确定
int page[1024];

// 暂定的文件页接口。具体实现需商讨后确定
void initialPage()
{
    for (int i = 0; i < 1024; i++)
    {
        page[i] = i % 1024;
    }
}

// 暂定的内存接口。具体实现需商讨后确定
void initialMemory()
{
    fill(mem, mem + 8, 0);
}

void CLOCK()
{
    for (int i = 0; i < 1024; i++)
    {
        // 查找页是否已在内存里
        bool flag1 = false;
        for (int j = 0; j < 8; j++)
        {
            if (mem[j].number == page[i])
            {
                mem[j].status = 1;
                flag1 = true;
                break;
            }
        }
        if (flag1 == true)
        {
            continue;
        }

        // 查找是否有没存页的内存块
        bool flag2 = false;
        for (int j = 0; j < 8; j++)
        {
            if (mem[j].number == -1)
            {
                mem[j].number = page[i];
                mem[j].status = 1;
                flag2 = true;
                break;
            }
        }
        if (flag2 == true)
        {
            continue;
        }

        // 查找是否有访存位为0的内存块
        bool flag3 = false;
        for (int j = 0; j < 8; j++)
        {
            if (mem[j].status == 0)
            {
                mem[j].number = page[i];
                mem[j].status = 1;
                flag3 = true;
                break;
            }
        }
        if (flag3 == true)
        {
            continue;
        }

        // 页面置换
        for (int j = 0; j < 8; j++)
        {
            mem[j].status = 0;
        }
        mem[0].number = page[i];
    }
}

/*
int main()
{
    initialPage();
    CLOCK();
    return 0;
}
*/