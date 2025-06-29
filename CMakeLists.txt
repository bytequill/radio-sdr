cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(radio-sdr)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.hpp")

add_executable(main ${SOURCES} ${HEADERS})

set(CMAKE_CXX_FLAGS_DEBUG "-g -Og -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#==============================
# CPM - Cmake Package Manager |
#==============================
include(src/get_CPM.cmake)

CPMAddPackage("gh:libsdl-org/SDL#release-3.2.10")

CPMAddPackage(gh:ocornut/imgui@1.91.9b)
add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp # optionally comment this out
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
)
message(STATUS "imgui_SOURCE_DIR: ${imgui_SOURCE_DIR}")
target_include_directories(imgui INTERFACE ${imgui_SOURCE_DIR})
target_compile_definitions(imgui PUBLIC -DIMGUI_DISABLE_OBSOLETE_FUNCTIONS) # optional imgui setting

target_link_libraries(main SDL3 imgui)
