
#if !defined(LOG_DEF)
#define LOG_DEF
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <ctype.h>
#include <filesystem>

class Log
{
public:
    static void write(std::string filepath, std::string content)
    {
        std::ofstream writer(filepath, std::ios_base::app);
        auto contentCStr = content.c_str();
        writer.write(contentCStr, content.size());
    }

    static void createDir(std::string path)
    {
        std::filesystem::create_directories(path);
    }
};
#endif // LOG_DEF
