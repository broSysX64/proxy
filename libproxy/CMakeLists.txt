set(libproxy_SRCS
    connection.hpp
    connection.cpp
    connection_manager.hpp
    connection_manager.cpp
    server.hpp
    server.cpp
    log_level.hpp
    logger.hpp
    logger_factory.hpp
    logger_factory.cpp
    error.hpp
    error.cpp
    file_out_logger.hpp
    file_out_logger.cpp
)

add_library(libproxy STATIC
    ${libproxy_SRCS}
)

target_include_directories(libproxy PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
