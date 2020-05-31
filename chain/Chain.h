#if !defined(CHAIN_DEF)
#define CHAIN_DEF
#include "./Block.h"
#include "../data-structure/LinkedList.h"
#include <map>
#include <string>

class Chain
{
private:
    LinkedList<Block *> *chain;
    // std::string dataFolder;

public:
    Chain()
    {
        chain = new LinkedList<Block *>();
        addGenesisBlock();
        //this->dataFolder = dataFolder;
    }

    void addGenesisBlock()
    {
        std::map<std::string, FileMetadata *> fileMetadataMapper;
        auto genesisBlock = new Block(fileMetadataMapper, 0);
        pushBack(genesisBlock);
    }

    void pushBack(Block *block)
    {

        chain->pushBack(block);
        // saveBlock(block);
    }

    Block *getLastBlock()
    {

        auto currentNode = chain->mHead;
        if (currentNode == nullptr)
        {
            return nullptr;
        }

        while (currentNode->next)
        {
            currentNode = currentNode->next;
        }
        return currentNode->data;
    }

    Node<Block *> *getHead()
    {
        return chain->mHead;
    }

    // void saveBlock(Block *block)
    // {
    //     std::string dir = this->dataFolder + "\\" + block->branchName + "\\" + to_string(block->blockHash);
    //     IO::createDir(dir);

    //     IO::write(dir + "\\blockInfo.txt", block->branchName + "\n" + to_string(block->prevHash) + "\n" + to_string(block->blockHash) + "\n");
    // }

    ~Chain()
    {
    }
};
#endif // CHAIN_DEF