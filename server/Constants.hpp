
#ifndef _SERVER_CONSTANTS_HPP_
#define _SERVER_CONSTANTS_HPP_

#include <string>

class Constants
{
public:
    class DatabaseTypes 
    {
    public:
        static const std::string fifo;
        static const std::string lru;
    };
    
};


#endif