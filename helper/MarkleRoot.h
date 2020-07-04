#if !defined(MARKLE_ROOT)
#define MARKLE_ROOT
#include <queue>
#include <string>
#include "../crypto/HashSuit.h"

template <typename T>
std::string getMarkleRoot(std::vector<T> collections)
{
    std::queue<std::string> Q;

    for (auto element : collections)
    {
        Q.push(std::to_string(element));
    }

    while (Q.size() != 1)
    {
        auto first = Q.front();
        Q.pop();
        auto second = Q.front();
        Q.pop();

        auto combined = first + second;
        auto combinedHash = std::to_string(HashSuit::getHash(combined));
        Q.push(combinedHash);
    }
    return Q.front();
}
#endif // MARKLE_ROOT