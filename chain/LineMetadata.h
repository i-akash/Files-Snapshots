#if !defined(LINE_METADATA)
#define LINE_METADATA

#include <string>
#include "../crypto/HashSuit.h"

class LineMetadata
{
public:
    std::string line;
    std::string createdBy;
    std::string deletedBy;
    FileState state;
    std::size_t lineHash;
    LineMetadata(LineMetadata *lineMetadata)
    {
        line = lineMetadata->line;
        createdBy = lineMetadata->createdBy;
        deletedBy = lineMetadata->deletedBy;
        state = lineMetadata->state;
        lineHash = lineMetadata->lineHash;
    }

    LineMetadata(std::string pLine, size_t pLineHash)
    {
        this->line = pLine;
        this->lineHash = pLineHash;
    }

    void create(std::string pCreatedBy)
    {
        state = FileState::NEW;
        createdBy = pCreatedBy;
    }

    void _delete(std::string pDeletedBy)
    {
        state = FileState::DELETED;
        deletedBy = pDeletedBy;
    }

    ~LineMetadata() {}
};

#endif // LINE_METADATA
