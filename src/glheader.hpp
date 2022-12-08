#pragma once

#ifdef _WIN32
#include <glad/glad.h>
#elif _WIN64
#include <glad/glad.h>
#elif __CYGWIN__
#include <glad/glad.h>
#elif __ANDROID__
#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>
#elif __linux__
#include <glad/glad.h>
#elif _UNIX
#include <glad/glad.h>
#elif __APPLE__
#include <glad/glad.h>
#endif





