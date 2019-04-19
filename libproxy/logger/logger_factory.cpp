#include <algorithm>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "file_out_logger.hpp"
#include "console_out_logger.hpp"
#include "logger_factory.hpp"
#include "error.hpp"

namespace proxy {
namespace logging {

static std::mutex mtx_;
static logger_list_t singletons_;

logger_factory::logger_factory() {
    handlers_output_.emplace("console_out",
                     [](const logging_config_t &config) -> logger * {
                         return new console_out_logger(config);
                     });
    handlers_output_.emplace("file_out", [](const logging_config_t &config) -> logger * {
        return new file_out_logger(config);
    });
}

logger *logger_factory::get_logger_handler(const logging_config_t &config) const
{
    auto type_config = config.find("type");
    if (type_config == config.end())
        throw std::runtime_error("Error configuration type logger");

    auto found = handlers_output_.find(type_config->second);
    if (found != handlers_output_.end())
        return found->second(config);

    throw std::runtime_error("Not found logger for type: " + type_config->second);
}

logger_list_t logger_factory::get_logger_handler(const logging_config_list_t &configs) const
{
    logger_list_t logger_list;
    for (auto config : configs) {
        auto type_config = config.find("type");
        auto found = handlers_output_.find(type_config->second);
        logger_list.push_back(found->second(config));
    }

    if (logger_list.size() == 0)
        throw std::runtime_error("Not found logger type");

    return std::move(logger_list);
}

logger_factory &get_factory()
{
    static logger_factory factory_singleton{};
    return factory_singleton;
}

logger &get_handler_logger(const logging_config_t &config)
{
    static std::unique_ptr<logger> singleton(get_factory().get_logger_handler(config));
    return *singleton;
}

logger_list_t get_handler_loggers(const logging_config_list_t &configs)
{
    if (singletons_.size() == 0)
            std::transform(configs.begin(), configs.end(), back_inserter(singletons_),
                          [](const logging_config_t &config)
                           {
                                return get_factory().get_logger_handler(config);
                           });

    return singletons_;
}

error configuration(const logging_config_t &config)
{
    try {
        get_handler_logger(config);
    } catch (...) {
        return error(error_type::CONFIG_ERROR);
    }

    return error();
}

error configuration(const logging_config_list_t &configs)
{
    try {
        get_handler_loggers(configs);
    } catch (...) {
        return error(error_type::CONFIG_ERROR);
    }

    return error();
}

error log(const std::string &message, const log_level level,const log_query query) {
    mtx_.lock();
    for (auto &&logger : get_handler_loggers()) {
        if (logger)
            if (logger->log(message, level, query).has_error())
                return error(error_type::LOG_ERROR,"log_level: INFO, " + message);
    }
    mtx_.unlock();

    return error();
}

error log(const std::string &message)
{
    mtx_.lock();
    for (auto &&logger : get_handler_loggers()) {
        if (logger)
            if (logger->log(message, log_level::INFO).has_error())
                return error(error_type::LOG_ERROR,"log_level: INFO, " + message);
    }
    mtx_.unlock();

    return error();
}

error TRACE(const std::string &message, const log_query query) {
    mtx_.lock();
    for (auto &&logger : get_handler_loggers()) {
        if (logger)
            if (logger->log(message, log_level::TRACE, query).has_error())
                return error(error_type::LOG_ERROR,"log_level: TRACE, " + message);
    }
    mtx_.unlock();

    return error();
}

error DEBUG(const std::string &message, const log_query query)
{
    mtx_.lock();
    for (auto &&logger : get_handler_loggers()) {
        if (logger)
            if (logger->log(message, log_level::DEBUG, query).has_error())
                return error(error_type::LOG_ERROR, "log_level: DEBUG, " + message);
    }
    mtx_.unlock();

    return error();
}

error INFO(const std::string &message, const log_query query)
{
    mtx_.lock();
    for (auto &&logger : get_handler_loggers()) {
        if (logger)
            if (logger->log(message, log_level::INFO, query).has_error())
                return error(error_type::LOG_ERROR, "log_level: INFO, " + message);
    }
    mtx_.unlock();

    return error();
}

error WARN(const std::string &message, const log_query query) {

    mtx_.lock();
    for (auto &&logger : get_handler_loggers()) {
        if (logger)
            if (logger->log(message, log_level::WARN, query).has_error())
                return error(error_type::LOG_ERROR, "log_level: WARN, " + message);
    }
    mtx_.unlock();

    return error();
}

error ERROR(const std::string &message, const log_query query)
{
    mtx_.lock();
    for (auto &&logger : get_handler_loggers()) {
        if (logger)
            if (logger->log(message, log_level::ERROR, query).has_error())
                return error(error_type::LOG_ERROR, "log_level: ERROR, " + message);
    }
    mtx_.unlock();

    return error();
}

} // namespace logging
} // namespace proxy
