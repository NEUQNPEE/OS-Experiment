#include <iostream>
#include <algorithm>
#include "memory.h"
#include "memoryBlock.h"

using namespace std;

// 用于装入文件的八个内存块的块号
int tempBlockNumber[8] = {0};

// 决定由哪八个内存块装入文件
void initialTempBlockNumber()
{
    // 外遍历：装文件的内存块。内遍历：搜索未装页的内存块
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j <= 64; j++)
        {
            if (j == 64)
            {
                cout << "警告!已无空闲内存块!内存溢出!";
                exit(0);
            }
            if (block[j].pageNumber < 0)
            {
                tempBlockNumber[i] = j;
                break;
            }
        }
    }
}

// 将文件的内容填充到内存中。声明见memory.h。具体实现需商讨后完成
void fillMemory(int begin, int size)
{
}

// 全局置换CLOCK算法。雏形已有，实现细节还需商讨后完善
void CLOCK(int page)
{
    // 装入的内存块的块号
    int flag = 0;

    // 查找页是否已在内存里
    bool flag1 = false;
    for (int j = 0; j < 8; j++)
    {
        if (block[tempBlockNumber[j]].pageNumber == page)
        {
            block[tempBlockNumber[j]].status = 1;
            flag = j;
            flag1 = true;
            break;
        }
    }
    if (flag1 == true)
    {
        fillMemory(tempBlockNumber[flag], 40);
        return;
    }

    // 查找是否有没存页的内存块
    bool flag2 = false;
    for (int j = 0; j < 8; j++)
    {
        if (block[tempBlockNumber[j]].pageNumber == -1)
        {
            block[tempBlockNumber[j]].pageNumber = page;
            block[tempBlockNumber[j]].status = 1;
            flag = j;
            flag2 = true;
            break;
        }
    }
    if (flag2 == true)
    {
        fillMemory(tempBlockNumber[flag], 40);
        return;
    }

    // 查找是否有访存位为0的内存块
    bool flag3 = false;
    for (int j = 0; j < 8; j++)
    {
        if (block[tempBlockNumber[j]].status == 0)
        {
            block[tempBlockNumber[j]].pageNumber = page;
            block[tempBlockNumber[j]].status = 1;
            flag = j;
            flag3 = true;
            break;
        }
    }
    if (flag3 == true)
    {
        fillMemory(tempBlockNumber[flag], 40);
        return;
    }

    // 页面置换
    for (int j = 0; j < 8; j++)
    {
        block[tempBlockNumber[j]].status = 0;
    }
    block[tempBlockNumber[0]].pageNumber = page;
    // 文件内容填充到内存中
    fillMemory(tempBlockNumber[0], 40);
}

// 暂定的文件读入到内存的接口，声明见memoryBlock.h。具体实现需商讨后确定
void readFile()
{
    // 假设的文件分块后的页序列，方便演示
    int page[1024] = {0};
    for (int i = 0; i < 1024; i++)
    {
        CLOCK(page[i]);
    }
}

/*
int main()
{
    readFile();
    return 0;
}
*/
