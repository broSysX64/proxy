#ifndef LOGGER_FACTORY_HPP
#define LOGGER_FACTORY_HPP

#include <functional>
#include <memory>
#include <vector>

#include "logger.hpp"

class error;

namespace proxy {
namespace logging {

using logger_creator_f = std::function<logger *(const logging_config_t &)>;
using logger_list_t = std::vector<logger *>;

class logger_factory final {
public:
    explicit logger_factory();
    logger *get_logger_handler(const logging_config_t &config) const;
    logger_list_t get_logger_handler(const logging_config_list_t &configs) const;

    static logger_list_t singletons_;
protected:
    std::unordered_map<std::string, logger_creator_f> handlers_output_;

};

inline logger_factory &get_factory();

inline logger &get_handler_logger(const logging_config_t &config = {
                                      {"type", "console_out"}, {"color", ""}});

inline logger_list_t get_handler_loggers(const logging_config_list_t &config = {
                                             {{"type", "console_out"}, {"color", ""}},
                                             {{"type", "file_out"},{"file_name", "output.log"},{"reopen_interval", "1000"}}});

error configuration(const logging_config_t &config);
error configuration(const logging_config_list_t &configs);

error log(const std::string &message);
error log(const std::string &message, const log_level level, const log_query query = log_query::NONE);

error TRACE(const std::string &message, const log_query query = log_query::NONE);
error DEBUG(const std::string &message, const log_query query = log_query::NONE);
error INFO(const std::string &message, const log_query query = log_query::NONE);
error WARN(const std::string &message, const log_query query = log_query::NONE);
error ERROR(const std::string &message, const log_query query = log_query::NONE);

} // namespace logging
} // namespace proxy
#endif // LOGGER_FACTORY_HPP
