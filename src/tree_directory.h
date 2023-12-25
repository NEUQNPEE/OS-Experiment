#pragma once
#include <sstream>
#include "file_control.h"
Folder *begin_folder;
Folder root = Folder();
// 子节点数以及文件类型
std::vector<int> tree_number;
std::vector<std::string> tree_dir;
std::queue<Folder *> folder_dir;
std::queue<File *> file_dir;
std::queue<int> fileOrfolder;
// 子节点数生成
void cdir_num_generate();
// 子节点数序列化
std::string cdir_num_serialize();
// 树形目录生成
void tree_generate();
// 树形目录序列化
std::string tree_serialize();
// 文件和文件夹按照树形目录反序列化
void fileOrFilder_deserialize(std::queue<int> arr1, std::queue<int> arr2, std::queue<std::string> str);
// 树形目录的反序列化
void tree_deserialize(std::string tree_str, std::string f_str);