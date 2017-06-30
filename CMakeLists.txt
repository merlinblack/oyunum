cmake_minimum_required(VERSION 2.8)
project(oyunum)

#set(CMAKE_MODULE_PATH_ADDITION ...)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

add_subdirectory(lua)
find_package( allegro 5.0 COMPONENTS main image font primitives )

file(GLOB SRCS src/*.cpp)
file(GLOB HDRS src/*.h manualbind/src/*.h)
include_directories(manualbind/src)
include_directories(${lua_INCLUDE_DIRS})
include_directories(${allegro_INCLUDE_DIRS})

# we don't want to copy if we're building in the source dir
if (NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)

    # list of files for which we add a copy rule
    set(data_SHADOW script.lua)

    foreach(item IN LISTS data_SHADOW)
        message(STATUS ${item})
        add_custom_command(
            OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${item}"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/${item}" "${CMAKE_CURRENT_BINARY_DIR}/${item}"
            DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${item}"
        )
    endforeach()

    # list of directories for which we add a copy rule
    set(data_SHADOW data)

    foreach(item IN LISTS data_SHADOW)
        message(STATUS ${item})
        add_custom_command(
            OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${item}"
            COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/${item}" "${CMAKE_CURRENT_BINARY_DIR}/${item}"
            DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${item}"
        )
    endforeach()
endif()

add_executable(oyunum
    ${SRCS}
    ${HDRS}
    )

target_link_libraries(oyunum
    ${Lua_LIBRARIES}
    ${allegro_LIBRARIES}
    )

add_custom_target(data-target ALL DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/data" "${CMAKE_CURRENT_BINARY_DIR}/script.lua")

include(GitVersioning)
enable_git_versioning(oyunum)
