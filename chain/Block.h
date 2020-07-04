#if !defined(BLOCK_DEF)
#define BLOCK_DEF
#include <string>
#include <map>
#include "./FileMetadata.h"
#include "../crypto/HashSuit.h"
#include "../helper/MarkleRoot.h"

class Block
{
public:
    std::map<std::string, FileMetadata> fileMetadataMapper;
    std::size_t prevHash;
    std::size_t blockHash;

    Block(std::map<std::string, FileMetadata> fileMetadataMapper, std::size_t prevHash)
    {
        this->fileMetadataMapper = fileMetadataMapper;
        this->prevHash = prevHash;
        this->blockHash = this->computeBlockHash();
    }

    ~Block()
    {
    }

private:
    size_t computeBlockHash()
    {
        std::vector<size_t> filesHash;
        for (auto file : fileMetadataMapper)
        {
            filesHash.push_back(file.second.fileHash);
        }
        auto markleRoot = getMarkleRoot<size_t>(filesHash);
        auto blockHashContent = markleRoot + std::to_string(prevHash);
        return HashSuit::getHash(blockHashContent);
    }
};
#endif // BLOCK_DEF
