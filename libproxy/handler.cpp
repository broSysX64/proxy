#include "handler.hpp"
#include "error.hpp"

namespace proxy {

handler::handler(boost::asio::io_service &io_service,
                   const std::string &local_host, unsigned short local_port,
                   const std::string &server_host, unsigned short server_port)
    : io_service_(io_service),
      localhost_(boost::asio::ip::address_v4::from_string(local_host)),
      acceptor_(io_service_,ip::tcp::endpoint(localhost_,local_port)),
      server_port_(server_port),
      server_host_(server_host)
{}

error handler::do_accept()
{
    try
    {
        session_ = boost::shared_ptr<connection>(new connection(io_service_));
        acceptor_.async_accept(session_->local_socket(),
                               [this](const boost::system::error_code& error) -> proxy::error
                               {
                                   if (!error)
                                   {
                                       boost::get<connection::handler_start_f>(session_->connection_handlers()[log_socket_handler::start_session])(server_host_,server_port_);

                                       if (do_accept().has_error())
                                           return proxy::error(error_type::CONNECTION_ERROR);
                                   }
                                   else
                                       return proxy::error(error_type::CONNECTION_ERROR, error.message());

                                   return proxy::error(error_type::NO_ERROR);
                               });
    }
    catch(std::exception& e)
    {
        return proxy::error(error_type::CONNECTION_ERROR, e.what());
    }

    return proxy::error(error_type::NO_ERROR);
}

} // namespace proxy
