cmake_minimum_required(VERSION 3.10)
project(ReceiveServer)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost REQUIRED)
include_directories(/usr/include/crow)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PQXX REQUIRED libpqxx)

add_executable(receive_server src/main.cpp src/ReceiveServer.cpp src/PostgresDB.cpp)

target_include_directories(receive_server 
    PRIVATE 
    Boost::Boost 
    ${PQXX_INCLUDE_DIRS}
)

target_link_libraries(receive_server 
    PRIVATE 
    pthread 
    ${PQXX_LIBRARIES}
)

target_compile_options(receive_server PRIVATE ${PQXX_CFLAGS_OTHER})