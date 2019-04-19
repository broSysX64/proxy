#include "connection_manager.hpp"
#include "error.hpp"

namespace proxy {

connection_manager::connection_manager()
{
}

error connection_manager::start(connection_ptr c)
{
  connections_.insert(c);

  return error(error_type::NO_ERROR);
}

error connection_manager::stop(connection_ptr c)
{
  connections_.erase(c);

  return boost::get<connection::handler_close_f>(c->connection_handlers()[log_socket_handler::close_session])();
}

error connection_manager::stop_all()
{
  for (auto c: connections_)
      return boost::get<connection::handler_close_f>(c->connection_handlers()[log_socket_handler::close_session])();;

  return error(error_type::NO_ERROR);
}

} // namespace proxy
