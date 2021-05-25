
#include <iostream>
#include <string>

#include <boost/program_options.hpp>
#include <boost/asio.hpp>

#include "Server.hpp"
#include "Database.hpp"
#include "DatabaseServerApi.hpp"

namespace po = boost::program_options;


int main(int argc, char* argv[])
{
    po::variables_map vm;

    po::options_description desc{"simple database showcase"};
    desc.add_options()
    ("help,h", "Help screen")
    ("port,p", po::value<short>()->default_value(20000), "port for client / server communication")
    ;

    try
    {           
        po::store(parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help"))
        {
            std::cout << desc << std::endl;
            return 1;
        }
    }
    catch (const po::error& er)
    {
        std::cout << er.what() << std::endl;
        return -1;
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return -1;
    }

    boost::asio::io_context io_context;

    Server s(io_context, vm["port"].as<short>());

    io_context.run();

    return 0;
}