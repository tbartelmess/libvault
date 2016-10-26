cmake_minimum_required(VERSION 3.6)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")


find_package(CURL REQUIRED)

find_package(Check REQUIRED)


file(GLOB SOURCES "src/*.c")
file(GLOB HEADERS "src/*.h")

IF(${CMAKE_C_COMPILER_ID} STREQUAL "Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Weverything")
ELSE()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-unknown-pragmas")
ENDIF()

add_library(vault SHARED ${SOURCES})
target_include_directories(vault PRIVATE ${CURL_INCLUDE_DIRS})
target_link_libraries(vault ${CURL_LIBRARIES})

add_library(vault_static STATIC ${SOURCES})
set_target_properties(vault_static PROPERTIES OUTPUT_NAME vault)
target_include_directories(vault_static PRIVATE ${CURL_INCLUDE_DIRS})
target_link_libraries(vault_static ${CURL_LIBRARIES})


enable_testing()
file(GLOB TEST_SOURCES "test/*.c")
add_executable(vault_tests ${TEST_SOURCES})
target_include_directories(vault_tests PRIVATE ${CHECK_INCLUDE_DIR})
set_target_properties(vault_tests PROPERTIES COMPILE_FLAGS "-Wno-missing-prototypes")
target_link_libraries(vault_tests vault)
target_link_libraries(vault_tests ${CHECK_LIBRARIES})
add_test(vault_tests vault_tests)
