#include "tree_directory.h"
#include "memory.h"
Folder *root = new Folder("root");
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
        str = str + std::to_string(localTime.tm_year + 1900) + ":" + std::to_string(localTime.tm_mon + 1) + ":" + std::to_string(localTime.tm_mday) + ":" + std::to_string(localTime.tm_hour) + ":" + std::to_string(localTime.tm_min) + ":" + std::to_string(localTime.tm_sec) + ":";
    }
    else
    {
        // 错误处理
        std::cerr << "Error in localtime_s" << std::endl;
    }
    return str;
}
// 将字符串转化为char数组
char *gotoChar(std::string str)
{
    char *ch = new char[str.size() + 1];
    str.copy(ch, str.size());
    ch[str.size()] = '\0';
    return ch;
}

// 将char数组转化为字符串
std::string gotoString(char *ch)
{
    std::string str(ch);
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
    return ReadFile(std::to_string(this->ID));
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
    this->Change_time = gettime();
}

// 设置标识符
void File::set_ID()
{
    std::string str = this->Create_time;
    std::string str1 = "";
    int n = 0;
    for (int i = 0; i < str.size(); i++)
    {
        if (n < 3)
        {
            if (str[i] == ';')
            {
                n++;
            }
            else
            {
            }
        }
        else
        {
            if (str[i] == ':')
            {
            }
            else
            {
                str1 += str[i];
            }
        }
    }
    this->ID = std::stoi(str1);
}

// 写内容
void File::set_Content(std::string str)
{
    std::string time = gettime();
    this->Change_time = time;
    int n = str.size();
    n -= this->size;
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
    str += std::to_string(this->size) + "$";
    str += std::to_string(this->ID) + "$";
    return str;
}

// 文件属性反序列化
void File::file_deserialize(std::string str)
{
    std::string str1[6] = {};
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
    this->ID = std::stoi(str1[5]);
}

// 展示文件属性
std::vector<std::string> File::show()
{
    std::vector<std::string> v;
    v.push_back(std::to_string(this->ID));
    v.push_back(this->name);
    v.push_back(this->addr);
    v.push_back(std::to_string(this->size));
    v.push_back(this->Create_time);
    v.push_back(this->Change_time);
    return v;
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

// 参构造函数
Folder::Folder(std::string str)
{
    this->name = str;
    this->dad = this;
    this->addr = str;
    Create_time = gettime();
    Change_time = gettime();
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
        file->set_ID();
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
std::vector<std::string> Folder::show()
{
    std::vector<std::string> v;
    v.push_back(this->name);
    v.push_back(this->type);
    v.push_back(this->addr);
    v.push_back(std::to_string(this->size));
    v.push_back(std::to_string(this->File_number));
    v.push_back(std::to_string(this->Folder_number));
    v.push_back(this->Create_time);
    v.push_back(this->Change_time);
    return v;
}

// 文件内容读入文件
void file_con_ser_gen(File *file)
{
}

// 文件内容传给内存
void file_con_ser(File *file)
{
}

// 树形目录序列化生成
void tree_dir_ser_gen(std::string *str)
{
    std::queue<int> child_num;
    std::queue<int> type;
    std::queue<std::string> inform;
    std::queue<Folder *> q;
    q.push(root);
    child_num.push(root->get_Folder_child().size() + root->get_File_child().size());
    type.push(1);
    inform.push(root->folder_serialize());
    while (q.empty() == false)
    {
        Folder *folder = q.front();
        q.pop();
        int folder_num = folder->get_Folder_child().size();
        int file_num = folder->get_File_child().size();
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
    std::string str = "";
    tree_dir_ser_gen(&str);
    return str;
}

// 树形目录反序列化生成
void tree_dir_diser_gen(std::queue<int> type, std::queue<int> child_num, std::queue<std::string> inform)
{
    std::queue<Folder *> q;
    std::queue<int> q_num;
    root->folder_deserialize(inform.front());
    q.push(root);
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
                Folder *folder_child_folder = new Folder("新建文件夹");
                folder_child_folder->folder_deserialize(inform.front());
                inform.pop();
                q.front()->Add_folder(folder_child_folder, 0);
                q.push(folder_child_folder);
                q_num.push(child_num.front());
                child_num.pop();
            }
            else
            {
                type.pop();
                child_num.pop();
                File *folder_child_file = new File("新建文件");
                folder_child_file->file_deserialize(inform.front());
                inform.pop();
                q.front()->Add_file(folder_child_file, 0);
            }
        }
        q.pop();
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
    tree_dir_diser_gen(type, child_num, inform);
}

// 初始化函数
Folder *init()
{
    tree_dir_diser(gotoString(ReadDirectoryInfo()));
    return root;
}

// 判断文件夹是否重名
bool folder_is_repeat(Folder *folder, std::string str)
{
    for (int i = 0; i < folder->get_Folder_child().size(); i++)
    {
        if (folder->get_Folder_child()[i]->get_Name() == str)
        {
            return true;
        }
    }
    return false;
}
// 判断文件是否重名接口
bool file_is_repeat(Folder *folder, std::string str)
{
    for (int i = 0; i < folder->get_File_child().size(); i++)
    {
        if (folder->get_File_child()[i]->get_Name() == str)
        {
            return true;
        }
    }
    return false;
}

// 添加文件接口
File *add_file(Folder *folder, std::string str)
{
    File *file = new File(str);
    folder->Add_file(file);
    WriteDirectoryInfo(gotoChar(tree_dir_ser()));
    return file;
}

// 添加文件夹接口
Folder *add_folder(Folder *folder, std::string str)
{
    Folder *child_folder = new Folder(str);
    folder->Add_folder(child_folder);
    WriteDirectoryInfo(gotoChar(tree_dir_ser()));
    return child_folder;
}

// 删除文件夹
void delete_folder(Folder *folder)
{
    folder->delete_folder();
    WriteDirectoryInfo(gotoChar(tree_dir_ser()));
}

// 删除文件
void delete_file(File *file)
{
    file->delete_File();
    DeleteFile(std::to_string(file->get_ID()));
    WriteDirectoryInfo(gotoChar(tree_dir_ser()));
}

// 改文件夹名接口
Folder *folder_change_name(Folder *folder, std::string name)
{
    folder->set_Name(name);
    WriteDirectoryInfo(gotoChar(tree_dir_ser()));
    return folder;
}

// 改文件名
File *file_change_name(File *file, std::string name)
{
    file->set_Name_Type(name);
    WriteDirectoryInfo(gotoChar(tree_dir_ser()));
    return file;
}

// 改文件内容
File *file_change_content(File *file, std::string content)
{
    file->set_Content(content);
    WriteFile(std::to_string(file->get_ID()), content, gotoChar(tree_dir_ser()));
    return file;
}

// 获得文件夹的子文件夹
std::vector<Folder *> get_folder_child(Folder *folder)
{
    return folder->get_Folder_child();
}

// 获得文件夹的子文件
std::vector<File *> get_file_child(Folder *folder)
{
    return folder->get_File_child();
}

// 打开文件
std::string look_file_content(File *file)
{
    return file->get_Content();
}

// 查看文件属性
std::vector<std::string> look_file(File *file)
{
    return file->show();
}

// 查看文件夹属性
std::vector<std::string> look_folder(Folder *folder)
{
    return folder->show();
}
