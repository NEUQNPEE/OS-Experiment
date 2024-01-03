#include <iostream>
#include <vector>

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
    //所属的进程的进程ID
    int process_id = -1;
} memoryBlock;

// 总计64个内存块
memoryBlock memory_block[64];

// 用于当前进程的八个内存块的块号
int block_ids[8];

// 正在装入内存的文件分块后的各文件页。页最多为1024（40KB/40B）
string page_content[1024];

//当前进程调度内存块状况记录。
//存储的string是第n次页面调度后当前进程在8个内存块中的内容。不同内存块的内容用换行符隔开
vector<string> clock_record;


// 初始化内存块
void initialMemoryBlock()
{
    for (int i = 0; i < 64; i++)
    {
        memory_block[i].block_id = i;
        memory_block[i].begin = i * 40;
    }
}

//创建进程时分配内存块,返回值标明是否分配成功
bool initialBlock_ids(int process_id);

//撤销进程时释放内存块
void clearBlock_ids(int process_id);

//返回当前进程对内存块的调度状况
vector<string> getProcessRecord()
{
    vector<string> temp = clock_record;
    //清空当前进程调度内存块的记录，以便下一个进程使用
    clock_record.clear();
    return temp;
}
