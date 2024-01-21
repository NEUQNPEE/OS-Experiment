/*
 * Created Date: Thursday January 4th 2024
 * Author: ArmouredEvil
 * Email: dengpersonal123@gmail.com
 * Company: Personal Coder
 * Description: 为进程系统提供的接口
 * Copyright (c) 2024 Personal Coder
 */
#include <iostream>
#include <vector>

//using namespace std;

//创建进程时分配内存块,返回分配内存块的块号。如果都是-1，则表明内存已满
std::vector<int> assignMemoryBlock(int process_id, int block_sum);

//撤销进程时释放内存块
void clearMemoryBlock(int clear_process_id);

//返回当前进程对内存块的调度状况
std::vector<int> getProcessRecord();

// 向上传递磁盘提供给QT的磁盘块占用情况
std::vector<bool> memory_get_disk_block_status();

// 向上传递磁盘提供给QT的成组链块的情况
std::vector<int> memory_get_group_block_status();

