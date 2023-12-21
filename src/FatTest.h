#ifndef FATTEST_H
#define FATTEST_H

#include <QVector> // 引入Qt的向量容器

class FatTest
{
public:
    // 常量定义：磁盘大小为40KB，每块大小为40B
    static const int DISK_SIZE = 40 * 1024; // 40KB
    static const int BLOCK_SIZE = 40;       // 每块大小40B

    FatTest(); // 构造函数

    // 获取当前磁盘上的空闲块数
    int getFreeBlocks() const;

    // 分配一个空闲块并返回其索引
    int allocateBlock();

    // 释放给定索引的块
    void freeBlock(int blockIndex);

    // 写入到兑换区
    void writeExchangeArea(int index, const QByteArray &data);

    // 从兑换区读取数据
    QByteArray readExchangeArea(int index) const;

private:
    QVector<char> disk;    // 模拟磁盘的数据存储
    QVector<int> fatTable; // 文件分配表，用于跟踪哪些块被使用或空闲
};

#endif