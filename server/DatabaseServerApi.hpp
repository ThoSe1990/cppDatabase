

#ifndef _SERVER_DATABASESERVERAPI_HPP_
#define _SERVER_DATABASESERVERAPI_HPP_

#include <string>
#include "Database.hpp"
#include "data.pb.h"
using boost::asio::ip::tcp;


namespace DatabaseServerApi 
{
    bool Put(const std::string& key, const std::string& value)
    {
        auto& db = Database::GetInstance();

        if (db.StringDataManager->Has(key)) return false;
        
        db.StringDataManager->Add(std::move(key), std::move(value));
        return true;
    }

    ProtobufData::Data GetValue(const std::string& key)
    {
        auto& db = Database::GetInstance();

        ProtobufData::Data reply;
        reply.set_value( db.StringDataManager->GetValue(key) );
        reply.set_key(key);

        return reply;
    }

    bool Delete(const std::string& key)
    {
        auto& db = Database::GetInstance();
        db.StringDataManager->Remove(key);
    }



    void HandleRequest(const std::string&& serializedData, tcp::socket& socket)
    {
        ProtobufData::Data data;
        if (!data.ParseFromString(serializedData))
            std::cout << "error during parsing" << std::endl;


        if (data.action() == ProtobufData::Data::Function::Data_Function_put)
        {
            DatabaseServerApi::Put(std::move(data.key()) , std::move(data.value()));
        }
        else if (data.action() == ProtobufData::Data::Function::Data_Function_get)
        {
            auto reply = DatabaseServerApi::GetValue(std::move(data.key()));
            
            std::cout << "requested value: " << reply.value();

            std::string serializedData;
            if (!reply.SerializePartialToString(&serializedData))
                std::cout << "serializing data failed " << std::endl;

            serializedData.append("<EOF>");
            boost::asio::write(socket, boost::asio::buffer(std::move(serializedData)));
        }
        else if (data.action() == ProtobufData::Data::Function::Data_Function_delete_)
        {
            DatabaseServerApi::Delete(std::move(data.key()));
        }
    }
}


#endif 