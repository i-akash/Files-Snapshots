#ifndef FILE_INFO
#define FILE_INFO

#include <string>
#include "../enums/FileState.h"
#include "../crypto/HashSuit.h"
#include "./LineMetadata.h"
#include "../helper/MapHelpers.h"
#include <map>
#include <vector>

class FileMetadata
{
public:
    size_t fileHash;
    std::string createdBy;
    std::string updatedBy;
    FileState state;
    std::map<size_t, LineMetadata> lineMapper;

    FileMetadata() {}

    FileMetadata(std::vector<std::string> plines)
    {
        fileHash = HashSuit::getHash(getContent(plines));
    }

    void create(std::vector<std::string> plines, std::string pCreatedBy)
    {
        state = FileState::NEW;
        createdBy = pCreatedBy;
        addLinesToMap(lineMapper, plines, pCreatedBy);
    }

    void dontModify(FileMetadata fileMetadata)
    {
        fileHash = fileMetadata.fileHash;
        createdBy = fileMetadata.createdBy;
        updatedBy = fileMetadata.updatedBy;
        state = FileState::NOT_MODIFED;
        lineMapper = fileMetadata.lineMapper;
    }

    void update(std::vector<std::string> plines, std::map<size_t, LineMetadata> pLineMapper, std::string pUpdatedBy)
    {
        state = FileState::UPDATED;
        updatedBy = pUpdatedBy;
        addLinesToMap(pLineMapper, plines, pUpdatedBy);
        deletedLinesFromMap(pLineMapper, pUpdatedBy);
    }

    void _delete(std::string pUpdatedBy)
    {
        state = FileState::DELETED;
        updatedBy = pUpdatedBy;
    }

    ~FileMetadata() {}

private:
    void addLinesToMap(std::map<size_t, LineMetadata> pLineMapper, std::vector<std::string> pLines, std::string author)
    {
        for (auto line : pLines)
        {
            size_t lineHash = HashSuit::getHash(line);
            if (isExist(lineMapper, lineHash) == false)
            {
                if (isExist(pLineMapper, lineHash))
                {
                    auto lineMetadata = pLineMapper[lineHash];
                    if (lineMetadata.state == FileState::DELETED)
                    {
                        lineMetadata.create(author);
                    }
                    else
                    {
                        lineMetadata.state = FileState::NEW;
                    }
                    lineMapper[lineHash] = lineMetadata;
                }
                else
                {
                    LineMetadata lineMetadata(line, lineHash);
                    lineMetadata.create(author);
                    lineMapper[lineHash] = lineMetadata;
                }
            }
        }
    }

    void deletedLinesFromMap(std::map<size_t, LineMetadata> pLineMapper, std::string author)
    {
        for (auto linePair : pLineMapper)
        {
            size_t lineHash = linePair.first;
            if (isExist(lineMapper, lineHash) == false)
            {
                auto lineMetadata = linePair.second;
                lineMetadata._delete(author);
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
