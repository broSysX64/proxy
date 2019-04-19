#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <mutex>
#include <unordered_map>
#include <vector>

#include "log_level.hpp"
#include "error.hpp"

namespace proxy {
namespace logging {

using logging_config_t = std::unordered_map<std::string, std::string>;
using logging_config_list_t = std::vector<logging_config_t>;

class logger {
public:
    explicit logger() = delete;
    explicit logger(const logging_config_t &config) {
       q_unused(config);
    }
    virtual ~logger() {}

    virtual error log(const std::string &) = 0;
    virtual error log(const std::string &, const log_level level, const log_query  query = log_query::NONE) = 0;
protected:
    std::mutex mtx_;
};
} // namespace logging
} // namespace proxy


#endif // LOGGER_HPP

