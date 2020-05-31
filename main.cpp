#include <iostream>
#include "file_expolere/FileExplorer.h"
#include "manager/SnapShotsManager.cpp"

int main(int argc, char const *argv[])
{
    auto s = "E:\\snapshots\\.snapshots";
    auto path1 = "E:\\snapshots-linear\\.snap1";
    auto path2 = "E:\\snapshots-linear\\.snap2";
    auto manager = new SnapShotsManager();

    auto filepaths = FileExplorer::getAllFiles(path1);
    auto block = manager->CreateBlock(filepaths);
    manager->addBlock(block);

    filepaths = FileExplorer::getAllFiles(path2);
    block = manager->CreateBlock(filepaths);
    manager->addBlock(block);

    manager->printChain();

    return 0;
}
