cmake_minimum_required(VERSION 3.10)

project(OptiFib VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Werror -g)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)

set(SOURCES
    src/main.c
    src/menu.c
    src/persistence.c
    src/dijkstra.c
    lib/common.c
    lib/graph.c
    lib/priority_queue.c
    lib/utils.c
)

add_executable(optifib ${SOURCES})

if(UNIX AND NOT APPLE)
    target_link_libraries(optifib m)
endif()

set_target_properties(optifib PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
