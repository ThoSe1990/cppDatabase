

#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>


#include "DatabaseClientApi.hpp"

std::string random_string(size_t length)
{
    auto randchar = []() -> char
    {
        const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length,0);
    std::generate_n( str.begin(), length, randchar );
    return str;
}


int main(int argc, char* argv[])
{
    
    DatabaseClientApi::Connect();

    while(true)
    {
        std::string s = random_string(10);
        DatabaseClientApi::Put(s,s);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    DatabaseClientApi::Disconnect();

    return 0;
}