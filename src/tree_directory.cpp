#include "tree_directory.h"
// 子节点数生成
void cdir_num_generate()
{
    folder_dir.push(&root);
    fileOrfolder.push(1);
    while (fileOrfolder.empty() == false)
    {
        if (fileOrfolder.front() == 1)
        {
            fileOrfolder.pop();
            Folder *folder = folder_dir.front();
            folder_dir.pop();
            int folder_num = folder->get_Folder_child().size();
            int file_num = folder->get_File_child().size();
            tree_number.push_back(folder_num + file_num);
            tree_number.push_back(1);
            for (int i = 0; i < folder_num; i++)
            {
                folder_dir.push(folder->get_Folder_child()[i]);
                fileOrfolder.push(1);
            }
            for (int i = 0; i < file_num; i++)
            {
                file_dir.push(folder->get_File_child()[i]);
                fileOrfolder.push(0);
            }
        }
        else
        {
            fileOrfolder.pop();
            file_dir.pop();
            tree_number.push_back(0);
            tree_number.push_back(0);
        }
    }
}
// 子节点数序列化
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
void tree_generate()
{
    folder_dir.push(&root);
    fileOrfolder.push(1);
    while (fileOrfolder.empty() == false)
    {
        if (fileOrfolder.front() == 1)
        {
            fileOrfolder.pop();
            Folder *folder = folder_dir.front();
            folder_dir.pop();
            int folder_num = folder->get_Folder_child().size();
            int file_num = folder->get_File_child().size();
            tree_dir.push_back(folder->folder_serialize());
            for (int i = 0; i < folder_num; i++)
            {
                folder_dir.push(folder->get_Folder_child()[i]);
                fileOrfolder.push(1);
            }
            for (int i = 0; i < file_num; i++)
            {
                file_dir.push(folder->get_File_child()[i]);
                fileOrfolder.push(0);
            }
        }
        else
        {
            fileOrfolder.pop();
            File *file = file_dir.front();
            file_dir.pop();
            tree_dir.push_back(file->file_serialize());
        }
    }
}

// 树形目录序列化
std::string tree_serialize()
{
    std::string str = "";
    for (int i = 0; i < tree_dir.size(); i++)
    {
        str += tree_dir[i] + "$";
    }
    return str;
}
// 文件和文件夹按照树形目录反序列化
void fileOrFilder_deserialize(std::queue<int> arr1, std::queue<int> arr2, std::queue<std::string> str)
{
    std::queue<Folder *> q;
    std::queue<int> q_num;
    Folder folder_begin = Folder();
    folder_begin.change_Dad(&folder_begin);
    folder_begin.change_Addr("root");
    folder_begin.folder_deserialize(str.front());
    q.push(&folder_begin);
    q_num.push(arr1.front());
    arr1.pop();
    arr2.pop();
    str.pop();
    while (q.empty() == false)
    {
        int n = q_num.front();
        q_num.pop();
        for (int i = 0; i < n; i++)
        {
            if (arr2.front() == 1)
            {
                arr2.pop();
                Folder folder_child_folder = Folder();
                folder_child_folder.folder_deserialize(str.front());
                str.pop();
                q.front()->Add_folder(&folder_child_folder, 0);
                q.push(&folder_child_folder);
                q_num.push(arr1.front());
                arr1.pop();
            }
            else
            {
                arr1.pop();
                arr2.pop();
                File folder_child_file = File();
                folder_child_file.file_deserialize(str.front());
                str.pop();
                q.front()->Add_file(&folder_child_file, 0);
            }
        }
        q.pop();
    }
    begin_folder = &folder_begin;
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
        n++;
    }
    std::queue<std::string> str;
    for (int i = 0; i < f_str.size();)
    {
        for (int j = i; j < f_str.size(); j++)
        {
            if (f_str[j] == '$' && f_str[j + 1] == '$')
            {
                str.push(f_str.substr(i, j - i));
                std::cout << f_str.substr(i, j - i) << std::endl;
                i = j + 2;
                break;
            }
        }
    }
    // 文件或文件夹反序列化
    fileOrFilder_deserialize(arr1, arr2, str);
}
int main()
{
    root.change_Dad(&root);
    root.set_Name("root");
    Folder WOWOS = Folder();
    WOWOS.set_Name("WOWOS");
    root.Add_folder(&WOWOS);
    File warn = File();
    warn.set_Name_Type("warn.txt");
    WOWOS.Add_file(&warn);
    warn.set_Content("珍爱生命,远离窝窝屎");
    cdir_num_generate();
    for (int i = 0; i < tree_number.size(); i++)
    {
        std::cout << tree_number[i] << std::endl;
    }
    std::cout << cdir_num_serialize() << std::endl;
    tree_generate();
    for (int i = 0; i < tree_dir.size(); i++)
    {
        std::cout << tree_dir[i] << std::endl;
    }
    std::cout << tree_serialize() << std::endl;
    tree_deserialize(cdir_num_serialize(), tree_serialize());
}