cmake_minimum_required(VERSION 3.15)
project(cache_sim)

set(CMAKE_CXX_STANDARD 14)
set(SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/CacheSimulator.h
        ${CMAKE_SOURCE_DIR}/CacheSimulator.cpp
        ${CMAKE_SOURCE_DIR}/CacheStuff.h
        ${CMAKE_SOURCE_DIR}/CacheStuff.cpp
        ${CMAKE_SOURCE_DIR}/main.cpp
        )
add_executable(${PROJECT_NAME} ${SOURCE_FILES})