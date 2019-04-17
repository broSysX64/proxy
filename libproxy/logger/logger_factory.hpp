#ifndef LOGGER_FACTORY_HPP
#define LOGGER_FACTORY_HPP

#include <functional>
#include <memory>
#include <vector>

#include "logger.hpp"

namespace logging {

class error;

using logger_creator = std::function<logger *(const logging_config_t &)>;
using logger_list_t = std::vector<logger *>;

class logger_factory final {
public:
    explicit logger_factory();
    logger *produce(const logging_config_t &config) const;
    logger_list_t produce(const logging_config_list_t &configs) const;

protected:
    std::unordered_map<std::string, logger_creator> handlers;
};

inline logger_factory &get_factory();
inline logger &get_handler_logger(const logging_config_t &config = {
                                      {"type", "console_out"}, {"color", ""}});

inline logger_list_t get_handler_loggers(const logging_config_list_t &config = {
                                             {{"type", "console_out"}, {"color", ""}},
                                             {{"type", "file_out"},{"file_name", "test.log"},{"reopen_interval", "100"}}});

error configuration(const logging_config_t &config);
error configuration(const logging_config_list_t &configs);

error log(const std::string &message);
error log(const std::string &message, const log_level level);


error TRACE(const std::string &message);
error DEBUG(const std::string &message);
error INFO(const std::string &message);
error WARN(const std::string &message);
error ERROR(const std::string &message);

} // namespace logging
#endif // LOGGER_FACTORY_HPP
