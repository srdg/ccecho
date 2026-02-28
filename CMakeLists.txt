cmake_minimum_required(VERSION 3.10)
project(EchoServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
message("CMake build type set as ${CMAKE_BUILD_TYPE}")

add_executable(ccecho
    main.cpp server.cpp
)
target_link_libraries(ccecho PRIVATE ws2_32)
target_include_directories(ccecho PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})


set_target_properties(ccecho PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/.bin"
)
