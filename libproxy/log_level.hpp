#ifndef LOG_LEVEL_HPP
#define LOG_LEVEL_HPP

#include <chrono>
#include <unordered_map>

namespace proxy {

enum class log_db : int {
    POSTGRESQL = 0,
};

enum class log_query : int {
    NONE = 0,
    SELECT = 1,
    UPDATE = 2,
    DELETE = 3,
    CREATE = 4,
    INSERT = 4,
    DROP = 5,
};

enum class log_level : int {
    TRACE = 0,
    DEBUG = 1,
    INFO = 2,
    WARN = 3,
    ERROR = 4
};

enum class log_protocol_psql : int {
    NONE = 0,
    QUERY = 1
};

enum class error_type : int {
    NO_ERROR = 0,    
    UNKNOWN_ERROR = 1,
    CONNECTION_ERROR = 2,
    LOG_ERROR = 3,
    CONFIG_ERROR = 4
};

enum class parser_state : int
{
    startup_message,
    commands
};

enum class log_socket_handler : int
{
    local_read = 0,
    server_read = 1,
    start_session = 2,
    close_session = 3,
    parse_log = 4
};

struct generate_hash {
    template <typename T> std::size_t operator()(T t) const {
        return static_cast<std::size_t>(t);
    }
};

const std::unordered_map<log_db, std::string, generate_hash> db{
    {log_db::POSTGRESQL, "postgresql"},
};


const std::unordered_map<log_query, std::string, generate_hash> text_log_query{
    {log_query::NONE,  "NONE"},
    {log_query::SELECT,"SELECT"},
    {log_query::UPDATE,"UPDATE"},
    {log_query::DELETE,"DELETE"},
    {log_query::CREATE,"CREATE"},
    {log_query::INSERT,"INSERT"},
    {log_query::DROP,"DROP"}
};

const std::unordered_map<log_query, std::string, generate_hash> uncolors_log_query{
    {log_query::NONE,  " [NONE] "},
    {log_query::SELECT," [SELECT] "},
    {log_query::UPDATE," [UPDATE] "},
    {log_query::DELETE," [DELETE] "},
    {log_query::CREATE," [CREATE] "},
    {log_query::INSERT," [INSERT]"},
    {log_query::DROP,  " [DROP]"}
};

const std::unordered_map<log_level, std::string, generate_hash> uncolors_log_level{
    {log_level::ERROR," [ERROR] "},
    {log_level::WARN, " [WARN] "},
    {log_level::INFO, " [INFO] "},
    {log_level::DEBUG," [DEBUG] "},
    {log_level::TRACE," [TRACE] "}
};

const std::unordered_map<log_level, std::string, generate_hash> colors_log_level{
    {log_level::WARN,  " \x1b[41;1m[WARN]\x1b[0m "},
    {log_level::INFO,  " \x1b[42;1m[INFO]\x1b[0m "},
    {log_level::DEBUG, " \x1b[43;1m[DEBUG]\x1b[0m "},
    {log_level::TRACE, " \x1b[44;1m[TRACE]\x1b[0m "}
};

const std::unordered_map<log_query, std::string, generate_hash> colors_log_query{
    {log_query::NONE,  " \x1b[45;1m[NONE]\x1b[0m "},
    {log_query::SELECT," \x1b[46;1m[SELECT]\x1b[0m "},
    {log_query::UPDATE," \x1b[47;1m[UPDATE]\x1b[0m "},
    {log_query::DELETE," \x1b[48;1m[DELETE]\x1b[0m "},
    {log_query::CREATE," \x1b[49;1m[CREATE]\x1b[0m "},
    {log_query::INSERT," \x1b[50;1m[INSERT]\x1b[0m "},
    {log_query::DROP,  " \x1b[51;1m[DROP]\x1b[0m "}

};

constexpr log_level REGISTER_LOG_LEVEL = log_level::INFO;

inline std::string get_timestamp() {
    std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
    std::time_t tt = std::chrono::system_clock::to_time_t(tp);
    std::tm gmt_curr{};
    gmtime_r(&tt, &gmt_curr);
    std::chrono::duration<double> fractional_seconds =
        (tp - std::chrono::system_clock::from_time_t(tt)) +
        std::chrono::seconds(gmt_curr.tm_sec);
    std::string current_date("year/mo/dy hr:mn:sc.xxxxxx");
    sprintf(&current_date.front(), "%04d/%02d/%02d %02d:%02d:%09.6f",
            gmt_curr.tm_year + 1900, gmt_curr.tm_mon + 1, gmt_curr.tm_mday, gmt_curr.tm_hour,
            gmt_curr.tm_min, fractional_seconds.count());
    return current_date;
}

template <typename T> void q_unused(T &&) {}

} // namespace proxy

#endif // LOG_LEVEL_HPP
