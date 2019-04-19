cmake_minimum_required(VERSION 2.8)

project(proxy_server)

add_subdirectory(main)
add_subdirectory(libproxy)

enable_testing()
add_subdirectory(test)
