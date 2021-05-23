
#include "DatabaseClientApi.hpp"

#include "data.pb.h"

#include <boost/asio.hpp>


namespace DatabaseClientApi
{

    using boost::asio::ip::tcp;
    boost::asio::io_context io_context;
    tcp::socket socket_(io_context);
    tcp::resolver resolver(io_context);

    void Connect()
    {
        boost::asio::connect(socket_, resolver.resolve("127.0.0.1", "20000"));
    }

    void Disconnect()
    {
        boost::system::error_code ec;
        socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
        socket_.close();
    }


    bool Put(const std::string& key, const std::string& value)
    {
        ProtobufData::Data data;
        data.set_key(key);
        data.set_value(value);
        data.set_action(ProtobufData::Data::Function::Data_Function_put);


        std::string serializedData;
        if (!data.SerializePartialToString(&serializedData))
            std::cout << "serializing data failed " << std::endl;

        serializedData.append("<EOF>");

        boost::asio::write(socket_, boost::asio::buffer(std::move(serializedData)));

        return true;
    }



    std::string Get(const std::string& key)
    {
        ProtobufData::Data data;
        data.set_key(key);
        data.set_action(ProtobufData::Data::Function::Data_Function_get);
        
        std::string serializedData;
        if (!data.SerializePartialToString(&serializedData))
            std::cout << "serializing data failed " << std::endl;

        serializedData.append("<EOF>");
        boost::asio::write(socket_, boost::asio::buffer(std::move(serializedData)));


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

        return replyData.value();
    }





    bool Delete(const std::string& key)
    {
        ProtobufData::Data data;
        data.set_key(key);
        data.set_action(ProtobufData::Data::Function::Data_Function_delete_);

        std::string serializedData;
        if (!data.SerializePartialToString(&serializedData))
            std::cout << "serializing data failed " << std::endl;

        serializedData.append("<EOF>");

        boost::asio::write(socket_, boost::asio::buffer(std::move(serializedData)));

        return true;
    }

}
