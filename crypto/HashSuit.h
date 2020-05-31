#if !defined(HASH_SUIT)
#define HASH_SUIT
#include <string>

class HashSuit
{
public:
    std::size_t static getHash(std::string pText)
    {
        std::hash<std::string> hashObject;
        return hashObject(pText);
    }
};
#endif // HASH_SUIT