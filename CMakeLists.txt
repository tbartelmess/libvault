cmake_minimum_required(VERSION 3.6)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")


find_package(CURL REQUIRED)
find_package(Jansson REQUIRED)

find_package(Check REQUIRED)
find_package(minizip REQUIRED)


file(GLOB SOURCES "src/*.c")
file(GLOB HEADERS "src/*.h")

IF(${CMAKE_C_COMPILER_ID} STREQUAL "Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Weverything")
ELSE()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-unknown-pragmas")
ENDIF()

set(VAULT_VERSION "0.6.2")
set(VAULT_DOWNLOAD_BASE "https://releases.hashicorp.com/vault")
string(TOLOWER "${CMAKE_HOST_SYSTEM_NAME}" VAULT_OS)

IF(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "x86_64")
    # Some systems report x86_64 as the CPU some report
    # amd64.
    set(VAULT_ARCH "amd64")
ELSE()
    set(VAULT_ARCH "${CMAKE_HOST_SYSTEM_PROCESSOR}")
ENDIF()

set(VAULT_ARCHIVE_NAME "vault_${VAULT_VERSION}_${VAULT_OS}_${VAULT_ARCH}.zip")
set(VAULT_DOWNLOAD_URL "${VAULT_DOWNLOAD_BASE}/${VAULT_VERSION}/${VAULT_ARCHIVE_NAME}")


add_library(vault SHARED ${SOURCES})
target_include_directories(vault PRIVATE ${CURL_INCLUDE_DIRS})
target_link_libraries(vault ${CURL_LIBRARIES})
target_link_libraries(vault ${JANSSON_LIBRARIES})

add_library(vault_static STATIC ${SOURCES})
set_target_properties(vault_static PROPERTIES OUTPUT_NAME vault)
target_include_directories(vault_static PRIVATE ${CURL_INCLUDE_DIRS})
target_link_libraries(vault_static ${CURL_LIBRARIES})
target_link_libraries(vault_static ${JANSSON_LIBRARIES})


enable_testing()

set(VAULT_DOWNLOAD_PATH "/tmp")
set(VAULT_DOWNLOAD_ARCHIVE "${VAULT_DOWNLOAD_PATH}/${VAULT_ARCHIVE_NAME}")
set(VAULT_DOWNLOAD_EXECUTABLE "${VAULT_DOWNLOAD_PATH}/vault_${VAULT_VERSION}")

configure_file(test/vault_version.h.in test/vault_version.h)

file(GLOB TEST_SOURCES "test/*.c")
add_executable(vault_tests ${TEST_SOURCES})
target_include_directories(vault_tests PRIVATE ${CHECK_INCLUDE_DIR})
set_target_properties(vault_tests PROPERTIES COMPILE_FLAGS "-Wno-missing-prototypes")
target_link_libraries(vault_tests vault)
target_link_libraries(vault_tests ${CHECK_LIBRARIES})
target_link_libraries(vault_tests ${MINIZIP_LIBRARIES})
target_include_directories(vault_tests PRIVATE ${CURL_INCLUDE_DIRS})
target_include_directories(vault_tests PRIVATE ${MINIZIP_INCLUDE_DIRS})
target_link_libraries(vault_tests ${CURL_LIBRARIES})
add_test(vault_tests vault_tests)
