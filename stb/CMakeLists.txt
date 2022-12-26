cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(Stb)

add_library(stb STATIC include/stb_image.h include/stb_image.c)
target_include_directories(stb PUBLIC include/)