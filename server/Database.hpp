
#ifndef _SERVER_DATABASE_HPP_
#define _SERVER_DATABASE_HPP_

#include <memory>

#include "Constants.hpp"
#include "DataManager.hpp"


class Database
{
public:
    Database(Database const&) = delete;
    void operator=(Database const&) = delete;

    static Database& GetInstance() noexcept
    {
        static Database instance; 
        return instance;
    }

    void Update(const std::string& type)
    {
        if (type.compare(Constants::DatabaseTypes::lru) == 0)
            updateLru();
        else if (type.compare(Constants::DatabaseTypes::fifo) == 0)
            updateFifo();
    }

    std::unique_ptr<DataManager<std::string, std::string>> StringDataManager = std::make_unique<DataManager<std::string, std::string>>();


private:
    Database() { }

    void updateFifo()
    {

    }

    void updateLru()
    {

    }
};






#endif