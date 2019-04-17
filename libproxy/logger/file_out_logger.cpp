#include <memory>

#include "error.h"
#include "file_out_logger.hpp"

namespace logging {

file_out_logger::file_out_logger(const logging_config_t &config)
    : logger(config) {
}

error file_out_logger::log(const std::string &message, const log_level level) {
    q_unused(message);
    q_unused(level);
    return error(error_type::NO_ERROR);
}

error file_out_logger::log(const string &message, const log_level level, const log_db db)
{
    q_unused(message);
    q_unused(level);
    q_unused(db);

    return error(error_type::NO_ERROR);
}

error file_out_logger::log(const string &message, const log_level level, const log_db db, const log_query query)
{
    q_unused(message);
    q_unused(level);
    q_unused(db);
    q_unused(query);

    return error(error_type::NO_ERROR);
}

error file_out_logger::log(const std::string &message)
{
    q_unused(message);

    return error(error_type::NO_ERROR);
}

void file_out_logger::reopen() {
}

} // namespace logging
