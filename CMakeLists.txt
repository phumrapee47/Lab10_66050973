cmake_minimum_required(VERSION 3.10)
project(OpenGLStarter)

set(CMAKE_CXX_STANDARD 17)

# หา OpenGL, GLEW, GLFW
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLM REQUIRED)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/Libs
    ${OPENGL_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
)

# Source files
set(SOURCE_FILES
    src/main.cpp
    Libs/Mesh.cpp
    Libs/Texture.cpp
    Libs/Shader.cpp
    Libs/Window.cpp
)

# สร้าง executable
add_executable(OpenGLStarter ${SOURCE_FILES})

# Link libraries
target_link_libraries(OpenGLStarter
    ${OPENGL_LIBRARIES}
    GLEW::GLEW
    glfw
)

# Copy Shaders folder
add_custom_target(copy_shaders ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/Shaders
    $<TARGET_FILE_DIR:OpenGLStarter>/Shaders)
add_dependencies(OpenGLStarter copy_shaders)

# Copy Models folder
add_custom_target(copy_models ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/Models
    $<TARGET_FILE_DIR:OpenGLStarter>/Models)
add_dependencies(OpenGLStarter copy_models)

# Copy Textures folder
add_custom_target(copy_textures ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/Textures
    $<TARGET_FILE_DIR:OpenGLStarter>/Textures)
add_dependencies(OpenGLStarter copy_textures)