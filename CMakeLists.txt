cmake_minimum_required(VERSION 3.10)
project(EchoServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
message("CMake build type set as ${CMAKE_BUILD_TYPE}")

add_executable(echo_server
    main.cpp server.cpp
)
target_link_libraries(echo_server PRIVATE ws2_32)
target_include_directories(echo_server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})


set_target_properties(echo_server PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/.bin"
)
