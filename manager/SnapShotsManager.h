#if !defined(SNAP_MANAGER)
#define SNAP_MANAGER

#include "../chain/Chain.h"
#include <iostream>
#include <filesystem>
#include "../io/IO.h"
#include <vector>
#include <map>
#include "../file_expolere/FileExplorer.h"
#include "../chain/LineMetadata.h"

using namespace std;

class SnapShotsManager
{

public:
    Chain *chain;
    string rootPath;
    SnapShotsManager(string rootPath)
    {
        chain = new Chain();
        this->rootPath = rootPath;
    }

    void syncChain()
    {
    }

    Block *CreateBlock(vector<filesystem::path> filepaths)
    {
        std::map<std::string, FileMetadata *> fileMetadataMapper;

        auto lastValidBlock = chain->getLastBlock();
        if (lastValidBlock == nullptr)
        {
            chain->addGenesisBlock();
            lastValidBlock = chain->getLastBlock();
        }

        auto fileMapper = lastValidBlock->fileMetadataMapper;

        for (auto filepath : filepaths)
        {
            string filepathString = filepath.string();
            auto lines = IO::read(filepathString);
            auto fileMetadata = new FileMetadata(lines, "akash");
            auto prevFileMetadata = fileMapper[filepathString];
            if (prevFileMetadata == nullptr)
            {
                fileMetadata->create("akash");
            }
            else if (prevFileMetadata->fileHash == fileMetadata->fileHash)
            {
                fileMetadata->dontModify();
            }
            else
            {
                fileMetadata->update(prevFileMetadata->lineMapper, "bkash");
            }
            fileMetadataMapper[filepathString] = fileMetadata;
        }

        for (auto prevFileMetadata : fileMapper)
        {
            auto path = prevFileMetadata.first;

            if (fileMetadataMapper[path] == nullptr)
            {
                vector<string> emptyLines;
                fileMetadataMapper[path] = new FileMetadata(emptyLines, "akash");
                fileMetadataMapper[path]->_delete("akash");
            }
        }

        auto newBlock = new Block(fileMetadataMapper, lastValidBlock->blockHash);
        return newBlock;
    }

    void addBlock(Block *block)
    {
        chain->pushBack(block);
    }

    void printChain()
    {
        auto currentNode = chain->getHead();

        while (currentNode)
        {
            auto block = currentNode->data;
            printBlock(block);
            currentNode = currentNode->next;
        }
    }

    void printLastBlock()
    {
        auto currentNode = chain->getHead();

        while (currentNode->next)
        {
            currentNode = currentNode->next;
        }
        auto block = currentNode->data;
        printBlock(block);
    }

    void printFileFromLastBlock(string path)
    {
        auto currentNode = chain->getHead();
        while (currentNode->next)
        {
            currentNode = currentNode->next;
        }
        auto block = currentNode->data;
        auto fileMetadata = block->fileMetadataMapper[path];
        if (fileMetadata == nullptr)
        {
            cout << "No File with this path";
        }
        else
        {
            printFileMetadata(path, fileMetadata);
        }
    }

    void printBlock(Block *block)
    {

        cout << "__START_OF_BLOCK__"
             << "\n"
             << "Block : " << block->blockHash << "\n";

        for (auto fileMatadataPair : block->fileMetadataMapper)
        {
            auto path = fileMatadataPair.first;
            auto fileMetadata = fileMatadataPair.second;
            printFileMetadata(path, fileMetadata);
        }

        cout << "___END_OF_BLOCK___\n\n";
    }

    void printFileMetadata(string path, FileMetadata *fileMetadata)
    {
        if (fileMetadata->state == FileState::NEW)
        {
            cout << "File (New) : " << path << "\n";
        }
        else if (fileMetadata->state == FileState::UPDATED)
        {
            cout << "File (Updated) : " << path << "\n";
        }
        else if (fileMetadata->state == FileState::NOT_MODIFED)
        {
            cout << "File (Not Modified) : " << path << "\n";
        }
        else
        {
            cout << "File (Deleted) : " << path << "\n";
        }

        auto linesMapper = fileMetadata->lineMapper;

        for (auto linesPair : linesMapper)
        {
            auto metadata = linesPair.second;
            if (metadata->state == FileState::NEW)
            {
                cout << "created by - " << metadata->createdBy << " : ";
            }
            else
            {
                cout << "deleted by - " << metadata->deletedBy << " : ";
            }
            cout << metadata->line << "\n";
        }
    }

    void saveChain()
    {
        auto currentNode = chain->getHead();
        int index = 0;
        while (currentNode)
        {
            auto block = currentNode->data;
            saveBlock(block, index);
            currentNode = currentNode->next;
            index++;
        }
    }

    void saveBlock(Block *block, int index)
    {
        string blockDir = rootPath + "\\storage\\" + to_string(index);
        IO::createDir(blockDir);

        string content = "";
        content += to_string(block->prevHash) + "\n";
        content += to_string(block->blockHash) + "\n";

        for (auto fileMetadataPair : block->fileMetadataMapper)
        {
            content += fileMetadataPair.first + "\n";
            content += to_string(fileMetadataPair.second->fileHash) + "\n";

            saveFileMetadata(fileMetadataPair.second, blockDir);
        }

        IO::write(blockDir + "\\block.txt", content);
    }

    void saveFileMetadata(FileMetadata *fileMetadata, string blockDir)
    {
        string content = "";

        content += to_string(fileMetadata->fileHash) + "\n";
        content += to_string(fileMetadata->state) + "\n";
        content += fileMetadata->createdBy + "\n";
        content += fileMetadata->updatedBy + "\n";

        for (auto linePair : fileMetadata->lineMapper)
        {
            auto lineInfo = linePair.second;
            content += lineInfo->line + "\n";
            content += to_string(lineInfo->lineHash) + "\n";
            content += to_string(lineInfo->state) + "\n";
            content += lineInfo->createdBy + "\n";
            content += lineInfo->deletedBy + "\n";
        }

        IO::write(blockDir + "\\" + to_string(fileMetadata->fileHash) + ".txt", content);
    }

    void restoreChain()
    {
        string storageDir = rootPath + "\\storage\\";
        auto blockDirs = FileExplorer::getAllDirsOfOneLevel(storageDir);

        for (auto blockDir : blockDirs)
        {
            auto block = restoreBlock(blockDir.string());
            chain->pushBack(block);
        }
    }

    Block *restoreBlock(string blockDir)
    {
        auto lines = IO::read(blockDir + "\\block.txt");
        int line = 0;
        size_t prevHash = stoull(lines[line++]);
        size_t blockHash = stoull(lines[line++]);

        map<string, FileMetadata *> fileMapper;

        while (line < lines.size())
        {
            string filePath = lines[line++];
            string fileHash = lines[line++];
            string fileMetadataPath = blockDir + "\\" + fileHash + ".txt";
            FileMetadata *fileMetadata = restoreFileMetadata(fileMetadataPath);
            fileMapper[filePath] = fileMetadata;
        }

        auto restoredBlock = new Block(fileMapper, prevHash);
        return restoredBlock;
    }

    FileMetadata *restoreFileMetadata(string fileMetadataPath)
    {
        int line = 0;
        auto lines = IO::read(fileMetadataPath);

        FileMetadata *restoredFileMetadata = new FileMetadata();
        restoredFileMetadata->fileHash = stoull(lines[line++]);
        restoredFileMetadata->state = FileState(stoi(lines[line++]));
        restoredFileMetadata->createdBy = lines[line++];
        restoredFileMetadata->updatedBy = lines[line++];

        map<size_t, LineMetadata *> linesMapper;

        while (line < lines.size())
        {
            auto lineInfo = new LineMetadata();
            lineInfo->line = lines[line++];
            lineInfo->lineHash = stoull(lines[line++]);
            lineInfo->state = FileState(stoi(lines[line++]));
            lineInfo->createdBy = lines[line++];
            lineInfo->deletedBy = lines[line++];

            linesMapper[lineInfo->lineHash] = lineInfo;
        }

        restoredFileMetadata->lineMapper = linesMapper;
        return restoredFileMetadata;
    }
};

#endif // SNAP_MANAGER