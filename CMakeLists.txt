include(FetchContent)
cmake_minimum_required(VERSION 3.18)
project(OpenGLPong LANGUAGES C)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Set the build type to Debug
set(CMAKE_BUILD_TYPE Debug)
# Set asset path

# Use vcpkg toolchain
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "")
endif()

# Collect all source and header files
set(SOURCE_FILES
    "OpenGLPong/Source/OpenGLPong.c"
 "OpenGLPong/Source/main.c")
include_directories("OpenGLPong/Includes")



# Add the main executable
add_executable(OpenGLPong ${SOURCE_FILES} ${HEADER_FILES})

# Set the C++ standard to 23 if CMake version is greater than 3.12
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET OpenGLPong PROPERTY CXX_STANDARD 23 C)
endif()

include_directories(${CMAKE_SOURCE_DIR}/src/Includes)

# Find and link OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(OpenGLPong PRIVATE OpenGL::GL)




# Find and link GLEW
find_package(GLEW CONFIG REQUIRED)
target_link_libraries(OpenGLPong PRIVATE GLEW::GLEW)

# Find and link GLFW
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(OpenGLPong PRIVATE glfw)

find_package(glm CONFIG REQUIRED)
target_link_libraries(OpenGLPong PRIVATE glm::glm)

FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG        658f48e72aee3c6582e80b05ac0f8787a64fe6bb #v2.0.6
    GIT_PROGRESS   TRUE
    SOURCE_SUBDIR  cmake
)
set(CMAKE_CXX_FLAGS_OLD "${CMAKE_CXX_FLAGS}")

# Add other flags here.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --on-demand")

FetchContent_MakeAvailable(glad)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_OLD}")
glad_add_library(glad_gl_core_43 STATIC REPRODUCIBLE LOADER API gl:core=4.3)
target_link_libraries(OpenGLPong PRIVATE glad_gl_core_43)
