#ifndef _CLIENT_DATABASECLIENTAPI_HPP_
#define _CLIENT_DATABASECLIENTAPI_HPP_

#include <string>

namespace DatabaseClientApi
{
    void Connect();
    void Disconnect();
    bool Put(const std::string& key, const std::string& value);
    std::string Get(const std::string& key);
    bool Delete(const std::string& key);

}


#endif 