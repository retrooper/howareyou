cmake_minimum_required(VERSION 3.20)
project(howareyou)

set(CMAKE_CXX_STANDARD 23)

# FetchContent
include(FetchContent)


set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
set(FETCHCONTENT_QUIET OFF)

FetchContent_Declare(ftxui
        GIT_REPOSITORY https://github.com/arthursonzogni/ftxui.git
        GIT_TAG v5.0.0
        GIT_PROGRESS TRUE
        GIT_SHALLOW TRUE
        EXCLUDE_FROM_ALL
        )
FetchContent_MakeAvailable(ftxui)

FetchContent_Declare(curl

        GIT_REPOSITORY "https://github.com/curl/curl.git"
        GIT_TAG "curl-8_6_0"
        GIT_SHALLOW TRUE)

FetchContent_MakeAvailable(curl)

#set( CURL_LIBRARY ${CMAKE_SOURCE_DIR}/lib )
#set( CURL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include )

# Application



include_directories(curl-mingw/include openai)

include_directories(include)

file(GLOB all_SRCS "${PROJECT_SOURCE_DIR}/src/*.cpp")
add_executable(howareyou ${all_SRCS})

target_link_libraries(howareyou
        PRIVATE ftxui::screen
        PRIVATE ftxui::dom
        PRIVATE ftxui::component
        PRIVATE CURL::libcurl_static
        )
