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
    std::vector<std::string> lines;
    std::map<size_t, LineMetadata *> lineMapper;
    size_t fileHash;
    std::string creator;
    std::string updater;
    FileState state;

    FileMetadata(std::vector<std::string> plines)
    {
        lines = plines;
        fileHash = HashSuit::getHash(getContent());
    }

    void create(std::string pCreator)
    {
        state = FileState::NEW;
        creator = pCreator;
        addLinesToMap(pCreator);
    }

    void update(std::map<size_t, LineMetadata *> pLineMapper, std::string pUpdater)
    {
        state = FileState::UPDATED;
        updater = pUpdater;
        addLinesToMap(pUpdater);
        deletedLinesFromMap(pLineMapper, pUpdater);
    }

    void _delete(std::string pUpdater)
    {
        state = FileState::DELETED;
        updater = pUpdater;
    }

    ~FileMetadata() {}

private:
    void addLinesToMap(std::string author)
    {
        for (auto line : lines)
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

    std::string getContent()
    {
        std::string content = "";
        for (auto line : lines)
        {
            content += line;
        }
        return content;
    }
};

#endif
