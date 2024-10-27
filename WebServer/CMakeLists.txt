cmake_minimum_required(VERSION 3.10)
project(WebServer)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost REQUIRED)

include_directories(/usr/include/crow)

add_executable(web_server src/main.cpp src/WebServer.cpp)

target_include_directories(web_server PRIVATE Boost::Boost)

target_link_libraries(web_server PRIVATE pthread)