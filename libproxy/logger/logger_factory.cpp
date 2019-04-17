#include <algorithm>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "file_out_logger.hpp"
#include "logger_factory.hpp"

namespace logging {

static std::mutex lock; // sync thread

logger_factory::logger_factory() {
}

logger *logger_factory::produce(const logging_config_t &config) const
{
    auto type_config = config.find("type");
    if (type_config == config.end())
        throw std::runtime_error("Error configuration type logger");

    auto found = handlers.find(type_config->second);
    if (found != handlers.end())
        return found->second(config);

    throw std::runtime_error("Not found logger for type: " + type_config->second);
}

logger_list_t
logger_factory::produce(const logging_config_list_t &configs) const
{
    logger_list_t logger_list;
    for (auto config : configs) {
        auto type_config = config.find("type");
        auto found = handlers.find(type_config->second);
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
    static std::unique_ptr<logger> singleton(get_factory().produce(config));
    return *singleton;
}

logger_list_t get_handler_loggers(const logging_config_list_t &configs)
{
    static logger_list_t singletons;

    for (auto config : configs)
        singletons.push_back(get_factory().produce(config));

    return std::move(singletons);
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

error log(const std::string &message, const log_level level) {
    lock.lock();
    std::for_each(get_handler_loggers().begin(), get_handler_loggers().end(),
                  [message, &level] (logger *logger) { logger->log(message, level); });
    lock.unlock();

    return error();
}

error log(const std::string &message)
{
    lock.lock();
    std::for_each(get_handler_loggers().begin(), get_handler_loggers().end(),
                  [message](logger *logger) { logger->log(message); });
    lock.unlock();

    return error();
}

error TRACE(const std::string &message) {
    lock.lock();
    for (auto &&logger : get_handler_loggers()) {
        if (logger)
            if (logger->log(message, log_level::TRACE).has_error())
                return error(error_type::LOG_ERROR,"log_level: TRACE, " + message);
    }
    lock.unlock();

    return error();
}

error DEBUG(const std::string &message)
{
    lock.lock();
    for (auto &&logger : get_handler_loggers()) {
        if (logger)
            if (logger->log(message, log_level::DEBUG).has_error())
                return error(error_type::LOG_ERROR, "log_level: DEBUG, " + message);
    }
    lock.unlock();

    return error();
}

error INFO(const std::string &message)
{
    lock.lock();
    for (auto &&logger : get_handler_loggers()) {
        if (logger)
            if (logger->log(message, log_level::INFO).has_error())
                return error(error_type::LOG_ERROR, "log_level: INFO, " + message);
    }
    lock.unlock();

    return error();
}

error WARN(const std::string &message) {

    lock.lock();
    for (auto &&logger : get_handler_loggers()) {
        if (logger)
            if (logger->log(message, log_level::WARN).has_error())
                return error(error_type::LOG_ERROR, "log_level: WARN, " + message);
    }
    lock.unlock();

    return error();
}

error ERROR(const std::string &message)
{
    lock.lock();
    for (auto &&logger : get_handler_loggers()) {
        if (logger)
            if (logger->log(message, log_level::ERROR).has_error())
                return error(error_type::LOG_ERROR, "log_level: ERROR, " + message);
    }
    lock.unlock();

    return error();
}

} // namespace logging
