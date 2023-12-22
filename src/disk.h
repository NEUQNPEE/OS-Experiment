#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

// 磁盘块状态枚举
enum class DiskBlockStatus
{
    FREE,
    ALLOCATED,
    OCCUPIED
};

// 重载输出运算符以输出磁盘块状态
std::ostream &operator<<(std::ostream &os, const DiskBlockStatus &status);

// 磁盘块类
class DiskBlock
{
private:
    int size;
    int number;
    DiskBlockStatus status;
    char *content;

public:
    DiskBlock(int size, int number);
    void set_content(char *content);
    char *get_content() const;
    void set_status(DiskBlockStatus status);
    DiskBlockStatus get_status() const;
    int get_number() const;
};

// 磁盘类
class Disk
{
private:
    int disk_space_size;
    int block_size;
    int block_num;
    int file_area_block_size;
    int exchange_area_block_size;
    std::vector<DiskBlock> blocks;
    std::vector<DiskBlock *> file_area_blocks;
    std::vector<DiskBlock *> exchange_area_blocks;

public:
    Disk(int disk_space_size, int block_size, int file_area_block_size, int exchange_area_block_size);
    void init_blocks();
    int get_disk_size() const;
    int get_block_size() const;
    int get_block_num() const;
    int get_file_area() const;
    int get_exchange_area() const;
    DiskBlock *get_block(int n);
    void str() const;
};

// 磁盘构造器
class DiskBuilder
{
private:
    Disk disk;

public:
    DiskBuilder &set_disk_size(int size);
    DiskBuilder &set_block_size(int size);
    DiskBuilder &set_file_area_block_num(int size);
    DiskBuilder &set_exchange_area_block_num(int size);
    DiskBuilder &init_blocks();
    Disk build();
};

// 磁盘管理类
class DiskManager
{
private:
    Disk disk;
    const int SUPER_BLOCK_INDEX = 0;
    DiskBlock *super_block;
    const int GROUP_BLOCK_SIZE = 128;
    std::vector<bool> file_allocation_table;

    struct GroupBlock
    {
        std::vector<int> block_numbers;
        GroupBlock *next_group_block;
        GroupBlock(int begin, int size);
        void add_block(int block_number);
        void set_next_group_block(GroupBlock *next_group_block);
    };

    GroupBlock *group_block;

public:
    DiskManager();
    void init_disk();
    char *get_super_block_content() const;
    void set_super_block_content(char *content);
    void save_file(const char *file_content);
    void store_content_in_exchange_area(const std::string &content, int block_number);
    void store_contents_in_exchange_area(const std::vector<std::string> &contents, int start_block_number);
    std::pair<std::string, int> retrieve_content_from_exchange_area(int block_number);
    std::vector<std::pair<std::string, int>> retrieve_contents_from_exchange_area(int start_block_number, int num_blocks);
};
