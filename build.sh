#!/bin/bash

# 编译选项
CXXFLAGS="-std=c++11"  # 设置 C++ 标准
INCLUDE_DIR="include"  # 指定头文件搜索路径
LIB_DIR="lib"  # 指定库文件搜索路径

# MinGW 工具链路径
MINGW_PATH="C:/mingw64/bin"

# 源文件目录
SRC_DIR="src"

# 源文件
SRC_FILES="$SRC_DIR/main.cpp"

# 输出文件名
OUTPUT_FILE="RandCard.exe"

# 编译命令
$MINGW_PATH/g++ -static $CXXFLAGS -I$INCLUDE_DIR -L$LIB_DIR -o $OUTPUT_FILE $SRC_FILES -lsqlite3

# 检查编译是否成功
if [ $? -eq 0 ]; then
    echo "编译成功！"
else
    echo "编译失败！"
fi