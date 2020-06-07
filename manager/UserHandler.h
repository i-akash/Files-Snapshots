#if !defined(USER_HANDLER)
#define USER_HANDLER

#include <iostream>
#include <string>
#include "SnapShotsManager.h"
#include "../configurations/SnapConfig.h"
#include "../io/IO.h"

class UserHandler
{
public:
    static void handleUser(int argc, char const *argv[])
    {
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
            handlerUserAction(argc, argv);
        }
    }

    static void handlerUserAction(int argc, char const *argv[])
    {
        auto action = std::string(argv[1]);
        SnapConfigInfo *config = nullptr;
        if (action == "init" && argc >= 3)
        {
            config = SnapConfig::initializeSnapConfig(std::string(argv[2]));
            SnapConfig::saveConfigInfo(config);
            std::cout << "initialized successfully \n";
            return;
        }
        else
        {
            config = SnapConfig::restoreConfigInfo();
            if (config == nullptr)
            {
                return;
            }
        }
        auto manager = new SnapShotsManager(config);
        manager->restoreChain();

        if (action == "commit")
        {
            auto filepaths = FileExplorer::getAllFiles(config->snapRootPath);
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
};

#endif // USER_HANDLER