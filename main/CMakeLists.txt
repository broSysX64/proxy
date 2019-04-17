set(Main_SRCS
    main.cpp
)

add_executable(app
    ${Main_SRCS}
)

target_link_libraries(app
    libproxy
)

install(TARGETS app DESTINATION bin)
