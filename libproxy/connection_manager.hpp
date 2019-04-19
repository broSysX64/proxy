#ifndef CONNECTION_MANAGER_HPP
#define CONNECTION_MANAGER_HPP

#include <set>
#include "connection.hpp"

namespace proxy {

class error;

class connection_manager
{
public:
  connection_manager(const connection_manager&) = delete;
  connection_manager& operator=(const connection_manager&) = delete;

  explicit connection_manager();

  error start(connection_ptr c);
  error stop(connection_ptr c);
  error stop_all();

private:
  std::set<connection_ptr> connections_;
};

} // namespace server

#endif // CONNECTION_MANAGER_HPP
