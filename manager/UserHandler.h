#if !defined(USER_HANDLER)
#define USER_HANDLER

#include <iostream>
#include <string>
#include "SnapShotsManager.h"

class UserHandler
{
public:
    static void handleUser(int argc, char const *argv[])
    {
        auto rootpath = "E:\\snapshots-linear";
        auto manager = new SnapShotsManager(rootpath);
        manager->restoreChain();

        if (argc <= 1)
        {
            std::cout << "No command is given\n";
            std::cout << "___Commands___\n";
            std::cout << "snap commit\n";
            std::cout << "snap print\n";
            std::cout << "snap print filename\n";
        }
        else
        {
            auto action = std::string(argv[1]);
            if (action == "commit")
            {
                auto filepaths = FileExplorer::getAllFiles(rootpath);
                auto block = manager->CreateBlock(filepaths);
                manager->addBlock(block);
                manager->saveChain();
                std::cout << "commited\n";
            }
            else if (action == "print")
            {
                if (argc == 2)
                {
                    manager->printLastBlock();
                }
                else
                {
                    manager->printFileFromLastBlock(std::string(argv[2]));
                }
            }
            else if (action == "status")
            {
            }
            else if (action == "diff")
            {
            }
        }
    }
};

#endif // USER_HANDLER