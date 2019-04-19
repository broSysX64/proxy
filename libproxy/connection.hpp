#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <cstdlib>
#include <cstddef>
#include <iostream>
#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/variant.hpp>
#include <boost/optional.hpp>

#include "parser/postgres/packet.hpp"

namespace proxy {

class connection_manager;
class error;

namespace ip = boost::asio::ip;


class connection : public boost::enable_shared_from_this<connection>, private boost::noncopyable
{
public:
    typedef std::function<proxy::error(const boost::system::error_code&, const size_t&)> handler_read_f;
    typedef std::function<proxy::error(const std::string&, unsigned short)> handler_start_f;
    typedef std::function<proxy::error(void)> handler_close_f;
    typedef std::function<proxy::error(const proxy::parser::postgres::header_psql::header_psql_ptr)> handler_log_f;
    typedef boost::shared_ptr<connection> connection_ptr;
    typedef boost::variant<handler_read_f, handler_start_f, handler_close_f, handler_log_f> connection_handler_f;
    typedef std::unordered_map<log_socket_handler,connection_handler_f> connection_handler_ptr;
    typedef ip::tcp::socket socket_type;

    enum { buffer_size = 1024 };

    explicit connection(boost::asio::io_service& ios);
    virtual ~connection() = default;

    socket_type& local_socket();
    socket_type& server_socket();

    connection_handler_ptr connection_handlers() const { return connection_handlers_; }
private:

    socket_type local_socket_;
    socket_type server_socket_;

    std::array<char, buffer_size> local_buffer_;
    std::array<char, buffer_size> server_buffer_;

    connection_handler_ptr connection_handlers_;

    std::shared_ptr<proxy::parser::postgres::parser_header> local_parser_;

    boost::mutex mtx_;
};


typedef std::shared_ptr<connection> connection_ptr;

} // namespace proxy

#endif // CONNECTION_HPP
