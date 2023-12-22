#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include <sstream>
#include <queue>
std::vector<int> tree_number;
std::vector<std::string> tree_ser_dir;
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
struct File; // 文件结构体的声明
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
    std::string name;
    std::string type = "文件夹";
    std::string addr;
    int size = 0;
    int File_number = 0;
    int Folder_number = 0;
    std::string Create_time;
    std::string Change_time;
    std::vector<folder *> Folder_child;
    std::vector<File *> File_child;
    folder *dad;
    // 构造函数
    folder()
    {
        Create_time = gettime();
        Change_time = gettime();
    }

    // 添加文件定义
    void Add_file(File *fi);

    // 添加文件夹定义
    void Add_folder(folder *fo);

    // 展示属性
    /*
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
    */

    // 文件夹序列化
    /*
    名称
    地址
    创建时间
    更改时间
    内容
    */
    std::string folder_serialize();

    // 文件夹反序列化
    void folder_deserialize(std::string str);
};
struct File
{
    /*文件属性：
    名称
    类型
    地址
    内容
    大小
    创建时间
    修改时间
    上级文件夹
    */
    std::string name;
    std::string type;
    std::string addr;
    int size = 0;
    std::string Create_time;
    std::string Change_time;
    std::string content = "";
    folder *dad;

    // 构造函数
    File()
    {
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

    // 文件序列化
    /*
    名称
    地址
    创建时间
    更改时间
    内容
    */
    std::string file_serialize()
    {

        std::string str = "";
        str += name + "$";
        str += addr + "$";
        str += Create_time + "$";
        str += Change_time + "$";
        str += content + "$";
        return str;
    }
    // 文件反序列化
    File file_deserialize(std::string str)
    {
        std::string str1[5] = {};
        int n = 0;
        for (int i = 0; i < str.size(); i++)
        {
            if (str[i] == '$')
            {
                n++;
            }
            else
            {
                str1[n] += str[i];
            }
        }
        File file = File();
        file.name = str1[0];
        file.addr = str1[1];
        file.Create_time = str1[2];
        file.Change_time = str1[3];
        file.content = str1[4];
        file.size = sizeof(file.content);
        // 用type截取name中'.'后面部分的内容
        file.type = file.name.substr(file.name.find('.') + 1);
        return file;
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

folder root = folder();
// 初始化根目录
void init_directory()
{
    root.name = "root";
    root.dad = &root;
    root.addr = "root";
}
// 子目录数生成
void cdir_num_generate(folder fo = root)
{
    if (fo.Folder_child.size() == 0 && fo.File_child.size() == 0)
    {
        tree_number.push_back(0);
        tree_number.push_back(1);
        return;
    }
    else
    {
        for (int i = 0; i < fo.Folder_child.size(); i++)
        {
            tree_number.push_back(fo.Folder_child.size() + fo.File_child.size());
            tree_number.push_back(1);
        }
        for (int i = 0; i < fo.File_child.size(); i++)
        {
            tree_number.push_back(0);
            tree_number.push_back(0);
        }
    }
    for (int i = 0; i < fo.Folder_child.size(); i++)
    {
        cdir_num_generate(*(fo.Folder_child[i]));
    }
}
// 子目录数序列化
std::string cdir_num_serialize()
{
    std::string str;
    for (int i = 0; i < tree_number.size(); i++)
    {
        str += std::to_string(tree_number[i]) + '$';
    }
    return str;
}
// 树形目录生成
void tree_generate(folder fo = root)
{
    for (int i = 0; i < fo.Folder_child.size(); i++)
    {
        tree_ser_dir.push_back((*(fo.Folder_child[i])).folder_serialize());
    }
    for (int i = 0; i < fo.File_child.size(); i++)
    {
        tree_ser_dir.push_back((*(fo.File_child[i])).file_serialize());
    }
    for (int i = 0; i < fo.Folder_child.size(); i++)
    {
        tree_generate(*(fo.Folder_child[i]));
    }
}

// 树形目录序列化
std::string tree_serialize()
{
    std::string str = root.folder_serialize() + "$$";
    for (int i = 0; i < tree_ser_dir.size(); i++)
    {
        str += tree_ser_dir[i] + "$$";
    }
    return str;
}
// 文件和文件夹按照树形目录反序列化
void fileOrFilder_deserialize(std::queue<int> arr1, std::queue<int> arr2, std::queue<std::string> str)
{
    std::queue<folder> q;
    while (arr1.empty() == false)
    {
        int n = arr1.front();
        arr1.pop();
        if (arr2.front() == 1)
        {
            arr2.pop();
            q.push(folder());
            folder fo = q.front();
            fo.folder_deserialize(str.front());
            str.pop();
            for (int i = 0; i < n; i++)
            {
                if (arr2.front() == 1)
                {
                    arr2.pop();
                    folder fo_child_folder = folder();
                    q.push(fo_child_folder);
                    fo_child_folder.folder_deserialize(str.front());
                    str.pop();
                    fo.Add_folder(&fo_child_folder);
                }
                else
                {
                    arr2.pop();
                    File fi_child_file = File();
                    fi_child_file.file_deserialize(str.front());
                    str.pop();
                    fo.Add_file(&fi_child_file);
                }
            }
            q.pop();
        }
        else
        {
        }
    }
}
// 树形目录的反序列化
void tree_deserialize(std::string tree_str, std::string f_str)
{
    std::queue<int> arr;
    std::queue<int> arr1;
    std::queue<int> arr2;
    int n = 0;
    // 使用字符串流分割字符串
    std::istringstream iss(tree_str);
    std::string number;
    // 通过'$'分割字符串并将每个数字字符串转换成整数
    while (std::getline(iss, number, '$'))
    {
        // 将字符串转换成整数并存入容器
        int num;
        std::istringstream(number) >> num;
        arr.push(num);
    }
    while (arr.empty() == false)
    {
        if (n % 2 == 0)
        {
            arr1.push(arr.front());
            arr.pop();
        }
        else
        {
            arr2.push(arr.front());
            arr.pop();
        }
    }
    std::queue<std::string> str;
    for (int i = 0; i < f_str.size();)
    {
        for (int j = i; j < f_str.size(); j++)
        {
            if (f_str[j] == '$' && f_str[i + 1] == '$')
            {
                str.push(f_str.substr(i, j - i));
                i = j + 2;
                break;
            }
        }
    }
    // 文件或文件夹反序列化
    fileOrFilder_deserialize(arr1, arr2, str);
}
// 添加文件实现
void folder::Add_file(File *fi)
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

// 文件夹序列化实现
std::string folder::folder_serialize()
{

    std::string str = "";
    str += name + "$";
    str += addr + "$";
    str += Create_time + "$";
    str += Change_time + "$";
    return str;
}

// 文件夹反序列化实现
void folder::folder_deserialize(std::string str)
{
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
