#include "tree_directory.h"
Folder root = Folder("root");
int ID_number = 1000;
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
// 读文件属性函数
int File::get_ID()
{
    return this->ID;
}
std::string File::get_Name()
{
    return this->name;
}
std::string File::get_Type()
{
    return this->type;
}
std::string File::get_Addr()
{
    return this->addr;
}
int File::get_Size()
{
    return this->size;
}
std::string File::get_Create_time()
{
    return this->Create_time;
}
std::string File::get_Change_time()
{
    return this->Change_time;
}
std::string File::get_Content()
{
    return this->content;
}
Folder *File::get_Dad()
{
    return this->dad;
}

// 写文件属性函数

// 写名称
void File::set_Name_Type(std::string str)
{
    this->addr = this->addr.replace((this->addr.rfind('\\') + 1), this->name.size(), str);
    this->name = str;
    this->type = this->name.substr(this->name.find('.') + 1);
    this->Change_time = gettime();
}

// 设置标识符
void File::set_ID()
{
    this->ID = ID_number;
    ID_number++;
}

// 写内容
void File::set_Content(std::string str)
{
    std::string time = gettime();
    this->Change_time = time;
    int n = str.size();
    n -= content.size();
    this->content = str;
    this->size += n;
    Folder *Temp_dad = this->dad;
    while (true)
    {
        if (Temp_dad == Temp_dad->get_Dad())
        {
            Temp_dad->change_Change_time(time);
            Temp_dad->change_size(n);
            break;
        }
        else
        {
            Temp_dad->change_Change_time(time);
            Temp_dad->change_size(n);
            Temp_dad = Temp_dad->get_Dad();
        }
    }
}

// 文件属性改变
void File::change_Dad(Folder *folder)
{
    this->dad = folder;
}

void File::change_Addr(std::string str)
{
    this->addr = str;
}

// 构造函数
File::File(std::string str)
{
    this->name = str;
    this->Create_time = gettime();
    this->Change_time = gettime();
}

// 拷贝文件
File *File::copy_File()
{
    File file = File();
    file.name = this->name;
    file.type = this->type;
    file.size = this->size;
    file.content = this->content;
    return &file;
}

// 删除文件
void File::delete_File()
{
    std::string time = gettime();
    int n = this->size * (-1);
    Folder *Temp_dad = this->dad;
    Temp_dad->change_File_child(false, this);
    while (true)
    {
        if (Temp_dad == Temp_dad->get_Dad())
        {
            Temp_dad->change_File_number(-1);
            Temp_dad->change_Change_time(time);
            Temp_dad->change_size(n);
            break;
        }
        else
        {
            Temp_dad->change_File_number(-1);
            Temp_dad->change_Change_time(time);
            Temp_dad->change_size(n);
            Temp_dad = Temp_dad->get_Dad();
        }
    }
}

// 文件属性序列化
std::string File::file_serialize()
{
    std::string str = "";
    str += this->addr + "$";
    str += this->name + "$";
    str += this->Create_time + "$";
    str += this->Change_time + "$";
    str += this->size + "$";
    return str;
}

// 文件属性反序列化
void File::file_deserialize(std::string str)
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
    this->addr = str1[0];
    this->name = str1[1];
    this->Create_time = str1[2];
    this->Change_time = str1[3];
    this->size = std::stoi(str1[4]);
    // 用type截取name中'.'后面部分的内容
    this->type = this->name.substr(this->name.find('.') + 1);
}

// 文件内容序列化
std::string File::file_con_serialize()
{
    std::string str = "";
    str += std::to_string(this->ID) + "$";
    str += this->content + "$";
    return str;
}

// 文件内容反序列化
void File::file_con_deserialize(std::string str)
{
    this->content = str;
}

// 展示文件属性
void File::show()
{
    std::cout << "类型：" << type << std::endl;
    std::cout << "名称：" << name << std::endl;
    std::cout << "地址：" << addr << std::endl;
    std::cout << "大小：" << size << std::endl;
    std::cout << "创建时间：" << Create_time << std::endl;
    std::cout << "更改时间：" << Change_time << std::endl;
    std::cout << "内容：" << content << std::endl;
}

// 读文件夹内容函数
std::string Folder::get_Name()
{
    return this->name;
}
std::string Folder::get_Type()
{
    return this->type;
}
std::string Folder::get_Addr()
{
    return this->addr;
}
int Folder::get_Size()
{
    return this->size;
}
int Folder::get_File_number()
{
    return this->File_number;
}
int Folder::get_Folder_number()
{
    return this->Folder_number;
}
std::string Folder::get_Create_time()
{
    return this->Create_time;
}
std::string Folder::get_Change_time()
{
    return this->Change_time;
}
std::vector<Folder *> Folder::get_Folder_child()
{
    return this->Folder_child;
}
std::vector<File *> Folder::get_File_child()
{
    return this->File_child;
}
Folder *Folder::get_Dad()
{
    return this->dad;
}

// 写文件夹名称
void Folder::set_Name(std::string str)
{
    std::string old_addr = this->addr;
    this->addr = this->addr.replace((this->addr.rfind('\\') + 1), this->name.size(), str);
    std::string new_addr = this->addr;
    std::queue<Folder *> q;
    q.push(this);
    while (q.empty() == false)
    {
        Folder *folder = q.front();
        q.pop();
        int folder_num = folder->get_Folder_child().size();
        int file_num = folder->get_File_child().size();
        for (int i = 0; i < folder_num; i++)
        {
            Folder *folder_child_folder = folder->get_Folder_child()[i];
            folder_child_folder->change_Addr(folder_child_folder->get_Addr().replace(0, old_addr.size(), new_addr));
            q.push(folder_child_folder);
        }
        for (int i = 0; i < file_num; i++)
        {
            File *folder_child_file = folder->get_File_child()[i];
            folder_child_file->change_Addr(folder_child_file->get_Addr().replace(0, old_addr.size(), new_addr));
        }
    }
    this->name = str;
    this->Change_time = gettime();
}
void Folder::change_size(int n)
{
    this->size += n;
}
void Folder::change_File_number(int n)
{
    this->File_number += n;
}
void Folder::change_Folder_number(int n)
{
    this->Folder_number += n;
}
void Folder::change_Change_time(std::string str)
{
    this->Change_time = str;
}
void Folder::change_Folder_child(bool flag, Folder *folder)
{
    if (flag == true)
    {
        this->Folder_child.push_back(folder);
    }
    else
    {
        for (int i = 0; i < this->Folder_child.size(); i++)
        {
            if (this->Folder_child[i] == folder)
            {
                this->Folder_child.erase(this->Folder_child.begin() + i);
                break;
            }
        }
    }
}
void Folder::change_File_child(bool flag, File *file)
{
    if (flag == true)
    {
        this->File_child.push_back(file);
    }
    else
    {
        for (int i = 0; i < this->File_child.size(); i++)
        {
            if (this->File_child[i] == file)
            {
                this->File_child.erase(this->File_child.begin() + i);
            }
        }
    }
}

void Folder::change_Dad(Folder *folder)
{
    this->dad = folder;
}
void Folder::change_Addr(std::string str)
{
    this->addr = str;
}

// 无参构造函数
Folder::Folder(std::string str)
{
    this->name = str;
    this->dad = this;
    this->addr = str;
    Create_time = gettime();
    Change_time = gettime();
}

// 拷贝文件夹
Folder *Folder::copy_Folder()
{
    Folder folder = Folder();
    folder.set_Name(this->name);
    folder.change_Dad(&folder);
    for (int i = 0; i < this->get_Folder_child().size(); i++)
    {
        folder.Add_folder(this->get_Folder_child()[i]->copy_Folder());
    }
    for (int i = 0; i < this->get_File_child().size(); i++)
    {
        folder.Add_file(this->get_File_child()[i]->copy_File());
    }
    return &folder;
}

// 添加文件
void Folder::Add_file(File *file, int flag)
{
    if (flag == 1)
    {
        std::string time = gettime();
        int n = file->get_Size();
        this->change_Change_time(time);
        this->change_File_number(1);
        this->change_size(n);
        if (this == this->dad)
        {
        }
        else
        {
            Folder *Temp_dad = this->dad;
            while (true)
            {
                if (Temp_dad == Temp_dad->dad)
                {
                    Temp_dad->change_Change_time(time);
                    Temp_dad->change_File_number(1);
                    Temp_dad->change_size(n);
                    break;
                }
                else
                {
                    Temp_dad->change_Change_time(time);
                    Temp_dad->change_File_number(1);
                    Temp_dad->change_size(n);
                    Temp_dad = Temp_dad->get_Dad();
                }
            }
        }
        this->File_child.push_back(file);
        file->change_Dad(this);
        file->change_Addr(this->addr + '\\' + file->get_Name());
    }
    else
    {
        int n = file->get_Size();
        this->change_File_number(1);
        this->change_size(n);
        if (this == this->dad)
        {
        }
        else
        {
            Folder *Temp_dad = this->dad;
            while (true)
            {
                if (Temp_dad == Temp_dad->get_Dad())
                {
                    Temp_dad->change_File_number(1);
                    Temp_dad->change_size(n);
                    break;
                }
                else
                {
                    Temp_dad->change_File_number(1);
                    Temp_dad->change_size(n);
                    Temp_dad = Temp_dad->get_Dad();
                }
            }
        }
        this->File_child.push_back(file);
        file->change_Dad(this);
        file->change_Addr(this->addr + '\\' + file->get_Name());
        file->set_ID();
    }
}

// 添加文件夹
void Folder::Add_folder(Folder *folder, int flag)
{
    if (flag == 1)
    {
        std::string time = gettime();
        int n = folder->get_Size();
        int file_num = folder->get_File_number();
        int folder_num = folder->get_Folder_number() + 1;
        this->change_Change_time(time);
        this->change_File_number(file_num);
        this->change_Folder_number(folder_num);
        this->change_size(n);
        if (this == this->dad)
        {
        }
        else
        {
            Folder *Temp_dad = this->dad;
            while (true)
            {
                if (Temp_dad == Temp_dad->dad)
                {
                    Temp_dad->change_Change_time(time);
                    Temp_dad->change_File_number(file_num);
                    Temp_dad->change_Folder_number(folder_num);
                    Temp_dad->change_size(n);
                    break;
                }
                else
                {
                    Temp_dad->change_Change_time(time);
                    Temp_dad->change_File_number(file_num);
                    Temp_dad->change_Folder_number(folder_num);
                    Temp_dad->change_size(n);
                    Temp_dad = Temp_dad->get_Dad();
                }
            }
        }
        this->Folder_child.push_back(folder);
        folder->change_Dad(this);
        folder->change_Addr(this->addr + '\\' + folder->get_Name());
        std::queue<Folder *> q;
        q.push(folder);
        while (q.empty() == false)
        {
            Folder *Temp_folder = q.front();
            q.pop();
            int folder_num = Temp_folder->get_File_child().size();
            int file_num = Temp_folder->get_File_child().size();
            for (int i = 0; i < folder_num; i++)
            {
                q.push(get_Folder_child()[i]);
                Temp_folder->get_Folder_child()[i]->change_Addr(dad->addr + '\\' + get_Folder_child()[i]->get_Name());
            }
            for (int i = 0; i < file_num; i++)
            {
                Temp_folder->get_File_child()[i]->change_Addr(dad->addr + '\\' + get_File_child()[i]->get_Name());
            }
        }
    }
    else
    {
        int n = folder->get_Size();
        int file_num = folder->get_File_number();
        int folder_num = folder->get_Folder_number() + 1;
        this->change_File_number(file_num);
        this->change_Folder_number(folder_num);
        this->change_size(n);
        if (this == this->dad)
        {
        }
        else
        {
            Folder *Temp_dad = this->dad;
            while (true)
            {
                if (Temp_dad == Temp_dad->dad)
                {
                    Temp_dad->change_File_number(file_num);
                    Temp_dad->change_Folder_number(folder_num);
                    Temp_dad->change_size(n);
                    break;
                }
                else
                {
                    Temp_dad->change_File_number(file_num);
                    Temp_dad->change_Folder_number(folder_num);
                    Temp_dad->change_size(n);
                    Temp_dad = Temp_dad->get_Dad();
                }
            }
        }
        this->Folder_child.push_back(folder);
        folder->change_Dad(this);
        folder->change_Addr(this->addr + '\\' + folder->get_Name());
    }
}

// 删除文件夹
void Folder::delete_folder()
{
    std::string time = gettime();
    int n = this->get_Size() * (-1);
    int file_num = this->get_File_number() * (-1);
    int folder_num = this->get_Folder_number() * (-1) - 1;
    Folder *Temp_dad = this->dad;
    Temp_dad->change_Folder_child(false, this);
    while (true)
    {
        if (Temp_dad == Temp_dad->dad)
        {
            Temp_dad->change_Change_time(time);
            Temp_dad->change_File_number(file_num);
            Temp_dad->change_Folder_number(folder_num);
            Temp_dad->change_size(n);
            break;
        }
        else
        {
            Temp_dad->change_Change_time(time);
            Temp_dad->change_File_number(file_num);
            Temp_dad->change_Folder_number(folder_num);
            Temp_dad->change_size(n);
            Temp_dad = Temp_dad->get_Dad();
        }
    }
}

// 文件夹序列化
std::string Folder::folder_serialize()
{
    std::string str = "";
    str += this->addr + '$';
    str += this->name + "$";
    str += this->Create_time + "$";
    str += this->Change_time + "$";
    return str;
}

// 文件夹反序列化
void Folder::folder_deserialize(std::string str)
{
    std::string str1[4] = {};
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
    this->addr = str1[0];
    this->name = str1[1];
    this->Create_time = str1[2];
    this->Change_time = str1[3];
}

// 展示文件夹属性
void Folder::show()
{
    std::cout << "类型：" << this->type << std::endl;
    std::cout << "名称：" << name << std::endl;
    std::cout << "地址：" << addr << std::endl;
    std::cout << "大小：" << size << std::endl;
    std::cout << "文件数：" << File_number << std::endl;
    std::cout << "文件夹数：" << Folder_number << std::endl;
    std::cout << "创建时间：" << Create_time << std::endl;
    std::cout << "更改时间：" << Change_time << std::endl;
}

// 文件内容序列化生成
void file_con_ser_gen(Folder *folder, std::string *str)
{
    int folder_num = folder->get_File_child().size();
    int file_num = folder->get_Folder_child().size();
    if (folder_num == 0 && file_num == 0)
    {
    }
    else
    {
        for (int i = 0; i < folder_num; i++)
        {
            file_con_ser_gen(folder->get_Folder_child()[i], str);
        }
        for (int i = 0; i < file_num; i++)
        {
            *str += folder->get_File_child()[i]->file_con_serialize();
        }
    }
}

// 文件内容序列化
std::string file_con_ser()
{
    std::string str;
    file_con_ser_gen(&root, &str);
    return str;
}

// 树形目录序列化生成
void tree_dir_ser_gen(std::string *str)
{
    std::queue<int> child_num;
    std::queue<int> type;
    std::queue<std::string> inform;
    std::queue<Folder *> q;
    q.push(&root);
    child_num.push(root.get_Folder_child().size() + root.get_File_child().size());
    type.push(1);
    inform.push(root.folder_serialize());
    while (q.empty() == false)
    {
        Folder *folder = q.front();
        q.pop();
        int folder_num = folder->get_Folder_child().size();
        int file_num = folder->get_Folder_child().size();
        for (int i = 0; i < folder_num; i++)
        {
            Folder *folder_child_folder = folder->get_Folder_child()[i];
            child_num.push(folder_child_folder->get_Folder_child().size() + folder_child_folder->get_File_child().size());
            type.push(1);
            inform.push(folder_child_folder->folder_serialize());
            q.push(folder_child_folder);
        }
        for (int i = 0; i < file_num; i++)
        {
            File *folder_child_file = folder->get_File_child()[i];
            child_num.push(0);
            type.push(0);
            inform.push(folder_child_file->file_serialize());
        }
    }
    while (type.empty() == false)
    {
        *str += std::to_string(type.front()) + "$" + std::to_string(child_num.front()) + "$" + inform.front() + "$";
        type.pop();
        child_num.pop();
        inform.pop();
    }
}

// 树形目录序列化
std::string tree_dir_ser()
{
    std::string str;
    tree_dir_ser_gen(&str);
    return str;
}

// 树形目录反序列化生成
void tree_dir_diser_gen(std::queue<int> type, std::queue<int> child_num, std::queue<std::string> inform)
{
    std::queue<Folder *> q;
    std::queue<int> q_num;
    root.folder_deserialize(inform.front());
    q.push(&root);
    q_num.push(child_num.front());
    type.pop();
    child_num.pop();
    inform.pop();
    while (q.empty() == false)
    {
        int n = q_num.front();
        q_num.pop();
        for (int i = 0; i < n; i++)
        {
            if (type.front() == 1)
            {
                type.pop();
                Folder folder_child_folder = Folder();
                folder_child_folder.folder_deserialize(inform.front());
                inform.pop();
                q.front()->Add_folder(&folder_child_folder, 0);
                q.push(&folder_child_folder);
                q_num.push(child_num.front());
                child_num.pop();
            }
            else
            {
                type.pop();
                child_num.pop();
                File folder_child_file = File();
                folder_child_file.file_deserialize(inform.front());
                inform.pop();
                q.front()->Add_file(&folder_child_file, 0);
            }
        }
    }
}

// 树形目录反序列化
void tree_dir_diser(std::string str)
{
    std::vector<std::string> ser_inform;
    for (int i = 0; i < str.size();)
    {
        for (int j = i; j < str.size(); j++)
        {
            if (str[j] == '$' && str[j + 1] == '$')
            {
                ser_inform.push_back(str.substr(i, j - i));
                std::cout << str.substr(i, j - i) << std::endl;
                i = j + 2;
                break;
            }
        }
    }
    std::queue<int> type;
    std::queue<int> child_num;
    std::queue<std::string> inform;
    for (int i = 0; i < ser_inform.size(); i++)
    {
        int n = 0;
        std::string str1 = "";
        std::string str2 = "";
        std::string str3 = "";
        for (int j = 0; j < ser_inform[i].size(); j++)
        {
            if (n < 2)
            {
                if (ser_inform[i][j] == '$')
                {
                    n++;
                }
                else
                {
                    if (n == 0)
                    {
                        str1 += ser_inform[i][j];
                    }
                    else
                    {
                        str2 += ser_inform[i][j];
                    }
                }
            }
            else
            {
                str3 += ser_inform[i][j];
            }
        }
        type.push(std::stoi(str1));
        child_num.push(std::stoi(str2));
        inform.push(str3);
    }
}