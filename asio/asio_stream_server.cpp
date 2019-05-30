//  http://think-async.com/Asio/boost_asio_1_12_2/doc/html/boost_asio/example/cpp11/echo/async_tcp_echo_server.cpp
//
// async_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <array>
#include <boost/asio.hpp>

#define TRACE(text) \
     std::cerr << " [TRACE] " << __FILE__ << ":" << __LINE__ << ":" \
     << "  " << "<" << __FUNCTION__ << "> - " << text << std::endl


using boost::asio::ip::tcp;

class session
  : public std::enable_shared_from_this<session>
{
private:
    tcp::socket m_socket;
    static constexpr size_t max_length = 1024 ;
    char m_data[max_length];
    boost::asio::streambuf m_input_buffer;
    boost::asio::streambuf m_output_buffer;

public:
  session(tcp::socket socket)
    : m_socket(std::move(socket))
  {
  }

  void start()
  {
    TRACE("Staring session");
    this->receive();
    //this->send();
  }

private:

  #if 1
  void receive()
  {

    TRACE("Reading message");
    auto self(shared_from_this());

    // Red until find a "\n" new line delimiter
    boost::asio::async_read_until(m_socket, boost::asio::buffer(m_data, max_length), "\n",
        [this, self](boost::system::error_code ec, std::size_t length)
        {
          TRACE("Processing client request");
          if(ec) return;
          //std:: iss(&m_sbuffer);
           //send(line);
          TRACE("Read finished");

#if 0
          std::size_t nbytes = boost::asio::buffer_copy(
                       m_output_buffer.prepare(m_input_buffer.size()),
                       m_input_buffer.data());
          m_output_buffer.commit(nbytes);
#endif
          this->send();

        });
  }
  #endif

  void send()
  {
    TRACE("Writing message");
    auto self(shared_from_this());

    #if 1
    {
        std::ostream ss(&m_output_buffer);
        ss << " - World Boost ASIO\n";
    }
   #endif

    boost::asio::async_write(m_socket, boost::asio::buffer(m_data, max_length),
        [this, self](boost::system::error_code ec, std::size_t /*length*/)
        {
          TRACE("Echo sent back OK");
          if (ec) return;
          // receive();
         // this->m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
         // this->m_socket.close();
          TRACE("Connection closed");

        });
  }

};


class server
{
    tcp::acceptor acceptor_;

public:
  server(boost::asio::io_context& io_context, std::uint16_t port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
  {
    TRACE("Create server");
    do_accept();
  }

private:  

  void do_accept()
  {
    TRACE("Waiting connection");
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket)
        {
          if (ec){
              TRACE("Connection rejected.");
              return;
          }
          TRACE("Connection accepted OK");
          std::make_shared<session>(std::move(socket))->start();
          do_accept();
        });
  }
};

int main(int argc, char* argv[])
{
  try
  {
    boost::asio::io_context io_context;
    server s(io_context, 9040);
    // server s(io_context, std::atoi(argv[1]));

    io_context.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
