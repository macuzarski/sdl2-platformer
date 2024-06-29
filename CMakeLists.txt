cmake_minimum_required(VERSION 3.24)
project(cowabunger)

set(CMAKE_CXX_STANDARD 17)


set(SDL2_DIR "/SDL2-2.30.1")
set(SDL2_IMAGE_DIR "/SDL2_image-2.8.2")
set(SDL2_TTF_DIR "/SDL2_ttf-2.22.0")
set(SDL2_MIXER_DIR "/SDL2_mixer-2.8.0")

include_directories(${SDL2_DIR}/include ${SDL2_IMAGE_DIR}/include ${SDL2_TTF_DIR}/include ${SDL2_MIXER_DIR}/include)

link_directories(${SDL2_DIR}/lib/x64 ${SDL2_IMAGE_DIR}/lib/x64 ${SDL2_TTF_DIR}/lib/x64 ${SDL2_MIXER_DIR}/lib/x64)

add_executable(${PROJECT_NAME} main.cpp
        defs.h
        Character.h
        LTexture.h
        LTexture.cpp
        Character.cpp
        LTimer.h
        LTimer.cpp
        LButton.h
        LButton.cpp
        Game.cpp
)

target_link_libraries(${PROJECT_NAME}
        ${SDL2_DIR}/lib/x64/SDL2.lib
        ${SDL2_DIR}/lib/x64/SDL2main.lib
        ${SDL2_IMAGE_DIR}/lib/x64/SDL2_image.lib
        ${SDL2_TTF_DIR}/lib/x64/SDL2_ttf.lib
        ${SDL2_MIXER_DIR}/lib/x64/SDL2_mixer.lib
)
