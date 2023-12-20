#include <iostream>

using namespace std;

// 模拟内存
typedef struct memory
{
    // 访问位
    int status = 0;
    // 内存块大小，以字节为单位
    int size = 40;
    // 块内页号
    int number = -1;
    // 存储的字符信息
    string info = "";
} memory;