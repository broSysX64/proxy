#ifndef FILE_OUT_LOGGER_HPP
#define FILE_OUT_LOGGER_HPP

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

#include "logger.hpp"

using namespace std;

namespace proxy {
namespace logging {

class file_out_logger : public logger {
public:
    explicit file_out_logger() = delete;
    explicit file_out_logger(const logging_config_t &config);

    error log(const std::string &message) override;
    error log(const std::string &message, const log_level level, const log_query query = log_query::NONE) override;

protected:
    void reopen_file();
    string file_name;
    ofstream file_stream;
    chrono::seconds reopen_duration;
    chrono::system_clock::time_point last_reopen_duration;

private:
    explicit file_out_logger(const file_out_logger &) noexcept;
    file_out_logger &operator=(const file_out_logger &) noexcept;
};

} // logging
} // proxy

#endif // FILE_OUT_LOGGER_HPP
