cmake_minimum_required(VERSION 3.10)
project(ReceiveServer)

set(CMAKE_CXX_STANDARD 20)

# Найти Boost и Crow
find_package(Boost REQUIRED)
include_directories(/usr/include/crow)

# Найти библиотеку libpqxx
find_package(PkgConfig REQUIRED)
pkg_check_modules(PQXX REQUIRED libpqxx)

# Добавить исполняемый файл
add_executable(receive_server src/main.cpp src/ReceiveServer.cpp src/PostgresDB.cpp)

# Указать пути к заголовочным файлам и библиотекам
target_include_directories(receive_server 
    PRIVATE 
    Boost::Boost 
    ${PQXX_INCLUDE_DIRS}
)

# Связать библиотеки pthread и libpqxx
target_link_libraries(receive_server 
    PRIVATE 
    pthread 
    ${PQXX_LIBRARIES}
)

# Установить дополнительные параметры компилятора для libpqxx, если необходимо
target_compile_options(receive_server PRIVATE ${PQXX_CFLAGS_OTHER})



# cmake_minimum_required(VERSION 3.10)
# project(WebSocketServer)

# set(CMAKE_CXX_STANDARD 20)

# if(DEFINED ENV{VCPKG_ROOT})
#     set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
# endif()

# add_executable(websocket_server src/main.cpp src/WebSocketServer.cpp)

# find_package(crow CONFIG REQUIRED)
# target_link_libraries(websocket_server crow::crow)
