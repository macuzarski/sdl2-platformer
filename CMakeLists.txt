cmake_minimum_required(VERSION 3.24)
project(cowabunger)

set(CMAKE_CXX_STANDARD 17)


set(SDL2_DIR "E:/sdl2-project/sdl2-platformer/lib/SDL2-2.30.1")
set(SDL2_IMAGE_DIR "E:/sdl2-project/sdl2-platformer/lib/SDL2_image-2.8.2")

include_directories(${SDL2_DIR}/include ${SDL2_IMAGE_DIR}/include)

link_directories(${SDL2_DIR}/lib/x64 ${SDL2_IMAGE_DIR}/lib/x64)

add_executable(${PROJECT_NAME} main.cpp
        defs.h
)

target_link_libraries(${PROJECT_NAME}
        ${SDL2_DIR}/lib/x64/SDL2.lib
        ${SDL2_DIR}/lib/x64/SDL2main.lib
        ${SDL2_IMAGE_DIR}/lib/x64/SDL2_image.lib
)
