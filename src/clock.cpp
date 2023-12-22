#include <iostream>
#include <algorithm>
#include "memory.h"
#include "memoryBlock.h"

using namespace std;
/*
* 文件管理系统才知道一个文件使用那八个内存块
*/

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
        fillMemory(block[tempBlockNumber[flag]].begin, 40);
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
        fillMemory(block[tempBlockNumber[flag]].begin, 40);
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
        fillMemory(block[tempBlockNumber[flag]].begin, 40);
        return;
    }

    // 页面置换
    for (int j = 0; j < 8; j++)
    {
        block[tempBlockNumber[j]].status = 0;
    }
    block[tempBlockNumber[0]].pageNumber = page;
    // 文件内容填充到内存中
    fillMemory(block[tempBlockNumber[0]].begin, 40);
}


// 这个是文件管理系统调用的函数，把用户修改过的新的文件数据传回内存
void WriteFile(int file_id,std::string file_corrent)
{

}

// 文件管理系统已经知道文件所使用的内存块，调用这个
// 文件管理系统从内存读出某个内存块，传入一个块号，返回一个内存块的内容
char* ReadMemoryBolcks(int* block_ids)
{

}

char* ReadMemoryBolck(int block_id)
{

}

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

/*
int main()
{
    readFile();
    return 0;
}
*/
