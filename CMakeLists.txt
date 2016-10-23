cmake_minimum_required(VERSION 3.6)

find_package(CURL REQUIRED)

file(GLOB SOURCES "src/*.c")
file(GLOB HEADERS "src/*.h")

set(CMAKE_BINARY_DIR build)

add_library(vault SHARED ${SOURCES})
target_include_directories(vault INTERFACE ${CURL_INCLUDE_DIRS})
target_link_libraries(vault ${CURL_LIBRARIES})
