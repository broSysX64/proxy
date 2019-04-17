set(libproxy_SRCS
    connection.hpp
    connection.cpp
    connection_manager.hpp
    connection_manager.cpp
    server.hpp
    server.cpp
    log_level.hpp
    error.hpp
    error.cpp
    logger/logger.hpp
    logger/logger_factory.hpp
    logger/logger_factory.cpp
    logger/file_out_logger.hpp
    logger/file_out_logger.cpp

    parser/postgres/packet.hpp

)

add_library(libproxy STATIC
    ${libproxy_SRCS}
)

target_include_directories(libproxy PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
