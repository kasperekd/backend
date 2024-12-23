cmake_minimum_required(VERSION 3.10)
project(CalculationModule)

set(CMAKE_CXX_STANDARD 20)

find_package(Boost REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PQXX REQUIRED libpqxx)

include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(calculation_module 
    src/main.cpp 
    src/CalculationModule.cpp 
    src/BaseStationCalculator.cpp
)

target_include_directories(calculation_module 
    PRIVATE 
    ${PQXX_INCLUDE_DIRS}
)

target_link_libraries(calculation_module 
    PRIVATE 
    pthread 
    ${PQXX_LIBRARIES}
)

target_compile_options(calculation_module PRIVATE ${PQXX_CFLAGS_OTHER})
