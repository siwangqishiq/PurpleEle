# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.18.1)

# Declares and names the project.
project("textrender")

set(DIR "../../../../..") #工程目录
set(SRC_DIR "../../../../../src")  #源码

include_directories(${SRC_DIR})
include_directories(${DIR}/include) #添加include目录
include_directories(include)

add_library( # Sets the name of the library.
        textrender
        SHARED
        libapp.cpp
        ${SRC_DIR}/application.cpp
        ${DIR}/libjson/json.cpp
        ${SRC_DIR}/utils.cpp
        ${SRC_DIR}/render/shader.cpp
        ${SRC_DIR}/render/triangle.cpp
        ${SRC_DIR}/render/render.cpp
        ${SRC_DIR}/render/render_batch.cpp
        ${SRC_DIR}/render/sprite.cpp
        ${SRC_DIR}/render/command.cpp
        ${SRC_DIR}/render/vram.cpp
        ${SRC_DIR}/resource/asset_manager.cpp
        ${SRC_DIR}/render/texture.cpp
        ${SRC_DIR}/widget/timer.cpp
        ${SRC_DIR}/audio/audio.cpp
        ${SRC_DIR}/game/test_demo.cpp
        ${SRC_DIR}/game/shader_demo.cpp
        )

set(opengl_lib GLESv3)

find_library(log_lib log)

target_link_libraries( # Specifies the target library.
        textrender
        ${log_lib}
        ${opengl_lib}
        android
        jnigraphics
        )