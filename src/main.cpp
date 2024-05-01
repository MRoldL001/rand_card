/**
 * @author: MRoldL001
 * @date: 2024/04/04
 * @description: �����ݿ��������ȡͼƬ����
 */

#include "sqlite3.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "(�p����)�s �������� " << argv[0] << " �������� <��ȡ����>" << std::endl;
        return 1;
    }

    int num_to_select = std::atoi(argv[1]);

    // ��������С�ڵ���0�������˳�
    if (num_to_select <= 0)
    {
        std::cerr << "Error(�� �㧥 ��;)��:������һ������������: " << std::endl;
        return 1;
    }

    // �������ִ���100��Ҫ��ȷ��
    std::string warn = "";
    if (num_to_select > 100)
    {
        std::cout << "Warning(�� �㧥 ��;)��:��ȷ��Ҫ��ȡ����100�ſ�Ƭ��? " << std::endl;
        std::cout << "Warning(�� �㧥 ��;)��:�����֪��������ʲô�������� yes : " << std::endl;

        std::cin >> warn;
        if (warn != "yes")
        {
            return 1;
        }
    }

    sqlite3 *db;
    sqlite3_stmt *stmt;
    const char *query = "SELECT file_name, file_path FROM images ORDER BY RANDOM() LIMIT ?";

    // �������ݿ�
    int rc = sqlite3_open("./data/cardNum.db", &db);
    if (rc)
    {
        std::cerr << "Error(�� �㧥 ��;)��:�޷������ݿ�: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    // �� ydk �ļ�
    std::ofstream outfile;
    int card_num = 0;
    std::string card_name = ("��ȡ���Ŀ�Ƭ_" + std::to_string(card_num)) + ".ydk";
    while (1)
    {
        if (access(card_name.c_str(), F_OK) == 0)
        {
            card_num++;
            card_name = ("��ȡ���Ŀ�Ƭ_" + std::to_string(card_num)) + ".ydk";
        }
        else
        {
            outfile.open(card_name);
            if (!outfile.is_open())
            {
                std::cerr << "Error(�� �㧥 ��;)��:�޷��� ydk �ļ�: " << std::endl;
            }
            outfile << "#created by rand_card" << std::endl;
            break;
        }
    }

    // ���벢ִ�� SQL ��ѯ
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Error(�� �㧥 ��;)��:�޷�׼�����: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    sqlite3_bind_int(stmt, 1, num_to_select);

    // ����
    srand(time(NULL));
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        const char *file_name = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
        const char *file_path = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));

        // ���file_name�ĺ�׺��
        std::string file_name_no_extension = file_name;
        file_name_no_extension.erase(file_name_no_extension.find_last_of("."));

        // ��ʾ file_name
        std::cout << "�ѳ�ȡ��Ƭ: " << file_name << " �d(�R���Q*)o" << std::endl;

        // ��������׺���ļ���д��ydk�ļ�
        outfile << file_name_no_extension << std::endl;

        // �򿪶�Ӧ���ļ�
        std::string command = "start \"\" \"" + std::string(file_path) + "\""; // ʹ�� start ������ļ�
        system(command.c_str());                                               // ����ϵͳ������ļ�
    }

    // �ͷ���Դ
    std::cout << "�ѳ�ȡָ�������Ŀ�Ƭ�� (*�㨌��*)" << std::endl;
    std::cout << card_name << std::endl;
    std::cout << (".ydk �������ļ�������, ���Ϊ " + std::to_string(card_num)) + " (*�㨌��*)" << std::endl;
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    outfile.close();

    return 0;
}