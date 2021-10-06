#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <ctime>
#include <FlexLexer.h>

size_t get_type(const std::string& str);
void from_keyboard(size_t type_output);
void from_file(size_t type_output);
std::string get_output_filename();
void proceed(yyFlexLexer& ftp, std::ostream& out, std::map<std::string, size_t>& server_map);

int main()
{
    if (const auto type_input{ get_type("Input from keyboard or from file?")}, type_output{ get_type("Output on keyboard or in file ?") }; type_input == 1)
        from_keyboard(type_output);
    else if (type_input == 2)
        from_file(type_output);

    return 0;
}

size_t get_type(const std::string& str)
{
    while (true)
    {
        std::cout << str << " [1 or 2]: ";

        std::string type_str;
        std::cin >> type_str;

        if (type_str == "1")
            return 1;
        else if (type_str == "2")
            return 2;

        std::cout << "Wrong input! Try again: ";
    }
}

void from_keyboard(size_t type_output)
{
    std::ofstream fout = {};
    std::ostream* out = &std::cout;
    if (type_output == 2) {
        fout.open(get_output_filename());
        out = &fout;
    }
    std::cout << std::endl;

    std::map<std::string, size_t> server_map = {};

    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    yyFlexLexer ftp(&std::cin);
    while (std::cin.good()) {
        proceed(ftp, *out, server_map);
    }
    
    *out << std::endl;
    for (auto& [key, value] : server_map) {
        *out << key << " - " << value << std::endl;
    }
}

void from_file(size_t type_output)
{
    std::cout << "Enter input file name: ";
    std::string file;
    std::cin >> file;
    std::ifstream fin(file);

    while (!fin.is_open())
    {
        std::cout << "This file doesn't exist. Try again: ";
        std::cin >> file;
        fin.open(file);
    }

    std::ofstream fout;
    std::ostream* out = &std::cout;
    if (type_output == 2)
    {
        fout.open(get_output_filename());
        out = &fout;
    }

    std::cout << std::endl;
    std::string str;
    std::map<std::string, size_t> server_map;

    yyFlexLexer ftp(&fin);
    unsigned int sum_time = 0;
    while (!fin.eof()) {
        unsigned int start_time = clock();
        proceed(ftp, *out, server_map);
        unsigned int end_time = clock();
        sum_time += end_time - start_time;
    }
    std::cout << "Time: " << sum_time << std::endl;

    *out << std::endl;
    for (auto& [key, value] : server_map)
        *out << key << " - " << value << std::endl;

    fin.close();
    if (type_output == 2)
        (dynamic_cast<std::ofstream&>(*out)).close();
}

std::string get_output_filename()
{
    std::cout << "Enter output file name: ";
    std::string file;
    std::cin >> file;

    return file;
}

void proceed(yyFlexLexer& ftp, std::ostream& out, std::map<std::string, size_t>& server_map)
{
    if (ftp.yylex()) {
        out << "acceptable. Server name is " << ftp.YYText() << std::endl;

        bool isAdd = false;
        for (auto& [key, value] : server_map) {
            if (key == ftp.YYText()) {
                value++;
                isAdd = true;
                break;
            }
        }

        if (!isAdd)
            server_map.emplace(ftp.YYText(), 1);
    }
    else
        out << "not acceptable" << std::endl;
}