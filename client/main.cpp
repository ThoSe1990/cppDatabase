

#include <iostream>
#include <algorithm>
#include <functional>
#include <unistd.h>
#include "DatabaseClientApi.hpp"


std::string random_string(const int len) {
    
    std::string tmp_s;
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    
    srand( (unsigned) time(NULL) * getpid());

    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) 
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    
    
    return tmp_s;
    
}


int main(int argc, char* argv[])
{

    DatabaseClientApi::Connect();

    std::string key = argv[1];
    auto value = DatabaseClientApi::Get(key);
    std::cout << "got value: " << value << std::endl;

    DatabaseClientApi::Delete(key);

    // for (int i = 1 ; i < 1000 ; i++)
    // {
    //     std::string s = random_string(15);
    //     DatabaseClientApi::Put(s, s);
    //     usleep(10000);
    // } 

    DatabaseClientApi::Disconnect();

    return 0;
}