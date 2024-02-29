#!/bin/bash

# 获取最新的提交哈希值
latest_commit=$(git rev-parse HEAD)

# 获取之前的提交哈希值
previous_commit=$(git rev-parse HEAD^)

# 比较最新提交和之前提交之间的差异，筛选出修改的.proto文件
changed_proto_files=$(git diff --name-only $previous_commit $latest_commit | grep '\.proto$')

# 输出修改的.proto文件
echo "Changed .proto files:"
echo "$changed_proto_files"
