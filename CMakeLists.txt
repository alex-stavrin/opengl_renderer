cmake_minimum_required(VERSION 3.10)
project(opengl_renderer)

# GLAD
add_library(glad STATIC "external/glad/src/glad.c")
target_include_directories(glad PUBLIC "external/glad/include")

# GLFW
add_subdirectory(external/glfw)

# Source files
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

set(OpenGL_GL_PREFERENCE GLVND) # used to supress warning. No idea what this does
find_package(OpenGL REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    glfw
    glad
    OpenGL::GL
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)