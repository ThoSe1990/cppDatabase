

#include "data.pb.h"
#include <iostream>
#include <fstream>

using namespace std;

int main (int argc, char* argv[])
{
    ProtobufData::Data data;
    data.set_key("any");
    data.set_value("value");

    

    std::ofstream ofs("bin/sensor.data", std::ios_base::out | std::ios_base::binary);
    if (!data.SerializeToOstream(&ofs)) {
      cerr << "Failed to write address book." << endl;
      return -1;
    }

    return 0;
}