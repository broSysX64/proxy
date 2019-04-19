set(libproxy_SRCS
    connection.hpp
    connection.cpp
    handler.hpp
    handler.cpp
    log_level.hpp
    error.hpp
    error.cpp
    connection_manager.hpp
    connection_manager.cpp
    logger/logger.hpp
    logger/logger_factory.hpp
    logger/logger_factory.cpp
    logger/file_out_logger.hpp
    logger/file_out_logger.cpp
    logger/console_out_logger.hpp
    logger/console_out_logger.cpp
    parser/postgres/packet.hpp
)

add_library(libproxy STATIC
    ${libproxy_SRCS}
)

target_include_directories(libproxy PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
