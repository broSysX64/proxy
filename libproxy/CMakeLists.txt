set(libproxy_SRCS
    connection.hpp
    connection.cpp
    connection_manager.hpp
    connection_manager.cpp
    server.hpp
    server.cpp
)

add_library(libproxy STATIC
    ${libproxy_SRCS}
)

target_include_directories(libproxy PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
