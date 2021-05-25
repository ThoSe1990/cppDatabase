

#ifndef _SERVER_DATABASESERVERAPI_HPP_
#define _SERVER_DATABASESERVERAPI_HPP_

#include <string>
#include <fstream>
#include "Database.hpp"
#include "data.pb.h"
#include <boost/asio.hpp>

using boost::asio::ip::tcp;


namespace DatabaseServerApi 
{
    void HandleRequest(const std::string&& serializedData, tcp::socket& socket);
    void Update();

    bool Put(const std::string& key, const std::string& value);
    ProtobufData::Data GetValue(const std::string& key);
    bool Delete(const std::string& key);

    void sendData(ProtobufData::Data data, tcp::socket& socket);

    void SerializeAllValues();
    void WriteDataToDisk(const std::string& key, const std::string& value);
    std::string GetValueFromDisk(const std::string& key);

}


#endif 