#ifndef SERVER_HPP
#define SERVER_HPP

#include <boost/asio.hpp>
#include <string>
#include "connection.hpp"
#include "connection_manager.hpp"

namespace proxy {

class server
{
public:
  server(const server&) = delete;
  server& operator=(const server&) = delete;

  explicit server(const std::string& address, const std::string& port,
      const std::string& doc_root);

  void run();

private:
  void do_accept();
  void do_await_stop();

  boost::asio::io_service io_service_;
  boost::asio::signal_set signals_;
  boost::asio::ip::tcp::acceptor acceptor_;
  connection_manager connection_manager_;
  boost::asio::ip::tcp::socket socket_;
};

} // namespace proxy


#endif // SERVER_HPP
