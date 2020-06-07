#if !defined(FILE_EXPLORER)
#define FILE_EXPLORER

#include <filesystem>
#include <vector>
#include <iostream>
#include <queue>
#include <regex>

class FileExplorer
{
public:
    static std::vector<std::filesystem::path> getAllFiles(std::filesystem::path root)
    {
        auto rootIterator = std::filesystem::recursive_directory_iterator(root);
        std::vector<std::filesystem::path> filePaths;

        for (auto filepath : rootIterator)
        {
            if (filepath.is_directory())
            {
                continue;
            }
            else if (isIgnored(filepath.path().string()))
            {
                continue;
            }
            else
            {
                filePaths.push_back(filepath);
            }
        }

        return filePaths;
    }

    static std::vector<std::filesystem::path> getAllDirsOfOneLevel(std::filesystem::path root)
    {
        std::vector<std::filesystem::path> filePaths;
        try
        {
            auto iterator = std::filesystem::directory_iterator(root);
            for (auto filepath : iterator)
            {
                if (filepath.is_directory())
                {
                    filePaths.push_back(filepath.path());
                }
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
        return filePaths;
    }

    static bool isIgnored(std::string path)
    {
        std::vector<std::regex> ignoredList = {std::regex(".*\\.git.*"),
                                               std::regex(".*\\.snap1.*"),
                                               std::regex(".*\\.snap2.*"),
                                               std::regex(".*\\.vscode.*"),
                                               std::regex(".*storage.*"),
                                               std::regex(".*\\.exe")};
        for (auto ignoredPath : ignoredList)
        {
            if (std::regex_match(path, ignoredPath))
            {
                return true;
            }
        }

        return false;
    }
};

#endif // FILE_EXPLORER