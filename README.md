<h1>Poorly Made SDL2-based platformer - PJATK student project.</h1>

Necessary libs:
---
>/lib directory includes required SDL libraries for ease of build.
<h6>Download official releases @https://github.com/libsdl-org</h6>
>- SDL2
>- SDL2_image
>- SDL2_ttf
>- SDL2_mixer

Compilation
---
>mkdir build <br>
>cd build <br>
>cmake ../

or via CLion IDE</br>
@https://www.jetbrains.com/clion/
>Settings -> Toolchains -> Visual Studio 2019
></br>Settings -> Cmake -> Debug Visual Studio

Known Issues
---
>- Depending on build approach, PATH variable  usage etc. it might be necessary to copy .dll files from libraries into build directory
>- For some reason paths to library directories act somewhat unclear on my machine, without a particular pattern. It might be necessary to change paths to lib directories in  CMakeLists.txt (happend on Windows10)
>:</br>set(SDL2_DIR "/SDL2-2.30.1")</br>
   set(SDL2_IMAGE_DIR "/SDL2_image-2.8.2")</br>
   set(SDL2_TTF_DIR "/SDL2_ttf-2.22.0")</br>
   set(SDL2_MIXER_DIR "/SDL2_mixer-2.8.0")</br>
>^ provide full path to directory here if issue occurs


This project is still WIP
