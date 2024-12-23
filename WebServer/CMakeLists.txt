cmake_minimum_required(VERSION 3.10)
project(WebServer)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PQXX REQUIRED libpqxx)

include_directories(/usr/include/crow)
include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(web_server src/main.cpp src/WebServer.cpp)

target_include_directories(web_server PRIVATE Boost::Boost ${PQXX_INCLUDE_DIRS})

target_link_libraries(web_server PRIVATE pthread ${PQXX_LIBRARIES})

target_compile_options(web_server PRIVATE ${PQXX_CFLAGS_OTHER})