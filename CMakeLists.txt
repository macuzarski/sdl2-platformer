cmake_minimum_required(VERSION 3.24)
project(cowabunger)

set(CMAKE_CXX_STANDARD 17)

set(SDL2_INCLUDE_DIR "E:\\SDL2-2.30.1\\include")
set(SDL2_LIB_DIR "E:\\SDL2-2.30.1\\lib\\x64")


include_directories(${SDL2_INCLUDE_DIR})
link_directories(${SDL2_LIB_DIR})
add_executable(${PROJECT_NAME} main.cpp
        defs.h
)
target_link_libraries(${PROJECT_NAME} ${SDL2_LIB_DIR}/SDL2.lib ${SDL2_LIB_DIR}/SDL2main.lib)