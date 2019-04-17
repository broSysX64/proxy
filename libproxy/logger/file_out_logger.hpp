#ifndef FILE_OUT_LOGGER_HPP
#define FILE_OUT_LOGGER_HPP

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

#include "logger.hpp"

using namespace std;

namespace logging {

class error;

class file_out_logger : public logger {
public:
    explicit file_out_logger() = delete;
    file_out_logger(const logging_config_t &config);

    virtual error log(const std::string &message);
    virtual error log(const std::string &message, const log_level level);
    virtual error log(const std::string &message, const log_level level, const log_db);
    virtual error log(const std::string &message, const log_level level, const log_db db, const log_query query);


protected:
    void reopen();
    string file_name;
    ofstream file;
    chrono::seconds reopen_interval;
    chrono::system_clock::time_point last_reopen;

private:
    explicit file_out_logger(const file_out_logger &) noexcept;
    file_out_logger &operator=(const file_out_logger &) noexcept;
};

} // logging

#endif // FILE_OUT_LOGGER_HPP
