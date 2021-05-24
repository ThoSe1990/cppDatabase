#include <boost/python.hpp>
#include <boost/python/scope.hpp>

#include "DatabaseClientApi.hpp"


BOOST_PYTHON_MODULE(pythonDatabase)
{

    using namespace boost::python;

    Py_Initialize();

    def("Connect", DatabaseClientApi::Connect);
    def("Disconnect", DatabaseClientApi::Disconnect);
    def("Put", DatabaseClientApi::Put);
    def("Get", DatabaseClientApi::Get);
    def("Delete", DatabaseClientApi::Delete);
    def("List", DatabaseClientApi::List);
}