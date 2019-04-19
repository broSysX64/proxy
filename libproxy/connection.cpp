#include "connection.hpp"
#include "connection_manager.hpp"

#include <utility>
#include <vector>
#include <memory>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/variant.hpp>
#include "error.hpp"

#include "logger/logger.hpp"
#include "logger/logger_factory.hpp"

namespace proxy {

connection::connection(boost::asio::io_service &ios)
    : local_socket_(ios),
      server_socket_  (ios),
      local_parser_ (std::make_shared<proxy::parser::postgres::parser_header>())
{
    handler_start_f start_session_ptr = [this](const std::string& upstream_host, unsigned short upstream_port)
    {
        auto self(shared_from_this());

        server_socket_.async_connect(
            ip::tcp::endpoint(boost::asio::ip::address::from_string(upstream_host),upstream_port),
            [this, self](const boost::system::error_code& error) -> proxy::error {
                if (!error)
                {
                    auto self(shared_from_this());

                    server_socket_.async_read_some(
                        boost::asio::buffer(server_buffer_, buffer_size),
                        [this, self](const boost::system::error_code& error, size_t bytes_transferred) -> proxy::error {
                            return boost::get<handler_read_f>(connection_handlers_[log_socket_handler::server_read])(error, bytes_transferred);
                        });

                    local_socket_.async_read_some (
                        boost::asio::buffer(local_buffer_, buffer_size),
                        [this, self](const boost::system::error_code& error, size_t bytes_transferred) -> proxy::error {
                            return boost::get<handler_read_f>(connection_handlers_[log_socket_handler::local_read])(error, bytes_transferred);
                        });
                }
                else
                    return boost::get<handler_close_f>(connection_handlers_[log_socket_handler::close_session])();

                return proxy::error(error_type::NO_ERROR);
            });

        return error(error_type::NO_ERROR);
    };

    handler_close_f close_session_ptr = [this](void) {
        boost::mutex::scoped_lock lock(mtx_);

        if (local_socket_.is_open())
            local_socket_.close();

        if (server_socket_.is_open())
            server_socket_.close();

        return error(error_type::NO_ERROR);
    };


    handler_log_f handler_log = [this](const proxy::parser::postgres::header_psql::header_psql_ptr packet) {
        if (packet->command() == log_protocol_psql::QUERY)
        {
            auto find_result = std::find_if(text_log_query.begin(), text_log_query.end() , [&packet](std::pair<log_query, std::string > element){
                auto find = (packet->query().find(element.second) != std::string::npos);
                return find;
            });

            if (packet->query().empty())
                return proxy::error();

            if (find_result != std::end(text_log_query))
                logging::INFO(packet->query(), find_result->first);
            else
                logging::INFO(packet->query());
        }
        return proxy::error();
    };


    handler_read_f server_read_ptr = [this](const boost::system::error_code& error, const size_t& bytes_transferred) {
        if (!error)
        {
            auto self(shared_from_this());

            async_write(local_socket_,
                        boost::asio::buffer(server_buffer_,bytes_transferred),
                        [this, self](const boost::system::error_code& error, size_t /*bytes_transferred*/) -> proxy::error
                        {
                            if (!error)
                            {
                                auto self(shared_from_this());

                                server_socket_.async_read_some(
                                    boost::asio::buffer(server_buffer_, buffer_size),
                                    [this, self](const boost::system::error_code& error, size_t bytes_transferred) -> proxy::error {
                                        return boost::get<handler_read_f>(connection_handlers_[log_socket_handler::server_read])(error, bytes_transferred);
                                    });
                            }
                            else
                                return boost::get<handler_close_f>(connection_handlers_[log_socket_handler::close_session])();

                            return proxy::error(error_type::NO_ERROR);
                        });
        }
        else
            return boost::get<handler_close_f>(connection_handlers_[log_socket_handler::close_session])();

        return proxy::error(error_type::NO_ERROR);
    };

    handler_read_f local_read_ptr = [this](const boost::system::error_code& error, const size_t& bytes_transferred)
    {
        if (!error)
        {
            auto packet = local_parser_->parse_buffer(local_buffer_.data(), bytes_transferred);
            boost::get<handler_log_f>(connection_handlers_[log_socket_handler::parse_log])(packet);

            auto self(shared_from_this());
            async_write(server_socket_,
                        boost::asio::buffer(local_buffer_, bytes_transferred),
                        [this, self](const boost::system::error_code& error, size_t /*bytes_transferred*/) -> proxy::error {
                            if (!error)
                            {
                                auto self(shared_from_this());

                                local_socket_.async_read_some(
                                    boost::asio::buffer(local_buffer_, buffer_size),
                                    [this, self](const boost::system::error_code& error, size_t bytes_transferred) -> proxy::error {
                                        return boost::get<handler_read_f>(connection_handlers_[log_socket_handler::local_read])(error, bytes_transferred);
                                    });
                            }
                            else
                                return boost::get<handler_close_f>(connection_handlers_[log_socket_handler::close_session])();

                            return proxy::error(error_type::NO_ERROR);
                        });
        }
        else
            return boost::get<handler_close_f>(connection_handlers_[log_socket_handler::close_session])();

        return proxy::error(error_type::NO_ERROR);
    };


    connection_handlers_.insert(std::make_pair(log_socket_handler::server_read,server_read_ptr));
    connection_handlers_.insert(std::make_pair(log_socket_handler::local_read,local_read_ptr));
    connection_handlers_.insert(std::make_pair(log_socket_handler::start_session,start_session_ptr));
    connection_handlers_.insert(std::make_pair(log_socket_handler::close_session,close_session_ptr));
    connection_handlers_.insert(std::make_pair(log_socket_handler::parse_log,handler_log));
}

connection::socket_type &connection::local_socket()
{
    return local_socket_;
}

connection::socket_type &connection::server_socket()
{
    return server_socket_;
}

} // namespace proxy
