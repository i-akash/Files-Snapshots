#if !defined(BLOCK_DEF)
#define BLOCK_DEF
#include <string>
#include <map>
#include "./FileMetadata.h"
#include "../crypto/HashSuit.h"

class Block
{
public:
    std::map<std::string, FileMetadata *> fileMetadataMapper;
    std::size_t prevHash;
    std::size_t blockHash;

    Block(std::map<std::string, FileMetadata *> fileMetadataMapper, std::size_t prevHash)
    {
        this->fileMetadataMapper = fileMetadataMapper;
        this->prevHash = prevHash;
        this->blockHash = HashSuit::getHash(std::to_string(rand()));
    }

    ~Block()
    {
    }
};
#endif // BLOCK_DEF
