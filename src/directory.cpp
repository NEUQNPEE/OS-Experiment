#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>
std::string gettime()
{
    std::string str = "";
    // 获取当前系统时间点
    auto currentTimePoint = std::chrono::system_clock::now();

    // 将时间点转换为时间类型（time_t）
    std::time_t currentTime = std::chrono::system_clock::to_time_t(currentTimePoint);

    // 使用本地时间
    std::tm localTime;
    if (localtime_s(&localTime, &currentTime) == 0)
    {
        // 将时间信息拼接成字符串
        str = str + std::to_string(localTime.tm_year + 1900) + "年" + std::to_string(localTime.tm_mon + 1) + "月" + std::to_string(localTime.tm_mday) + "日" + std::to_string(localTime.tm_hour) + "时" + std::to_string(localTime.tm_min) + "分" + std::to_string(localTime.tm_sec) + "秒";
    }
    else
    {
        // 错误处理
        std::cerr << "Error in localtime_s" << std::endl;
    }
    return str;
}
struct file; // 文件结构体的声明
struct folder
{
    /*文件夹属性
    类型
    名称
    地址
    大小
    文件数目
    文件夹数目
    创建时间
    修改时间
    下级文件夹
    下级文件
    上级文件夹
    */
    std::string type = "文件夹";
    std::string name;
    std::string addr;
    int size = 0;
    int File_number = 0;
    int Folder_number = 0;
    std::string Create_time;
    std::string Change_time;
    std::vector<folder *> Folder_child;
    std::vector<file *> File_child;
    folder *dad;

    // 构造函数
    folder(std::string Name)
    {
        this->name = Name;
        Create_time = gettime();
        Change_time = gettime();
    }

    // 添加文件定义
    void Add_file(file *fi);

    // 添加文件夹定义
    void Add_folder(folder *fo);

    // 展示属性
    void show()
    {
        std::cout << "类型：" << type << std::endl;
        std::cout << "名称：" << name << std::endl;
        std::cout << "地址：" << addr << std::endl;
        std::cout << "大小：" << size << std::endl;
        std::cout << "文件数量：" << File_number << std::endl;
        std::cout << "文件夹数量：" << Folder_number << std::endl;
        std::cout << "创建时间：" << Create_time << std::endl;
        std::cout << "更改时间：" << Change_time << std::endl;
    }
};
struct file
{
    /*文件属性：
    类型
    名称
    地址
    内容
    大小
    创建时间
    修改时间
    上级文件夹
    */
    std::string type;
    std::string name;
    std::string addr;
    std::string content = "";
    int size = 0;
    std::string Create_time;
    std::string Change_time;
    folder *dad;

    // 构造函数
    file(std::string Type, std::string Name)
    {
        this->type = Type;
        this->name = Name;
        this->Create_time = gettime();
        this->Change_time = gettime();
    }

    // 读取文件
    void read()
    {
        std::cout << "文件内容：" << content << std::endl;
    }

    // 删除文件内容
    void dele()
    {
        this->Change_time = gettime();
        content = "";
        folder *Temp_dad = this->dad;
        while (true)
        {
            if (Temp_dad == Temp_dad->dad)
            {
                Temp_dad->size -= size;
                break;
            }
            else
            {
                Temp_dad->size -= size;
                Temp_dad = Temp_dad->dad;
            }
        }
        this->size = 0;
    }

    // 写入文件内容
    void write(std::string str)
    {
        this->Change_time = gettime();
        content += str;
        folder *Temp_dad = this->dad;
        while (true)
        {
            if (Temp_dad == Temp_dad->dad)
            {
                Temp_dad->size += sizeof(str);
                break;
            }
            else
            {
                Temp_dad->size += sizeof(str);
                Temp_dad = Temp_dad->dad;
            }
        }
        size += sizeof(str);
    }

    // 战术文件属性
    void show()
    {
        std::cout << "类型：" << type << std::endl;
        std::cout << "名称：" << name << std::endl;
        std::cout << "地址：" << addr << std::endl;
        std::cout << "大小：" << size << std::endl;
        std::cout << "创建时间：" << Create_time << std::endl;
        std::cout << "更改时间：" << Change_time << std::endl;
    }
};

// 添加文件实现
void folder::Add_file(file *fi)
{
    File_number++;
    this->size += fi->size;
    if (this == this->dad)
    {
    }
    else
    {
        folder *Temp_dad = this->dad;
        while (true)
        {
            if (Temp_dad == Temp_dad->dad)
            {
                Temp_dad->File_number++;
                Temp_dad->size += fi->size;
                break;
            }
            else
            {
                Temp_dad->File_number++;
                Temp_dad->size += fi->size;
                Temp_dad = Temp_dad->dad;
            }
        }
    }
    this->File_child.push_back(fi);
    fi->dad = this;
    fi->addr = fi->dad->addr + "\\" + fi->name;
}

// 添加文件夹实现
void folder::Add_folder(folder *fo)
{
    Folder_number++;
    this->size += fo->size;
    if (this == this->dad)
    {
    }
    else
    {
        folder *Temp_dad = this->dad;
        while (true)
        {
            if (Temp_dad == Temp_dad->dad)
            {
                Temp_dad->Folder_number++;
                Temp_dad->size += fo->size;
                break;
            }
            else
            {
                Temp_dad->Folder_number++;
                Temp_dad->size += fo->size;
                Temp_dad = Temp_dad->dad;
            }
        }
    }
    this->Folder_child.push_back(fo);
    fo->dad = this;
    fo->addr = fo->dad->addr + "\\" + fo->name;
}

/*
int main()
{
    folder root = folder("root");
    root.dad = &root;
    root.addr = "root";
    root.show();
    folder WOWOS = folder("WOWOS");
    root.Add_folder(&WOWOS);
    root.show();
    WOWOS.show();
    file warn = file("txt", "warn");
    WOWOS.Add_file(&warn);
    root.show();
    WOWOS.show();
    warn.show();
    warn.write("珍爱生命，远离窝窝屎");
    root.show();
    WOWOS.show();
    warn.show();
    warn.read();
    return 0;
}
*/
