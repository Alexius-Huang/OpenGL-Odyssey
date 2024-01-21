cmake_minimum_required(VERSION 3.12)
project(opengl-odyssey)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

set(CMAKE_CXX_STANDARD 20)

# Add the source files of glad to the project
add_library(glad STATIC include/glad.c)
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(
    opengl-odyssey
    main.cpp
    libs/WindowManager.cpp
    libs/Shader.cpp
    # Shader.cpp
    # Transform.cpp
    # BufferObjectManager.cpp
    # EventListener.cpp
    # Camera.cpp
)

# Link Glad
target_link_libraries(opengl-odyssey PRIVATE glad)

# # Include directories for GLEW
# target_include_directories(opengl-odyssey PRIVATE /opt/homebrew/Cellar/glew/2.2.0_1/include)

# # Link GLEW library
# target_link_libraries(opengl-odyssey PRIVATE OpenGL::GL glfw)

# Include directories for GLFW and GLM
target_include_directories(opengl-odyssey PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Link GLFW and OpenGL
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

target_link_libraries(opengl-odyssey PRIVATE OpenGL::GL glfw)

# Copying Shader Files
file(COPY shaders/ DESTINATION ${CMAKE_BINARY_DIR}/shaders/)
