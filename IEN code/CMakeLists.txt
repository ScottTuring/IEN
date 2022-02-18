cmake_minimum_required (VERSION 3.4)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# 只允许build tree在 其他目录 下
if(CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
	message(FATAL_ERROR "FATAL: builds must NOT in {SOURCE_DIR}")
endif()

project(ien)
include(./cconfig.cmake)

set(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/lib/${TARGET_NAME}/${BITS}/${CMAKE_BUILD_TYPE}/")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin/${TARGET_NAME}/${BITS}/${CMAKE_BUILD_TYPE}/")
set(COMPILE_PDB_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/pdb/${TARGET_NAME}/${BITS}/${CMAKE_BUILD_TYPE}/")
set(PDB_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/pdb/${TARGET_NAME}/${BITS}/${CMAKE_BUILD_TYPE}/")
include_directories(./3rd/libuv-1.29.1/include)

add_subdirectory(./src/zlib-1.2.11)
add_subdirectory(./src/mbedtls-2.16.1)
add_subdirectory(./src/libuv-1.29.1)
add_subdirectory(./src/sqlite3-3.29.0)
add_subdirectory(./src/log4cplus-2.0.7) 
add_subdirectory(./src/libien)

