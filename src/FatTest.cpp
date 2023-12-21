#include "FatTest.h"

// 构造函数
FatTest::FatTest() : disk(DISK_SIZE, 0), fatTable(1024, -1)
{
    // 初始化文件分配表
    fatTable[0] = 1; // 第一个块是兑换区

    // 初始化兑换分区块
    for (int i = 1; i < 124; ++i)
    {
        fatTable[i] = 0; // 兑换分区块，标记为已使用
    }

    // 初始化文件区块
    for (int i = 124; i < 1024; ++i)
    {
        fatTable[i] = -1; // 文件区块，初始状态为-1表示未使用
    }
}

// 获取当前磁盘上的空闲块数
int FatTest::getFreeBlocks() const
{
    int count = 0;
    for (int entry : fatTable)
    {
        if (entry == -1)
        {
            count++;
        }
    }
    return count;
}

// 分配一个空闲块并返回其索引
int FatTest::allocateBlock()
{
    for (int i = 124; i < 1024; ++i)
    {
        if (fatTable[i] == -1)
        {
            fatTable[i] = 0; // 标记为已使用
            return i;
        }
    }
    throw std::runtime_error("No free blocks available."); // 没有可用块
}

// 释放给定索引的块
void FatTest::freeBlock(int blockIndex)
{
    if (blockIndex >= 124 && blockIndex < 1024)
    {
        fatTable[blockIndex] = -1; // 标记为未使用
    }
}

// 写入数据到兑换区
void FatTest::writeExchangeArea(int index, const QByteArray &data)
{
    int startPos = index * BLOCK_SIZE;
    int endPos = startPos + BLOCK_SIZE - 1;
    if (endPos >= DISK_SIZE)
    {
        throw std::runtime_error("Beyond disk boundaries!"); // 超出磁盘边界
    }
    for (int i = 0; i < BLOCK_SIZE; ++i)
    {
        disk[startPos + i] = data.at(i);
    }
}

// 从兑换区读取数据
QByteArray FatTest::readExchangeArea(int index) const
{
    QByteArray data;
    int startPos = index * BLOCK_SIZE;
    int endPos = startPos + BLOCK_SIZE - 1;
    if (endPos < DISK_SIZE)
    {
        for (int i = 0; i < BLOCK_SIZE; ++i)
        {
            data.append(disk[startPos + i]); // 逐字节从磁盘读取数据
        }
    }
    return data; // 返回读取的数据
}