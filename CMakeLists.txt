cmake_minimum_required(VERSION 3.16)
project(w4app)

if (NOT DEFINED ENV{W4_KIT})
    message(FATAL_ERROR [=[
        W4_KIT environment variable is not set
        Run install script first
]=])
endif ()

list(
    APPEND CMAKE_MODULE_PATH
    "$ENV{W4_KIT}/sdk/cmake.modules"
)

include(W4)

w4_define_web_app("${CMAKE_SOURCE_DIR}")
