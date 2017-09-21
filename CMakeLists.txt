cmake_minimum_required(VERSION 2.8)
project(oyunum)

set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

#set(CMAKE_MODULE_PATH_ADDITION ...)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

add_subdirectory(lua)
find_package( allegro 5.0 COMPONENTS main image font color ttf primitives )

file(GLOB SRCS src/*.cpp manualbind/src/*.cpp)
file(GLOB exe_GAME src/boot.cpp)
file(GLOB exe_KOSH src/kosh.cpp)
list(REMOVE_ITEM SRCS ${exe_GAME} ${exe_KOSH})

file(GLOB HDRS src/*.h manualbind/src/*.h)
include_directories(
    manualbind/src
    ${Lua_INCLUDE_DIRS}
    ${allegro_INCLUDE_DIRS}
    )

# we don't want to copy if we're building in the source dir
if (NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)

    # list of directories for which we add a copy rule
    set(data_SHADOW scripts data)

    foreach(item IN LISTS data_SHADOW)
        message(STATUS "Configure directory: " ${item})
        add_custom_command(
            OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${item}"
            COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/${item}" "${CMAKE_CURRENT_BINARY_DIR}/${item}"
            DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${item}"
        )
    endforeach()
endif()

add_library (oyunum_common
    ${SRCS}
    ${HDRS}
    )

add_executable(oyunum
    ${exe_GAME}
    )

target_link_libraries(oyunum
    oyunum_common
    ${Lua_LIBRARIES}
    ${allegro_LIBRARIES}
    )

add_executable(kosh
    ${exe_KOSH}
    )

target_link_libraries(kosh
    oyunum_common
    ${Lua_LIBRARIES}
    ${allegro_LIBRARIES}
    )

set_property(TARGET oyunum PROPERTY CXX_STANDARD 11)
set_property(TARGET oyunum PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET oyunum_common PROPERTY CXX_STANDARD 11)
set_property(TARGET oyunum_common PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET kosh PROPERTY CXX_STANDARD 11)
set_property(TARGET kosh PROPERTY CXX_STANDARD_REQUIRED ON)

add_custom_target(data-target ALL DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/data" "${CMAKE_CURRENT_BINARY_DIR}/scripts")

include(GitVersioning)
enable_git_versioning(oyunum_common)
