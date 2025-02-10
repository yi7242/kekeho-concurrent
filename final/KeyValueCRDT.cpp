#include <iostream>
#include <map>
#include <string>
#include <set>
#include <functional>

class KeyValueCRDT
{
public:
    void set(std::string key, std::string value)
    {
        if (map.find(key) == map.end())
        {
            map[key] = value;
        }
        else {
            if (std::hash<std::string>{}(map[key]) <= std::hash<std::string>{}(value))
            {
                map[key] = value;
            }
        }
    }
    void del(std::string key)
    {
        tombstone.insert(key);
    }
    std::map<std::string, std::string> get_map() 
    {
        std::map<std::string, std::string> result;
        for (auto it = map.begin(); it != map.end(); it++)
        {
            if (tombstone.find(it->first) == tombstone.end())
            {
                result[it->first] = it->second;
            }
        }
        return result;
    }

private:
    std::map<std::string, std::string> map;
    std::set<std::string> tombstone;
};

int main()
{
    KeyValueCRDT crdt;
    crdt.set("key1", "value1");
    crdt.set("key2", "value2");
    crdt.del("key2");
    crdt.set("key1", "value6");
    crdt.set("key2", "value5");
    std::map<std::string, std::string> result = crdt.get_map();
    for (auto it = result.begin(); it != result.end(); it++)
    {
        std::cout << it->first << " " << it->second << std::endl;
    }
}