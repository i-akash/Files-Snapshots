#include "../chain/Chain.h"
#include <iostream>
#include <filesystem>
#include "../io/IO.h"

using namespace std;

class SnapShotsManager
{

public:
    Chain *chain;
    SnapShotsManager()
    {
        chain = new Chain();
    }

    void syncChain()
    {
    }

    Block *CreateBlock(vector<filesystem::path> filepaths)
    {
        std::map<std::string, FileMetadata *> fileMetadataMapper;

        auto lastValidBlock = chain->getLastBlock();
        auto fileMapper = lastValidBlock->fileMetadataMapper;

        for (auto filepath : filepaths)
        {
            string filepathString = filepath.string();
            auto lines = IO::read(filepathString);
            auto fileMetadata = new FileMetadata(lines);
            auto prevFileMetadata = fileMapper[filepathString];
            if (prevFileMetadata == nullptr)
            {
                fileMetadata->create("akash");
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
                fileMetadataMapper[path] = new FileMetadata(prevFileMetadata.second->lines);
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

    void printBlock(Block *block)
    {

        cout << "__START_OF_BLOCK__"
             << "\n"
             << "Block : " << block->blockHash << "\n";

        for (auto fileMatadataPair : block->fileMetadataMapper)
        {
            auto path = fileMatadataPair.first;
            auto fileMetadata = fileMatadataPair.second;

            if (fileMetadata->state == FileState::NEW)
            {
                cout << "File (New) : " << path << "\n";
            }
            else if (fileMetadata->state == FileState::UPDATED)
            {
                cout << "File (Updated) : " << path << "\n";
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

        cout << "___END_OF_BLOCK___\n\n";
    }
};