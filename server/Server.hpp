#ifndef _SERVER_SERVER_HPP_
#define _SERVER_SERVER_HPP_

#include <iostream>
#include <boost/asio.hpp>


#include "DatabaseServerApi.hpp"

using boost::asio::ip::tcp;



class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(tcp::socket Socket)  : socket(std::move(Socket)){ }

    void Run()
    {
        std::cout << "running server" << std::endl;
        waitForRequest();
    }


private:

    tcp::socket socket;
    boost::asio::streambuf buffer;
    const std::string end_of_stream{"<EOF>"};
    
    void waitForRequest()
    {
        auto self(shared_from_this());

        boost::asio::async_read_until(socket, buffer, end_of_stream, 
        [this, self](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (!ec)  
            {
                std::string serializedData { 
                    buffers_begin(buffer.data()), 
                    buffers_end(buffer.data()) - end_of_stream.length() 
                };
                buffer.consume(buffer.size());
                
                DatabaseServerApi::HandleRequest(std::move(serializedData), socket);
                DatabaseServerApi::Update();

                waitForRequest();
            }
            else
                std::cout << "error: " << ec << std::endl;;
        
        });
    
    }

};


class Server
{
public:
    Server(boost::asio::io_context& io_context, short port) : acceptor(io_context, tcp::endpoint(tcp::v4(), port))
    {
        do_accept();
    }


private:
    tcp::acceptor acceptor;

     void do_accept()
    {
        acceptor.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket)
        {
            if (!ec)
            {
                std::cout << "creating session on: " << socket.remote_endpoint().address().to_string() << ":" << socket.remote_endpoint().port() << std::endl;
                std::make_shared<Session>(std::move(socket))->Run();
            }
            else 
                std::cout << "error: " << ec.message() << std::endl;

            do_accept();
        });
    }


};


#endif