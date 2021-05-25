

#include "DatabaseServerApi.hpp"

using boost::asio::ip::tcp;

#include <mutex>
#include <filesystem>

namespace DatabaseServerApi 
{
    std::mutex mutex;

    void HandleRequest(const std::string&& serializedData, tcp::socket& socket)
    {
        std::lock_guard<std::mutex> guard(mutex);

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
            ListSerializedValues();
        }

    }

    void Update()
    {
        std::lock_guard<std::mutex> guard(mutex);
        auto& db = Database::GetInstance();

        if (db.StringDataManager->GetDatabseSize() > Constants::MaxDatabaseSize)
            SerializeAllValues();

    }


    void SerializeAllValues()
    {
        auto& db = Database::GetInstance();
        std::cout << "serializing: " << db.StringDataManager->GetCount() << " keys/values to disk: " << Constants::serializedDataLocation << std::endl;

        for (int i = 0 ; i < db.StringDataManager->GetCount() ; i++)
        {           
            std::string key = db.StringDataManager->GetKey(i);
            std::string value = db.StringDataManager->at(i);
            WriteDataToDisk(key, value);
        }
        db.StringDataManager->Clear();
    }





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
        
        std::string value = db.StringDataManager->GetValue(key);
        
        if (value.empty())
            value = GetValueFromDisk(key);
            

        reply.set_value(value);
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

    


    void WriteDataToDisk(const std::string& key, const std::string& value)
    {
        ProtobufData::Data data;
        data.set_key(key);
        data.set_value(value);

        std::string path = Constants::serializedDataLocation;
        path.append(key);
        
        std::fstream output(path, std::ios::out | std::ios::trunc | std::ios::binary);
        if (!data.SerializeToOstream(&output)) 
            std::cout << "error during writing data to disk " << key << " " << value << std::endl;
    }

    std::string GetValueFromDisk(const std::string& key)
    {
        for (const auto & d : std::filesystem::directory_iterator(Constants::serializedDataLocation))
        {
            std::filesystem::path file(d);
            std::string filename = file.stem();
            if (filename.compare(key) == 0)
            {
                std::fstream input(file, std::ios::in | std::ios::binary);
                ProtobufData::Data data;
                if (!data.ParseFromIstream(&input)) std::cout << "error parsing file: " << d << std::endl;

                return data.value();
            }
        }
        return "";
    }


    void ListSerializedValues()
    {
        std::cout << std::endl << std::endl << " **** serialized keys/values ****" << std::endl;
        for (const auto & d : std::filesystem::directory_iterator(Constants::serializedDataLocation))
        {
            std::filesystem::path file(d);
            std::fstream input(file, std::ios::in | std::ios::binary);
            ProtobufData::Data data;
            if (!data.ParseFromIstream(&input)) std::cout << "error parsing file: " << d << std::endl;

            std::cout << "key: " << data.key() << " value: " << data.value() << std::endl;

        }
    }


}
