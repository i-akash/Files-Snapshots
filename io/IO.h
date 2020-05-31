#if !defined(IO_DEF)
#define IO_DEF
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <ctype.h>
#include <filesystem>

class IO
{
public:
    static std::vector<std::string> read(std::string file)
    {
        std::ifstream reader(file);
        std::vector<std::string> lines;
        std::string line;
        while (std::getline(reader, line))
        {
            lines.push_back(line);
        }
        return lines;
    }
    static void write(std::string filepath, std::string content)
    {
        std::ofstream writer(filepath);
        auto contentCStr = content.c_str();
        writer.write(contentCStr, content.size());
    }
    static void createDir(std::string path)
    {
        std::filesystem::create_directories(path);
    }
};
#endif // IO_DEF
