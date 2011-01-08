cmake_minimum_required(VERSION 2.8)

project(Ants CXX)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../cmake")

add_definitions(-O2 -fsigned-char -freg-struct-return -Wshadow)

include_directories(.)

set(CMAKE_OSX_ARCHITECTURES i386)

set(ASSET_DIR ${CMAKE_SOURCE_DIR}/../assets)

# Automatically find all .cpp files in the folder
file(GLOB ALL_CPP_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
if (ALL_CPP_FILES)
  foreach(CPP_FILE IN LISTS ALL_CPP_FILES)
  	message(STATUS "Adding file: ${CPP_FILE}")
  endforeach()
endif()

set(SMFL_DEBUG true)
find_package(SFML REQUIRED COMPONENTS System Window Graphics)

add_executable(ants ${ALL_CPP_FILES})
target_link_libraries(ants ${SFML_LIBRARY})

execute_process(COMMAND rm -rf ${CMAKE_BINARY_DIR}/assets)
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/../assets
                                                           ${CMAKE_BINARY_DIR}/assets)


message(STATUS "Copying 'assets' directory")
