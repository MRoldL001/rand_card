/**
 * @author: MRoldL001
 * @date: 2024/04/04
 * @description: 从数据库中随机抽取图片并打开
 */

#include "sqlite3.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "(╬▔皿▔)╯ 请在命令 " << argv[0] << " 后加入参数 <抽取数量>" << std::endl;
        return 1;
    }

    int num_to_select = std::atoi(argv[1]);

    // 输入数字小于等于0，报错退出
    if (num_to_select <=0){
        std::cerr << "Error(っ °Д °;)っ:请输入一个非零正整数: " << std::endl;
        return 1;
    }

    // 输入数字大于100，要求确认
    std::string warn = "";
    if (num_to_select > 100)
    {
        std::cout << "Warnning(っ °Д °;)っ:你确定要抽取超过100张卡片吗? " << std::endl;
        std::cout << "Warnning(っ °Д °;)っ:如果你知道你在做什么，请输入 yes : " << std::endl;

        std::cin >> warn;
        if (warn != "yes")
        {
            return 1;
        }
    }

    sqlite3 *db;
    sqlite3_stmt *stmt;
    const char *query = "SELECT file_name, file_path FROM images ORDER BY RANDOM() LIMIT ?";

    // 连接数据库
    int rc = sqlite3_open("./data/cardNum.db", &db);
    if (rc)
    {
        std::cerr << "Error(っ °Д °;)っ:无法打开数据库: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    // 打开 ydk 文件
    std::ofstream outfile;
    outfile.open("抽取到的卡片.ydk");
    if (!outfile.is_open())
    {
        std::cerr << "Error(っ °Д °;)っ:无法打开 ydk 文件: " << std::endl;
    }
    outfile << "#created by rand_card" << std::endl;

    // 编译并执行 SQL 查询
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Error(っ °Д °;)っ:无法准备语句: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    // 绑定参数
    sqlite3_bind_int(stmt, 1, num_to_select);

    // 遍历结果集
    srand(time(NULL));
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        const char *file_name = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
        const char *file_path = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));

        // 清除file_name的后缀名
        std::string file_name_no_extension = file_name;
        file_name_no_extension.erase(file_name_no_extension.find_last_of("."));

        // 显示 file_name
        std::cout << "已抽取卡片: " << file_name << " ヾ(≧▽≦*)o" << std::endl;

        // 将不带后缀的文件名写入ydk文件
        outfile << file_name_no_extension << std::endl;

        // 打开对应的文件
        std::string command = "start \"\" \"" + std::string(file_path) + "\""; // 使用 start 命令打开文件
        system(command.c_str());                                               // 调用系统命令打开文件
    }

    // 释放资源
    std::cout << "已抽取指定数量的卡片捏 (*°▽°*)" << std::endl;
    std::cout << ".ydk 卡组码文件已生成 (*°▽°*)" << std::endl;
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    outfile.close();

    return 0;
}