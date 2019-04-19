#ifndef HANDLER_HPP
#define HANDLER_HPP

#include "connection.hpp"

namespace proxy {

class error;

class handler
{
public:
    explicit handler(boost::asio::io_service& io_service,
             const std::string& local_host, unsigned short local_port,
             const std::string& upstream_host, unsigned short upstream_port);

    error do_accept();

private:
    boost::asio::io_service& io_service_;
    ip::address_v4 localhost_;
    ip::tcp::acceptor acceptor_;
    connection::connection_ptr session_;

    unsigned short server_port_;
    std::string server_host_;
};

} //namespace proxy


#endif // HANDLER_HPP
