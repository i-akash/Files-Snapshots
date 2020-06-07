#if !defined(USER_CONFIG)
#define USER_CONFIG

#include <string>
#include "../io/IO.h"
#include "../file_expolere/FileExplorer.h"

struct SnapConfigInfo
{
    std::string userName;
    std::string snapRootPath;
    std::string snapStoragePath;
    std::string snapLogsPath;
    std::string snapConfigPath;
};

class SnapConfig
{
public:
    static SnapConfigInfo *initializeSnapConfig(std::string userName)
    {
        auto configInfo = new SnapConfigInfo();
        configInfo->userName = userName;
        configInfo->snapRootPath = FileExplorer::currentWorkingDirectory().string();
        configInfo->snapStoragePath = configInfo->snapRootPath + "\\.snap";
        configInfo->snapLogsPath = configInfo->snapStoragePath + "\\logs.txt";
        configInfo->snapConfigPath = configInfo->snapStoragePath + "\\config.txt";

        return configInfo;
    }

    static SnapConfigInfo *restoreConfigInfo()
    {
        auto snapRootDir = FileExplorer::searchSnapDir();
        auto configPath = snapRootDir + "\\config.txt";
        SnapConfigInfo *config = nullptr;

        if (snapRootDir.size() > 0)
        {
            auto content = IO::read(configPath);
            config = new SnapConfigInfo();
            config->userName = content[0];
            config->snapRootPath = content[1];
            config->snapStoragePath = content[2];
            config->snapLogsPath = content[3];
            config->snapConfigPath = content[4];
        }

        return config;
    }

    static void saveConfigInfo(SnapConfigInfo *config)
    {
        std::string content = "";
        content += config->userName + "\n";
        content += config->snapRootPath + "\n";
        content += config->snapStoragePath + "\n";
        content += config->snapLogsPath + "\n";
        content += config->snapConfigPath + "\n";

        IO::createDir(config->snapStoragePath);
        IO::write(config->snapConfigPath, content);
    }
};

#endif // USER_CONFIG
