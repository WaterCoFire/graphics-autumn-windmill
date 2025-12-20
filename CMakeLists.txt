cmake_minimum_required(VERSION 3.15)
project(graphics_autumn_windmill)

set(CMAKE_CXX_STANDARD 17)

# Public include directories
include_directories(common include include/glad include/GLFW include/glm include/KNR)

# Detect OS
if (WIN32)
    message(STATUS ">>> Building on Windows <<<")
    # Static link libgcc and libstdc++ to avoid DLL dependencies
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -static")
    # glfw-3.4.bin.WIN64 is in D:\. Change accordingly if any changes take place.
    set(GLFW_DIR "D:/glfw-3.4.bin.WIN64")
    include_directories(${GLFW_DIR}/include)
    link_directories(${GLFW_DIR}/lib-mingw-w64)  # If using MSVC, change to lib-vc2022 or lib-vc2023

elseif (APPLE)
    message(STATUS ">>> Building on macOS <<<")
    # GLFW & GLM installed via Homebrew in macOS
    include_directories(/opt/homebrew/include)
    link_directories(/opt/homebrew/lib)
endif ()

# Find OpenGL
find_package(OpenGL REQUIRED)

# --- Assimp ---
# Use FetchContent to download and build Assimp automatically
include(FetchContent)
FetchContent_Declare(
        assimp
        GIT_REPOSITORY https://github.com/assimp/assimp.git
        GIT_TAG v5.2.5 # Using a stable version
)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(assimp)

# On macOS with modern compilers, Assimp can trigger deprecated-declarations warnings
# Since these are treated as errors, we suppress this specific warning for the assimp target only
if(APPLE)
    target_compile_options(assimp PRIVATE "-Wno-deprecated-declarations")
endif()
# --- End Assimp ---

# --- ImGui ---
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.89.9
)
FetchContent_MakeAvailable(imgui)

# Find ImGui headers
include_directories(
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
)
# --- End ImGui ---

# General GLAD/GLFW wrapper
# This way, COMMON_SRC = ["common/glad.c", "common/wrapper_glfw.cpp", ...]
set(COMMON_SRC
        common/glad.c
        common/wrapper_glfw.cpp
        common/wrapper_glfw.h
        common/model.cpp
        common/particle.cpp

        # ImGui Sources
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

# === executable ===
add_executable(graphics_autumn_windmill ${COMMON_SRC} main.cpp)
# Link Assimp to our executable
target_link_libraries(graphics_autumn_windmill PRIVATE ${OPENGL_LIBRARIES} glfw3 assimp)

# Extra libraries based on different OS
if (APPLE)
    target_link_libraries(graphics_autumn_windmill
            PRIVATE
            "-framework Cocoa"
            "-framework IOKit"
            "-framework CoreFoundation"
            "-framework CoreGraphics"
            "-framework AppKit"
            "-framework CoreVideo"
    )
elseif (WIN32)
    #    target_link_libraries(basic
    #            PRIVATE
    #            opengl32
    #    )
endif ()

# Copy all assets to the build directory (cmake-build-debug)
file(COPY
        shader.vert
        shader.frag
        skybox.vert
        skybox.frag
        particle.vert
        particle.frag
        objects
        textures
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)
