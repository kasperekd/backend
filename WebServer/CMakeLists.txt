cmake_minimum_required(VERSION 3.10)
project(WebServer)

set(CMAKE_CXX_STANDARD 20)

# Указываем путь к Crow
set(CROW_INCLUDE_DIR "/crow/include")

# Создание исполняемого файла
add_executable(web_server src/main.cpp src/WebServer.cpp)

# Линкуем Crow к проекту
target_include_directories(web_server PRIVATE ${CROW_INCLUDE_DIR})
