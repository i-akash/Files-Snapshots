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

public:
    Chain()
    {
        chain = new LinkedList<Block *>();
    }

    void addGenesisBlock()
    {
        std::map<std::string, FileMetadata> fileMetadataMapper;
        auto genesisBlock = new Block(fileMetadataMapper, 0);
        pushBack(genesisBlock);
    }

    void pushBack(Block *block)
    {
        chain->pushBack(block);
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

    ~Chain()
    {
    }
};
#endif // CHAIN_DEF