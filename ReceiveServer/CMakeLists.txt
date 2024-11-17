cmake_minimum_required(VERSION 3.10)
project(ReceiveServer)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost REQUIRED)
include_directories(/usr/include/crow)

add_executable(receive_server src/main.cpp src/ReceiveServer.cpp)

target_include_directories(receive_server PRIVATE Boost::Boost)
target_link_libraries(receive_server PRIVATE pthread)



# cmake_minimum_required(VERSION 3.10)
# project(WebSocketServer)

# set(CMAKE_CXX_STANDARD 20)

# if(DEFINED ENV{VCPKG_ROOT})
#     set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
# endif()

# add_executable(websocket_server src/main.cpp src/WebSocketServer.cpp)

# find_package(crow CONFIG REQUIRED)
# target_link_libraries(websocket_server crow::crow)
