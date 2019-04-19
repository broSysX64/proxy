set(Main_SRCS
    main.cpp
)

add_executable(app
    ${Main_SRCS}
)


find_package(Boost 1.65 COMPONENTS system thread program_options serialization REQUIRED)
add_definitions( -DBOOST_ALL_DYN_LINK )
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
include_directories(${Boost_INCLUDE_DIRS})


target_link_libraries(app
    libproxy
    ${Boost_LIBRARIES} -lpthread
)

install(TARGETS app DESTINATION bin)
