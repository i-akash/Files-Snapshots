#ifndef FILE_INFO
#define FILE_INFO

#include <string>
#include "../enums/FileState.h"
#include "../crypto/HashSuit.h"
#include "./LineMetadata.h"
#include <map>
#include <vector>

class FileMetadata
{
public:
    size_t fileHash;
    std::string createdBy;
    std::string updatedBy;
    FileState state;
    std::map<size_t, LineMetadata *> lineMapper;

    FileMetadata() {}

    FileMetadata(std::vector<std::string> plines, std::string author)
    {
        fileHash = HashSuit::getHash(getContent(plines));
        addLinesToMap(author, plines);
    }

    void create(std::string pCreatedBy)
    {
        state = FileState::NEW;
        createdBy = pCreatedBy;
    }

    void dontModify()
    {
        state = FileState::NOT_MODIFED;
    }

    void update(std::map<size_t, LineMetadata *> pLineMapper, std::string pUpdatedBy)
    {
        state = FileState::UPDATED;
        updatedBy = pUpdatedBy;
        deletedLinesFromMap(pLineMapper, pUpdatedBy);
    }

    void _delete(std::string pUpdatedBy)
    {
        state = FileState::DELETED;
        updatedBy = pUpdatedBy;
    }

    ~FileMetadata() {}

private:
    void addLinesToMap(std::string author, std::vector<std::string> pLines)
    {
        for (auto line : pLines)
        {
            size_t lineHash = HashSuit::getHash(line);
            if (lineMapper[lineHash] == nullptr)
            {
                auto lineMetadata = new LineMetadata(line, lineHash);
                lineMetadata->create(author);
                lineMapper[lineHash] = lineMetadata;
            }
        }
    }

    void deletedLinesFromMap(std::map<size_t, LineMetadata *> pLineMapper, std::string author)
    {
        for (auto linePair : pLineMapper)
        {
            size_t lineHash = linePair.first;
            if (lineMapper[lineHash] == nullptr)
            {
                auto lineMetadata = linePair.second;
                lineMetadata->_delete(author);
                lineMapper[lineHash] = lineMetadata;
            }
        }
    }

    std::string getContent(std::vector<std::string> pLines)
    {
        std::string content = "";
        for (auto line : pLines)
        {
            content += line;
        }
        return content;
    }
};

#endif
