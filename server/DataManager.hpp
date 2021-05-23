
#ifndef _SERVER_DATAMANAGER_HPP_
#define _SERVER_DATAMANAGER_HPP_

#include <iostream>
#include <vector>
#include <unordered_map>
#include <cassert>
#include <algorithm>



template<typename Key, typename Value>
class DataManager
{
private:
    std::vector<Value> values;
    std::vector<Key> keys;
    std::unordered_map<Key, size_t> lookup;

    std::mutex databaseMutex;
public:

    Value Add(Key key)
    {
        assert(lookup.find(key) == lookup.end());
        assert(keys.size() == values.size());
        assert(lookup.size() == values.size());

		lookup[key] = values.size();

        std::lock_guard<std::mutex> guard(databaseMutex);

        values.push_back();
        keys.push_back(key);
        
        return values.back();
    }

    template <typename... TArgs>
    Value Add(Key key, TArgs&&... args)
    {
        assert(lookup.find(key) == lookup.end());
        assert(keys.size() == values.size());
        assert(lookup.size() == values.size());

		lookup[key] = values.size();

        std::lock_guard<std::mutex> guard(databaseMutex);

        values.push_back(std::move(std::forward<TArgs>(args)...));
        keys.push_back(key);

        std::cout << values.size() << " " << key << " value " << values.back() << std::endl;

        return values.back();
    }

    void Remove(Key key)
    {
        std::cout << "Deleting: " << key << std::endl;

        auto it = lookup.find(key);
        if (it != lookup.end())
        {
            std::lock_guard<std::mutex> guard(databaseMutex);

            const size_t index = it->second;
            const Key key = keys[index];

            if (index < values.size() - 1)
            {
                values[index] = std::move(values.back());
                keys[index] = keys.back();
                lookup[keys[index]] = index;
            }
            values.pop_back();
            keys.pop_back();
            lookup.erase(key);

            std::cout << "Deleting: " << key << std::endl;
        }
    }

    void MoveItem(size_t indexFrom, size_t indexTo)
    {
        assert(indexFrom < GetCount());
        assert(indexTo < GetCount());
        if (indexFrom == indexTo)
            return;
        
        std::lock_guard<std::mutex> guard(databaseMutex);


        std::shared_ptr<Value> value = std::move(values[indexFrom]);
        Key key = keys[indexFrom];

        const int direction = indexFrom < indexTo ? 1 : -1;
        for (size_t i = indexFrom; i != indexTo; i += direction)
        {
            const size_t next = i + direction;
            values[i] = std::move(values[next]);
            keys[i] = keys[next];
            lookup[keys[i]] = i;
        }
        values[indexTo] = std::move(value);
        keys[indexTo] = key;
        lookup[key] = indexTo;
    }


    bool Has(Key key) const
    {
        return lookup.find(key) != lookup.end();
    }

    Value GetValue(Key key) const
    {
        auto it = lookup.find(key);
        if (it != lookup.end())
        {
            return values[it->second];
        }
        return nullptr;
    }


    const Value GetValue(Key key)
    {
        auto it = lookup.find(key);
        if (it != lookup.end())
        {
            return values[it->second];
        }
        return nullptr;
    }

    void Clear()
    {
        values.clear();
        keys.clear();
    }

    std::shared_ptr<Value> operator[](size_t index) const
    {
        return values[index];
    }
    std::shared_ptr<Value> at(size_t index) const
    { 
        return values[index]; 
    }
    size_t GetCount() const 
    { 
        return values.size();
    }
    size_t GetIndex(Key key) const 
    {
        return lookup[key];
    }
    Key GetKey(size_t index) const 
    { 
        return keys[index];
    }

};

#endif