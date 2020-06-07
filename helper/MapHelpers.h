#if !defined(KEY_MAP)
#define KEY_MAP
#include <map>

template <typename K, typename V>
bool isExist(const std::map<K, V> &mp, const K &key)
{
    auto it = mp.find(key);
    if (it == mp.end())
    {
        return false;
    }
    else
    {
        return true;
    }
}
#endif // KEY_MAP