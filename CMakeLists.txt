cmake_minimum_required(VERSION 3.5)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules/cxxbasics/CXXBasics.cmake")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(search_for_string)

set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost 1.64 REQUIRED COMPONENTS system filesystem)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
endif()

add_executable("${PROJECT_NAME}"
        constants.h
        file_searcher.h
        file_searcher.cpp
        dir_dispatcher.h
        utils.h
        utils.cpp
        main.cpp)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU.*" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang.*")
target_compile_options("${PROJECT_NAME}" PUBLIC
	"-Wall"
	"-Wpedantic"
	"-Wextra"
        "-Werror"
        "-Ofast")
endif(CMAKE_CXX_COMPILER_ID MATCHES "GNU.*" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang.*")

# "-fprofile-arcs"
# "-ftest-coverage")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_link_libraries("${PROJECT_NAME}" Threads::Threads ${Boost_LIBRARIES})

# "-lgcov"
