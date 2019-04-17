#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <mutex>
#include <unordered_map>
#include <vector>

#include "log_level.hpp"
#include "error.hpp"

namespace logging {

using logging_config_t = std::unordered_map<std::string, std::string>;
using logging_config_list_t = std::vector<logging_config_t>;

class logger {
public:
    explicit logger() = delete;
    logger(const logging_config_t &config) {
       q_unused(config);
    }
    virtual ~logger() {}
    virtual error log(const std::string &, const log_level) {
        return error(error_type::NO_ERROR);
    }
    virtual error log(const std::string &) {
        return error(error_type::NO_ERROR);
    }

protected:
    std::mutex lock;
};
} // namespace logging

#endif // LOGGER_HPP

