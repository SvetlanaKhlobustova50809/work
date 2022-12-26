cmake_minimum_required(VERSION 3.5)
project(landscape)

set(CMAKE_CXX_STANDARD 11)
set(CMALE_CXX_FLAGS "-O3")
set(DEPEN_DIR "depen")

file(GLOB_RECURSE glm
    ${DEPEN_DIR}/glm/*.h
    ${DEPEN_DIR}/glm/*.cpp
    ${DEPEN_DIR}/glm/*.hpp
)


file(GLOB_RECURSE shaders
    ${DEPEN_DIR}/shaders/*.glsl
)

file(GLOB_RECURSE textures
    ${DEPEN_DIR}/textures/*.png
    ${DEPEN_DIR}/textures/*.bmp
)

file(GLOB sources
    include/*.h
    include/*.hpp
    include/*.cpp
    include/*.c
)

include_directories("include" "depen" "depen/GLAD" "sources")

add_executable(${PROJECT_NAME} ${glm} ${glad} ${glfw} ${sources} ${include})
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE C)


link_directories("static")


add_subdirectory(glad)
add_subdirectory(glfw)
add_subdirectory(stb)


set_target_properties(${EDITOR_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

target_link_libraries(${PROJECT_NAME} glfw stb)



