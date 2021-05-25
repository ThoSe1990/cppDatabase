

cmake_minimum_required(VERSION 3.16)

project(Database VERSION 1.0.0.0)

set(PROJECT_ROOT_DIR ${PROJECT_SOURCE_DIR})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)


include(FindProtobuf)
find_package(Protobuf REQUIRED)


find_package(Boost COMPONENTS program_options REQUIRED)


add_subdirectory(${PROJECT_SOURCE_DIR}/server)
add_subdirectory(${PROJECT_SOURCE_DIR}/client)    
add_subdirectory(${PROJECT_SOURCE_DIR}/python_api)    
