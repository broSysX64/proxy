#ifndef LOG_LEVEL_HPP
#define LOG_LEVEL_HPP

#include <chrono>
#include <unordered_map>

namespace logging {

enum class log_db : int {
    POSTGRESQL = 0,
    MYSQL = 1,
    MSSQL = 2,
    ORACLE = 3
};

enum class log_query : int {
    SELECT = 0,
    UPDATE = 1,
    DELETE = 2,
    CREATE = 3
};


enum class log_level : int {
    TRACE = 0,
    DEBUG = 1,
    INFO = 2,
    WARN = 3,
    ERROR = 4
};

enum class error_type : int {
    NO_ERROR = 0,
    UNKNOWN_ERROR = 1,
    REGISTER_LOG_LEVEL_ERROR = 2,
    LOG_ERROR = 3,
    CONFIG_ERROR = 4
};

struct generate_hash {
    template <typename T> std::size_t operator()(T t) const {
        return static_cast<std::size_t>(t);
    }
};

const std::unordered_map<log_level, std::string, generate_hash> uncolors{
    {log_level::ERROR, " [ERROR] "},
    {log_level::WARN, " [WARN] "},
    {log_level::INFO, " [INFO] "},
    {log_level::DEBUG, " [DEBUG] "},
    {log_level::TRACE, " [TRACE] "}};

constexpr log_level REGISTER_LOG_LEVEL = log_level::INFO;

template <typename T> void UNSED(T &&) {}

} // namespace logging

#endif // LOG_LEVEL_HPP
