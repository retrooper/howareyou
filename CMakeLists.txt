cmake_minimum_required(VERSION 3.20)
project(howareyou)

set(CMAKE_CXX_STANDARD 23)

# FetchContent
include(FetchContent)


set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
set(FETCHCONTENT_QUIET OFF)

FetchContent_Declare(ftxui
        GIT_REPOSITORY https://github.com/arthursonzogni/ftxui.git
        GIT_TAG        v5.0.0
        GIT_PROGRESS   TRUE
        GIT_SHALLOW    TRUE
        EXCLUDE_FROM_ALL
        )
FetchContent_MakeAvailable(ftxui)


# Application
add_executable(howareyou main.cpp)

target_link_libraries(howareyou
        PRIVATE ftxui::screen
        PRIVATE ftxui::dom
        PRIVATE ftxui::component
        )
