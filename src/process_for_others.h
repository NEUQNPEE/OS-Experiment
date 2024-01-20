/*
 * Created Date: Thursday January 18th 2024
 * Author: JiuYou2020
 * Email: JiuYou2020@gmail.com
 * Company: Personal Coder
 * Description: 为其它系统提供的接口，应该只有前端
 * Copyright (c) 2024 Personal Coder
 */

#include "process_manager.h"

/**
 * 创建进程，共需两步
 * 1. 调用create_process创建进程,并进行强制类型转换为需要创建的进程类型，有以下几种类型：
 * 1.1 DataGenerationProcess 数据生成进程
 * 1.2 DataDeletionProcess 数据删除进程
 * 1.3 ExecutionProcess 执行进程
 * 1.4 InitProcess 初始化进程
 * 2. 根据需要创建的进程类型，进行参数传递，至此进程创建完成
 * @param type 需要创建的进程类型
 * @return 创建完成的进程，需要进行强制类型转换
 */
Process *create_process(ProcessType type);

/**
 * 传递参数给初始化进程
 * @param process  进程
 * @param priority  优先级
 */
void pass_args_to_init_process(InitProcess *process, int priority);

/**
 * 传递参数给数据生成进程
 * @param process 进程
 * @param priority 优先级
 * @param command  操作指令
 * @param fName  文件/文件夹名
 * @param path 文件/文件夹路径
 */
void pass_args_to_generation_process(DataGenerationProcess *process, int priority, OperationCommand command,
                                     std::string fName, std::string path);

/**
 * 传递参数给数据删除进程
 * @param process  进程
 * @param priority  优先级
 * @param command  操作指令
 * @param fName  文件/文件夹名
 * @param path 文件/文件夹路径
 * @return
 */
bool pass_args_to_deletion_process(DataDeletionProcess *process, int priority, OperationCommand command,
                                   std::string fName, std::string path);

/**
 * 传递参数给执行进程
 * @param process 进程
 * @param priority  优先级
 * @param command  操作指令
 * @param fName  文件/文件夹名
 * @param path 文件/文件夹路径
 * @param blockNum  需要分配的磁盘块数
 */
void pass_args_to_execution_process(ExecutionProcess *process, int priority, OperationCommand command,
                                    std::string fName, std::string path, int blockNum);

/**
 * 执行读操作
 * @param path 读取的文件路径
 * @param pid  负责读取的进程pid
 * @return  读取的内容
 */
std::string execute_read(std::string path, int pid);

/**
 * 执行写操作
 * @param path 写入的文件路径
 * @param pid  负责写入的进程pid
 * @param content 写入的内容
 * @return  是否写入成功
 */
bool execute_write(std::string path, int pid, std::string content);

/**
 * 展示当前进程对内存块的调度状况
 * @return 当前进程对内存块的调度状况
 */
vector<int> show_process_record();

/**
 * 展示当前磁盘块的占用情况
 * @return  当前磁盘块的占用情况
 */
vector<bool> show_disk_block_status();

/**
 * 展示当前成组链块的情况
 * @return  当前成组链块的情况
 */
vector<int> show_group_block_status();

/**
 * 获取当前进程的集合
 * @return  当前进程的集合
 */
vector<Process *> get_process_map();

/**
 * 进行线程调度
 */
void schedule();

/**
 * 结束进程系统
 */
void end();