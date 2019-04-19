#include <memory>

#include "error.h"
#include "file_out_logger.hpp"

namespace proxy {
namespace logging {

file_out_logger::file_out_logger(const logging_config_t &config)
    : logger(config) {
    auto name = config.find("file_name");
    if (name == config.end())
        throw std::runtime_error("No output file");
    file_name = name->second;

    reopen_duration = std::chrono::seconds(10);
    auto interval = config.find("reopen_interval");
    if (interval != config.end()) {
        try {
            reopen_duration = std::chrono::seconds(std::stoul(interval->second));
        } catch (...) {
            throw std::runtime_error(interval->second +
                                     " is not a valid reopen interval");
        }
    }
    reopen_file();
}

error file_out_logger::log(const std::string &message, const log_level level,const log_query query) {
    if (level < REGISTER_LOG_LEVEL)
        return error(error_type::NO_ERROR);

    std::string msg;
    msg.reserve(message.length() + 64);
    msg.append(get_timestamp());
    msg.append(uncolors_log_level.find(level)->second);

    auto search = uncolors_log_query.find(query);
    if (search != uncolors_log_query.end())
        msg.append(uncolors_log_query.find(query)->second);

    msg.append(message);
    msg.push_back('\n');
    log(msg);

    return error(error_type::NO_ERROR);
}

error file_out_logger::log(const std::string &message) {
    try {
        mtx_.lock();
        file_stream << message;
        file_stream.flush();
        mtx_.unlock();
        reopen_file();
    }
    catch (std::exception &e) {
        return error(error_type::LOG_ERROR, e.what());
    }

    return error(error_type::NO_ERROR);
}

void file_out_logger::reopen_file() {
    auto now = std::chrono::system_clock::now();
    mtx_.lock();
    if (now - last_reopen_duration   > reopen_duration) {
        last_reopen_duration = now;
        try {
            file_stream.close();
        } catch (...) {
        }
        try {
            file_stream.open(file_name, std::ofstream::out | std::ofstream::app);
            last_reopen_duration = std::chrono::system_clock::now();
        } catch (std::exception &e) {
            try {
                file_stream.close();
            } catch (...) {
            }
            throw e;
        }
    }
    mtx_.unlock();
}

} // namespace proxy
} // namespace logging
