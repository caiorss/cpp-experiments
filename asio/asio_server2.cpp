//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using berr = boost::system::error_code;

#define TRACE(text) \
     std::cerr << " [TRACE] " << __FILE__ << ":" << __LINE__ << ":" \
     << "  " << text << std::endl

std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}

class tcp_connection
  : public std::enable_shared_from_this<tcp_connection>
{
public:
  using pointer = std::shared_ptr<tcp_connection>;

  static pointer
  create(boost::asio::io_context& io_context)
  {
    return pointer(new tcp_connection(io_context));
  }

  tcp::socket&
  socket()
  {
    return socket_;
  }

  void start()
  {
    message_ = make_daytime_string();

    #if 0
    boost::asio::async_write(socket_, boost::asio::buffer(message_),
        boost::bind(&tcp_connection::handle_write, this->shared_from_this(),
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
   #endif

    #if 1
    boost::asio::async_write(socket_, boost::asio::buffer(message_),
                             [=](berr const& error, size_t bytes){
                                 this->handle_write(error, bytes);
                                 //return c;
                             });
    #endif

  }

private:
  tcp_connection(boost::asio::io_context& io_context)
    : socket_(io_context)
  {
  }

  void handle_write(const boost::system::error_code& /*error*/,
      size_t /*bytes_transferred*/)
  {
  }

  tcp::socket socket_;
  std::string message_;

}; //--- End of class tcp_connection --- //


class tcp_server
{
public:

  tcp_server(boost::asio::io_context& io_context)
    : m_ioctx(io_context),
      m_acceptor(io_context, tcp::endpoint(tcp::v4(), 9060))
  {
    this->start_accept();
  }

private:

  void start_accept()
  {
    tcp_connection::pointer conn = tcp_connection::create(m_ioctx);

    TRACE(" start_accept - Waiting connection");

    m_acceptor.async_accept(conn->socket(), [=](berr const& error)
    {
        this->handle_accept(conn, error);
    });

  }

  void handle_accept(tcp_connection::pointer new_connection, berr const& error)
  {
     TRACE(" handle_accept - connection accepted. OK");

    if (!error)
    {
      new_connection->start();
    }

    // Accept next connection
    start_accept();
  }

  boost::asio::io_context& m_ioctx;
  tcp::acceptor            m_acceptor;
};

int main()
{
  try
  {
    boost::asio::io_context io_context;
    tcp_server server(io_context);
    io_context.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "  Exception: " << e.what() << std::endl;
    TRACE("\n Error - Shutdown server.");
  }

  return 0;
}
