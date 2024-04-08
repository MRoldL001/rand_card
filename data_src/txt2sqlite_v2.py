# @author: MRoldL001
# @date: 2023/04/04
# @description: 将指定文件夹下的文件名存入 txt 文件，并将其存入数据库
# @usage: 将图片存入 rand_card 文件夹下的 \data_src\img 文件夹内，然后运行此脚本

import sqlite3
import os
import re


def sort_by_number(filename):
    return [int(s) if s.isdigit() else s.lower() for s in re.split(r'(\d+)', filename)]


# 文件夹路径
folder_path = '.\\img\\'

# 连接数据库
rc = sqlite3.connect("..\\data\\cardNum.db")
cursor = rc.cursor()

# 清空表格
cursor.execute('''DELETE FROM images''')

# 打开 data_src.txt 写入文件名并逐行读取数据
with open("data_src.txt", 'w', encoding='utf-8') as file:
    file.write("#数据库中存在的卡片编号#\n")
    for file_name in sorted(os.listdir(folder_path), key=sort_by_number):
        # 去除后缀名
        file_name_no_extension = os.path.splitext(file_name)[0]
        file.write(file_name_no_extension + '\n')
        # 构建文件路径
        file_path = ".\\data_src\\img\\" + file_name
        # 插入数据到数据库中的 file_name 列 file_path 列
        cursor.execute("INSERT INTO images (file_name, file_path) VALUES (?, ?)", (file_name, file_path))

# 提交并断开数据库连接
rc.commit()
rc.close()
