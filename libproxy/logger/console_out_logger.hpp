#ifndef CONSOLE_OUT_LOGGER_HPP
#define CONSOLE_OUT_LOGGER_HPP

#include "logger.hpp"
#include "error.hpp"

namespace proxy {
namespace logging {

class console_out_logger : public logger {
public:
    explicit console_out_logger() = delete;
    explicit console_out_logger(const logging_config_t &config);

    error log(const std::string &message) override;
    error log(const std::string &message, const log_level level, const log_query query_ = log_query::NONE) override;

private:
    explicit console_out_logger(const console_out_logger &) noexcept;
    console_out_logger &operator=(const console_out_logger &) noexcept;

protected:
    const std::unordered_map<log_level, std::string, generate_hash> levels_;
    const std::unordered_map<log_query, std::string, generate_hash> query_;
};
} // namespace proxy
} // namespace logging

#endif // CONSOLE_OUT_LOGGER_HPP
