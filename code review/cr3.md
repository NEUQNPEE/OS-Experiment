# cr3：进程系统

## 功能完整性

新功能待补充

## 可读性

尚可。存在某些长得很奇怪的函数，比如：
    
```c++

void ExecutionProcess::execute_write(File *file, ExecutionProcess *executionProcess){
    file_change_content(file, *executionProcess->fileInfo->data, executionProcess->pid);
    vector pids =look_open_file(file);
    if(pids.size()>1){
        ExecutionProcess::sendData(to_string(executionProcess->pid), *executionProcess->fileInfo->data);
        for (int pid : pids) {
            if(pid!=executionProcess->pid){
                //从进程列表中找到该进程
                for (auto & j : processManager.processList) {
                    if(j->pid==pid){
                        auto* executionProcess1=dynamic_cast<ExecutionProcess*>(j);
                        executionProcess1->renew(to_string(executionProcess->pid),executionProcess1);
                    }
                }
            }
        }
    }
}

```

注意调整其缩进层数

## 可维护性

一般。尚需文件拆分，和部分函数式编程

## 可扩展性

较好。能较为方便的扩展新类型的进程与操作

## 可测试性

等待测试模块的编写

## 代码健壮性

主要在参数传递上。等换为字符串并加入判断后会更好，

## 2024年1月9日 会议记录

### 1. 前后端传递参数问题

将现有的结构体传递全面转为字符串传递

### 2. BIOS文件

要有一个自己编写的类似BIOS文件，操作系统初始化将该文件读入，保存于bios.cpp,提供各级功能所使用的大量共享的，只读的全局变量

### 3. 虚拟内存

在解决内存目前的问题之后，基于 2. 中的BIOS文件，已经得知虚拟空间大小，因此实现虚拟内存。之后进程申请内存直接申请大小，内存系统返回虚拟地址，进程使用虚拟地址进行操作，内存系统将虚拟地址转换为物理地址进行操作。

### 4. 进程间通信

命名管道交给文件系统，以特殊文件子类实现。

### 5. 模拟性的时间片调度

一是qt前端的动态刷新，二是时间片轮转，都需要引入时间机制，寻找qt的解决方案。

### 6. 其他细节功能上的修改等。
