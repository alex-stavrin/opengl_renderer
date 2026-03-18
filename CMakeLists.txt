cmake_minimum_required(VERSION 3.10)
project(opengl_renderer)

# Set C++ Version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# GLAD
add_library(glad STATIC "external/glad/src/glad.c")
target_include_directories(glad PUBLIC "external/glad/include")

# GLFW
add_subdirectory(external/glfw)

# stb_image
add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE "external/stb_image")

# Source files
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

# GLM
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE "external/glm")

# ImGUI
file(GLOB IMGUI_SOURCES 
    "external/imgui/*.cpp"
)
add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC "external/imgui")
target_link_libraries(imgui PRIVATE glfw glad)

set(OpenGL_GL_PREFERENCE GLVND) # used to supress warning. No idea what this does
find_package(OpenGL REQUIRED)

# Assimp
# Disable heavy features to keep build fast
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "" FORCE) # Use internal zlib to avoid system dependency

add_subdirectory(external/assimp)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    glfw
    glad
    OpenGL::GL
    stb_image
    glm
    imgui
    assimp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)