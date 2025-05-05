# cmake_minimum_required(VERSION  4.0)
# set(CMAKE_TOOLCHAIN_FILE "C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake")
# project(test_p2p VERSION 1.0.0 LANGUAGES CXX)

# # Set C++20 standard
# set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_EXTENSIONS OFF)


# # Find required packages
# find_package(spdlog CONFIG REQUIRED)
# find_package(glfw3 CONFIG REQUIRED)
# find_package(GLEW REQUIRED)
# find_package(OpenGL REQUIRED)
# find_package(assimp CONFIG REQUIRED)
# find_package(OpenImageIO CONFIG REQUIRED)
# find_package(OpenColorIO CONFIG REQUIRED)


# # Find all cpp files recursively in src directory
# file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# # Create the executable
# add_executable(${PROJECT_NAME} ${SOURCES})

# # Include directories
# target_include_directories(${PROJECT_NAME} PRIVATE 
# 	${CMAKE_CURRENT_SOURCE_DIR}/src 
# 	${CMAKE_CURRENT_SOURCE_DIR}/src/glm
# )

# # Link libraries
# target_link_libraries(${PROJECT_NAME} PRIVATE
# 	spdlog::spdlog
# 	glfw
# 	GLEW::GLEW
# 	OpenGL::GL
# 	assimp::assimp
# 	OpenImageIO::OpenImageIO
# 	OpenColorIO::OpenColorIO
# )


# target_compile_definitions(${PROJECT_NAME} PRIVATE
# 	FMT_SHARED
# )

# if(WIN32)
# 	target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32)
# endif()

# # Optional: Set output directories
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# # Optional: Add compiler-specific flags
# if(MSVC)
# 	target_compile_options(${PROJECT_NAME} PRIVATE /W4)
# else()
# 	target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
# endif()

cmake_minimum_required(VERSION 4.0)

# Use vcpkg toolchain if available
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
endif()

# Project settings
project(test_p2p VERSION 1.0.0 LANGUAGES CXX)

# Fast parallel builds
if(MSVC)
    add_compile_options(/MP) # Multi-core compilation on MSVC
else()
    set(CMAKE_JOB_POOLS_COMPILE compile_pool)
    set(CMAKE_JOB_POOL_COMPILE compile_pool)
endif()

# C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Sources
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# Executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Compile definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE FMT_HEADER_ONLY=1)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/glm
)

# Find packages
find_package(spdlog CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(OpenImageIO CONFIG REQUIRED)
find_package(OpenColorIO CONFIG REQUIRED)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    spdlog::spdlog
    glfw
    GLEW::GLEW
    OpenGL::GL
    assimp::assimp
    OpenImageIO::OpenImageIO
    OpenColorIO::OpenColorIO
)

# Platform-specific linking
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32)
endif()

# Output directories
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

# Compiler warnings
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive-)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()
