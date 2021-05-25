
#if defined _WIN32 || defined __CYGWIN__
    #ifdef _EXPORT_DLL
        #ifdef __GNUC__
            #define DATABASE_API_PUBLIC __attribute__((dllexport))
        #else
            #define DATABASE_API_PUBLIC __declspec(dllexport)
        #endif
    #else
        #ifdef __GNUC__
            #define DATABASE_API_PUBLIC __attribute__((dllimport))
        #else
            #define DATABASE_API_PUBLIC __declspec(dllimport)
        #endif
    #endif
    #define DATABASE_API_HIDDEN
#else
    #if __GNUC__ >= 4
        #define DATABASE_API_PUBLIC __attribute__((visibility("default")))
        #define DATABASE_API_HIDDEN __attribute__((visibility("hidden")))
    #else
        #define DATABASE_API_PUBLIC
        #define DATABASE_API_HIDDEN
    #endif
#endif


#include "DatabaseClientApi.hpp"
#include "data.pb.h"
#include <boost/asio.hpp>


namespace DatabaseClientApi
{
    using boost::asio::ip::tcp;
    boost::asio::io_context io_context;
    tcp::socket socket_(io_context);
    tcp::resolver resolver(io_context);

    DATABASE_API_HIDDEN void sendData(ProtobufData::Data data)
    {
        std::string serializedData;
        if (!data.SerializePartialToString(&serializedData))
            std::cout << "serializing data failed " << std::endl;

        serializedData.append("<EOF>");

        boost::asio::write(socket_, boost::asio::buffer(std::move(serializedData)));
    }

    DATABASE_API_HIDDEN ProtobufData::Data receiveData()
    {
        boost::system::error_code error; 
        boost::asio::streambuf buffer_; 

        boost::asio::read_until(socket_, buffer_, "<EOF>", error ); 

        std::string serializedReply{
                buffers_begin(buffer_.data()),
                buffers_end(buffer_.data()) - 5
            };

        ProtobufData::Data replyData;
        if (!replyData.ParseFromString(serializedReply))
            std::cout << "invalid input message for protobuffer, parse from inputstream failed\n";

        return replyData;
    }







    DATABASE_API_PUBLIC void Connect()
    {
        boost::asio::connect(socket_, resolver.resolve("127.0.0.1", "20000"));
    }

    DATABASE_API_PUBLIC void Disconnect()
    {
        boost::system::error_code ec;
        socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
        socket_.close();
    }


    DATABASE_API_PUBLIC bool Put(const std::string& key, const std::string& value)
    {
        ProtobufData::Data data;
        data.set_key(key);
        data.set_value(value);
        data.set_action(ProtobufData::Data::Function::Data_Function_put);

        sendData(data);

        return true;
    }

    DATABASE_API_PUBLIC void List()
    {
        ProtobufData::Data data;
        data.set_action(ProtobufData::Data::Function::Data_Function_list);
        sendData(data);
    }


    DATABASE_API_PUBLIC std::string Get(const std::string& key)
    {
        ProtobufData::Data data;
        data.set_key(key);
        data.set_action(ProtobufData::Data::Function::Data_Function_get);
        sendData(data);

        ProtobufData::Data replyData = receiveData();

        return replyData.value();
    }



    DATABASE_API_PUBLIC bool Delete(const std::string& key)
    {
        ProtobufData::Data data;
        data.set_key(key);
        data.set_action(ProtobufData::Data::Function::Data_Function_delete_);
        sendData(data);

        return true;
    }



}
