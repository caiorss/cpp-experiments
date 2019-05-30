#include <iostream>
#include <string>
#include <functional>
#include <memory>
#include <ctime>

#include <boost/asio.hpp>

namespace ba = boost::asio;
using boost::asio::ip::tcp;

int main()
{

    boost::asio::io_context ioctx;

    // TCP/IPV4, port 9060
    tcp::acceptor acceptor(ioctx, tcp::endpoint(tcp::v4(), 9060));

    std::string text = "hello world server";

    for (;;) {
        tcp::socket socket(ioctx);
        std::cout << " [INFO] Waiting incoming connection ..." << std::endl;
        acceptor.accept(socket);
        std::cout << " [INFO] Connection accepted OK."  << std::endl;
        boost::asio::write(socket, boost::asio::buffer(text));
    }

    return 0;
}
