#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem/operations.hpp>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cctype> 
#include <boost/regex.hpp>
#include <boost/crc.hpp>
#include <fstream>

// Вычисление контрольной суммы (CRC32) для строки
uint32_t crc32(const std::string& my_string)
{
    boost::crc_32_type result;
    result.process_bytes(my_string.data(), my_string.length());
    return result.checksum();
}

// Чтение блоков из двух файлов и вычисление их хэшей
void readblock(std::ifstream &file1, std::ifstream &file2, uint32_t &hash_for_file1_block, uint32_t &hash_for_file2_block, int block_size)
{
    char buffer1[block_size];
    char buffer2[block_size];

    // Чтение блока из первого файла
    file1.read(buffer1, block_size);
    file2.read(buffer2, block_size);

    // Заполнение оставшейся части буфера нулями, если блок меньше заданного размера
    int counter1 = file1.gcount();
    if (counter1 < block_size)
    {
        for (auto i = counter1; i < block_size; i++)
        {
            buffer1[i] = '\0';
        }
    }

    int counter2 = file2.gcount();
    if (counter2 < block_size)
    {
        for (auto i = counter2; i < block_size; i++)
        {
            buffer2[i] = '\0';
        }
    }

    // Создание строк из считанных блоков
    std::string str1(buffer1, file1.gcount());
    std::string str2(buffer2, file2.gcount());

    // Вычисление хэшей для блоков
    hash_for_file1_block = crc32(str1);
    hash_for_file2_block = crc32(str2);
}

// Поиск дубликатов файлов на основе поблочного сравнения
std::vector<std::set<boost::filesystem::path>> find_repeat(std::vector<boost::filesystem::path> &all_files, int block_size)
{
    std::vector<std::set<boost::filesystem::path>> repeated;
    for (size_t i = 0; i < all_files.size(); ++i) 
    {
        std::set<boost::filesystem::path> group;
        for (size_t j = i + 1; j < all_files.size(); ++j) 
        {
            bool equal = true;
            boost::filesystem::path path1 = all_files[i];
            boost::filesystem::path path2 = all_files[j];
            std::ifstream file1(path1);
            std::ifstream file2(path2);

            uintmax_t size1 = boost::filesystem::file_size(path1);
            uintmax_t size2 = boost::filesystem::file_size(path2);

            // Выравнивание размера до ближайшего кратного размеру блока
            size_t size1_withzero = size1 + (block_size - (size1 % block_size));
            size_t size2_withzero = size2 + (block_size - (size2 % block_size));

            if (size1 != size2)
            {
                equal = false; // Размеры не совпадают, файлы не дубликаты
            }
            else
            {
                size_t blocks_number = size1_withzero / block_size; // Количество блоков
                uint32_t hash_for_file1_block;
                uint32_t hash_for_file2_block;
                for (int k = 0; k < blocks_number; k++)
                {
                    readblock(file1, file2, hash_for_file1_block, hash_for_file2_block, block_size);
                    if (hash_for_file1_block != hash_for_file2_block)
                    {
                        equal = false;
                        break;
                    }
                }
            }
            if (equal == true)
            {
                group.insert(path1);
                group.insert(path2);
            }
        }
        if (!group.empty())
        {
            repeated.push_back(group);
        }
    }
    return repeated;
}

// Рекурсивное сканирование директории с фильтрацией файлов
void scan_dir(std::vector<boost::filesystem::path> &files, std::string view, boost::filesystem::path dir_path, int level, uintmax_t file_size, const std::string &ignor_dir = "", const std::string& mask = "")
{
    for (const boost::filesystem::directory_entry &entry : boost::filesystem::directory_iterator(dir_path))
    {
        std::string file = view + entry.path().filename().string();

        // Приведение имени файла к нижнему регистру
        for (char& c : file) 
        {
            c = std::tolower(static_cast<unsigned char>(c));
        }

        if (boost::filesystem::is_directory(entry) && level == 1 && entry.path().string() != ignor_dir)
        {
            std::cout << file << std::endl;
            scan_dir(files, view + "   ", entry, level, file_size, ignor_dir, mask);
        }

        if (boost::filesystem::is_regular_file(entry) && boost::filesystem::file_size(entry.path()) >= file_size)
        {
            if (mask != "")
            {
                // Создание регулярного выражения для маски
                std::string regex_mask = "^" + boost::regex_replace(mask, boost::regex("\\*"), ".*") + "$";
                boost::regex regex_pattern(regex_mask);
                if (boost::regex_match(file, regex_pattern))
                {
                    uintmax_t size = boost::filesystem::file_size(entry.path());
                    std::cout << file << "  " << size << std::endl;
                    files.push_back(entry);
                }
            }
            else
            {
                uintmax_t size = boost::filesystem::file_size(entry.path());
                std::cout << file << "  " << size << std::endl;
                files.push_back(entry);
            }
        }
    }
} 

// Главная функция программы
int main(int argc, char* argv[])
{
    std::string view = "";
    std::string str_dir = "C:\\Programming\\pr7\\testdir";
    std::string ignordir{ "" };
    uintmax_t minsize = 0; // Минимальный размер файла
    int level = 1; // 1 - все директории, 0 - только текущая директория
    std::string mask{ "" };
    int block_size = 2; // Размер блока

    // Настройка параметров командной строки
    boost::program_options::options_description desc;
    desc.add_options()
        ("dir,d", boost::program_options::value<std::string>(&str_dir)->default_value("C:\\Programming\\pr7\\testdir"))
        ("ignordir,id", boost::program_options::value<std::string>(&ignordir)->default_value(""))
        ("mask,m", boost::program_options::value<std::string>(&mask)->default_value("*.txt"))
        ("minsize,ms", boost::program_options::value<uintmax_t>(&minsize)->default_value(0))
        ("level,l", boost::program_options::value<int>(&level)->default_value(1))
        ("blocksize,bs", boost::program_options::value<int>(&block_size)->default_value(2));

    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm);

    boost::filesystem::path dir{ str_dir };
    for (char& c : mask) 
    {
        c = std::tolower(static_cast<unsigned char>(c));
    }

    // Сканирование директории
    std::vector<boost::filesystem::path> files;
    scan_dir(files, view, dir, level, minsize, ignordir, mask);

    // Вывод списка подходящих файлов
    std::cout << std::endl << "files that satisfy conditions:" << std::endl;
    for (int i = 0; i < files.size(); i++)
    {
        std::cout << files[i] << std::endl;
    } 
    
    std::cout<<std::endl<<"repeated files:"<<std::endl;
    std::vector<std::set<boost::filesystem::path>> repeated_groups;
    repeated_groups=find_repeat(files,block_size);
    for(int i=0;i<repeated_groups.size();i++)
    {
        if(repeated_groups[i].size()!=0)
        {
            for (const auto& path : repeated_groups[i]) 
            {
                std::cout << path << std::endl;
            }
            std::cout<<std::endl;
        }
    }
}
