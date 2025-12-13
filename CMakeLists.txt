cmake_minimum_required(VERSION 3.15)
project(graphics_action_scene)

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
# We don't need to build Assimp's tests or install it globally
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(assimp)

# On macOS with modern compilers, Assimp can trigger deprecated-declarations warnings
# Since these are treated as errors, we suppress this specific warning for the assimp target only
if(APPLE)
    target_compile_options(assimp PRIVATE "-Wno-deprecated-declarations")
endif()
# --- End Assimp ---

# General GLAD/GLFW wrapper
# This way, COMMON_SRC = ["common/glad.c", "common/wrapper_glfw.cpp", "common/wrapper_glfw.h"
set(COMMON_SRC
        common/glad.c
        common/wrapper_glfw.cpp
        common/wrapper_glfw.h
        common/Model.cpp
)

# === executable ===
add_executable(graphics_action_scene ${COMMON_SRC} main.cpp)
# Link Assimp to our executable
target_link_libraries(graphics_action_scene PRIVATE ${OPENGL_LIBRARIES} glfw3 assimp)

# Extra libraries based on different OS
if (APPLE)
    target_link_libraries(graphics_action_scene
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

# Copy assets to the build directory (cmake-build-debug)
file(COPY
        shader.vert
        shader.frag
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)

file(COPY
        objects
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)
