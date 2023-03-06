cmake_minimum_required(VERSION 3.24)
project(VoxelRewrite)

set(CMAKE_CXX_STANDARD 20)

set(IMGUI_FILES libs/imgui/imgui.cpp libs/imgui/imgui_demo.cpp libs/imgui/imgui_draw.cpp libs/imgui/imgui_tables.cpp libs/imgui/imgui_widgets.cpp libs/imgui/backends/imgui_impl_glfw.cpp libs/imgui/backends/imgui_impl_opengl3.cpp "src/Util/Timer.h")

add_executable(VoxelRewrite main.cpp ${IMGUI_FILES} src/Core/Application.cpp src/Core/Application.h src/Core/Window.cpp src/Core/Window.h src/Util/Logger.cpp src/Render/Shader.cpp src/Core/Input/Input.cpp src/Core/Input/Input.h src/Core/Input/KeyListener.cpp src/Core/Input/MouseListener.cpp src/Voxeling/World.cpp src/Voxeling/World.h src/Voxeling/Chunk.cpp src/Voxeling/Chunk.h src/Render/Camera.cpp src/Render/Defines.h src/Render/SpriteSheet.cpp src/Render/SpriteSheet.h src/Voxeling/FastNoise.h src/Render/ImGui/ImGuiLayer.cpp src/Render/ImGui/ImGuiLayer.h)
add_compile_definitions(GLEW_STATIC)

# OpenGL
find_package(OpenGL REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/libs/include )
target_link_libraries(VoxelRewrite PUBLIC ${OPENGL_LIBRARIES} )

target_sources(VoxelRewrite PRIVATE libs/include/GL/glew.c libs/include/GL/glew.h)
add_compile_definitions(GLEW_BUILD)

#GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS ON CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES ON CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

add_subdirectory(libs/GLFW)
target_link_libraries(VoxelRewrite PUBLIC glfw)

target_include_directories(VoxelRewrite PUBLIC ${PROJECT_SOURCE_DIR}/libs/GLFW/include)
target_include_directories(VoxelRewrite PUBLIC src libs/stbi/include)
target_include_directories(VoxelRewrite PUBLIC src libs/imgui/include)
