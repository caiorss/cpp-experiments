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
#include <boost/asio.hpp>

#define TRACE(text) \
     std::cerr << " [TRACE] " << __FILE__ << ":" << __LINE__ << ":" \
     << "  " << "<" << __FUNCTION__ << "> - " << text << std::endl


using boost::asio::ip::tcp;

class session
  : public std::enable_shared_from_this<session>
{
public:
  session(tcp::socket socket)
    : socket_(std::move(socket))
  {
  }

  void start()
  {
    TRACE("Staring session");
    do_read();
  }

private:
  void do_read()
  {

    TRACE("Reading message");
    auto self(shared_from_this());
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
        [this, self](boost::system::error_code ec, std::size_t length)
        {
          if (!ec)
          {
            do_write(length);
            TRACE("Wiriting finished");
          }
        });
  }

  void do_write(std::size_t length)
  {
    TRACE("Writing message");
    auto self(shared_from_this());
    boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
        [this, self](boost::system::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {
            do_read();
          }
        });
  }

  tcp::socket socket_;
  static constexpr size_t max_length = 1024 ;
  char data_[max_length];
};

class server
{
public:
  server(boost::asio::io_context& io_context, short port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
  {
    TRACE("Create server");
    do_accept();
  }

private:
  tcp::acceptor acceptor_;

  void do_accept()
  {
    TRACE("Waiting connection");
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket)
        {
          if (!ec)
          {
            TRACE("Connection accepted OK");
            std::make_shared<session>(std::move(socket))->start();
          }

          do_accept();
        });
  }
};

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: async_tcp_echo_server <port>\n";
      return 1;
    }

    boost::asio::io_context io_context;

    server s(io_context, std::atoi(argv[1]));

    io_context.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
