cmake_minimum_required(VERSION 3.16)

project(logo2CloudMain
    LANGUAGES CXX
    VERSION 1.0.2
    DESCRIPTION "Project to convert a logo (image) into a point cloud."
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(exporter)

add_subdirectory(gui)

add_subdirectory(logic)




#MESSAGE(${CMAKE_CURRENT_SOURCE_DIR})
#if defined(__linux) || defined(__linux__) || defined(linux)
# define LINUX
#elif defined(__APPLE__)
# define MACOS
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(_WIN64)
# define WINDOWS
#endif
