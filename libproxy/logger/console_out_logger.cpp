#include <string>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "console_out_logger.hpp"
#include "error.h"

namespace proxy {
namespace logging {

console_out_logger::console_out_logger(const logging::logging_config_t &config)
    : logger(config),
      levels_(config.find("color") != config.end() ? colors_log_level : uncolors_log_level),
      query_(config.find("color") != config.end() ? colors_log_query : uncolors_log_query){
}

error console_out_logger::log(const std::string &message,
                              const log_level level, const log_query query) {
    if (level < REGISTER_LOG_LEVEL)
        return error(error_type::NO_ERROR);

    std::string msg;
    msg.reserve(message.length() + 64);
    msg.append(get_timestamp());
    msg.append(levels_.find(level)->second);
    auto search = query_.find(query);
    if (search != query_.end())
        msg.append(query_.find(query)->second);
    msg.append(message);
    msg.push_back('\n');
    log(msg);

    return error(error_type::NO_ERROR);
}

error console_out_logger::log(const std::string &message) {
    std::cout << message;
    std::cout.flush();

    return error(error_type::NO_ERROR);
}

} // namespace logging
} // namespace proxy
