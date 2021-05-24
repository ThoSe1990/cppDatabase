

#ifndef _SERVER_DATABASESERVERAPI_HPP_
#define _SERVER_DATABASESERVERAPI_HPP_

#include <string>
#include <fstream>
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
    
    
    void sendData(ProtobufData::Data data, tcp::socket& socket)
    {
        std::string serializedData;
        if (!data.SerializePartialToString(&serializedData))
            std::cout << "serializing data failed " << std::endl;

        serializedData.append("<EOF>");

        boost::asio::write(socket, boost::asio::buffer(std::move(serializedData)));
    }

    void HandleRequest(const std::string&& serializedData, tcp::socket& socket)
    {
        ProtobufData::Data data;
        if (!data.ParseFromString(serializedData))
            std::cout << "error during parsing" << std::endl;



        if (data.action() == ProtobufData::Data::Function::Data_Function_put)
        {
            Put(std::move(data.key()) , std::move(data.value()));
        }
        else if (data.action() == ProtobufData::Data::Function::Data_Function_get)
        {
            auto reply = GetValue(std::move(data.key()));
            sendData(reply, socket);
        }
        else if (data.action() == ProtobufData::Data::Function::Data_Function_delete_)
        {
            Delete(std::move(data.key()));
        }
        else if (data.action() == ProtobufData::Data::Function::Data_Function_list)
        {
            Database::GetInstance().StringDataManager->List();
        }

    }

    void WriteDataToDisk(const std::string& key, const std::string& value)
    {
        ProtobufData::Data data;
        data.set_key(key);
        data.set_value(value);
        std::string path = "/media/sf_sf_ubuntu/Database/build/bin/serializedData/";
        path.append(key);
        path.append(".data");
        std::fstream output(path, std::ios::out | std::ios::trunc | std::ios::binary);
        if (!data.SerializeToOstream(&output)) 
            std::cout << "error during writing data to disk " << key << " " << value << std::endl;
    }

    void Update()
    {
        auto& db = Database::GetInstance();

        if (db.StringDataManager->GetDatabseSize() > Constants::MaxDatabaseSize)
        {
            for (int i = 0 ; i < 35 ; i++)
            {           
                std::string key = db.StringDataManager->GetKey(i);
                std::string value = db.StringDataManager->at(i);
                WriteDataToDisk(key, value);
                Delete(key);
            }
        }
    }



}


#endif 