#if !defined(FILE_EXPLORER)
#define FILE_EXPLORER

#include <filesystem>
#include <vector>
#include <iostream>

class FileExplorer
{
public:
    static std::vector<std::filesystem::path> getAllFiles(std::filesystem::path root)
    {
        auto iterator = std::filesystem::recursive_directory_iterator(root);
        std::vector<std::filesystem::path> filePaths;
        for (auto filepath : iterator)
        {
            if (!filepath.is_directory())
            {
                filePaths.push_back(filepath.path());
            }
        }

        return filePaths;
    }
};

#endif // FILE_EXPLORER