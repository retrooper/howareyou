cmake_minimum_required(VERSION 3.20)
set(CMAKE_VERBOSE_MAKEFILE ON)
project(howareyou)

set(CMAKE_CXX_STANDARD 23)


# Prefer static libs over shared lib
LIST(REVERSE CMAKE_FIND_LIBRARY_SUFFIXES)

#========== Add CPM package manager =================
set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM.cmake")
set(CPM_VERSION 0.18)

if(NOT EXISTS ${CPM_DOWNLOAD_LOCATION})
    message(STATUS "Downloading CPM.cmake")
    file(DOWNLOAD https://github.com/TheLartians/CPM.cmake/releases/download/v${CPM_VERSION}/CPM.cmake ${CPM_DOWNLOAD_LOCATION})
endif(NOT EXISTS ${CPM_DOWNLOAD_LOCATION})
include(${CPM_DOWNLOAD_LOCATION})

##========== Add dependency =================
OPTION(ENABLE_OPENSSL_FROM_SOURCE "BUILD OPENSSL FROM SOURCE AT CONFIGURE TIME" ON)

if(ENABLE_OPENSSL_FROM_SOURCE)
    set(BUILD_OPENSSL_WORKING_DIR ${CMAKE_BINARY_DIR}/_external/build-openssl)
    set(BUILD_OPENSSL_SRC_DIR ${CMAKE_SOURCE_DIR}/build-openssl)
    set(BUILD_OPENSSL_INSTALL_DIR "${BUILD_OPENSSL_WORKING_DIR}/install/OpenSSL")
    file(MAKE_DIRECTORY ${BUILD_OPENSSL_WORKING_DIR})
    if(NOT EXISTS ${BUILD_OPENSSL_INSTALL_DIR})
        message(STATUS "Building OpenSSL.. at ${BUILD_OPENSSL_WORKING_DIR}, Install DIR ${BUILD_OPENSSL_INSTALL_DIR}")
        execute_process(
                COMMAND ${CMAKE_COMMAND} ${BUILD_OPENSSL_SRC_DIR} -DINSTALL_DIR=${BUILD_OPENSSL_INSTALL_DIR}
                WORKING_DIRECTORY ${BUILD_OPENSSL_WORKING_DIR}
        )
        execute_process(
                COMMAND ${CMAKE_COMMAND} --build .
                WORKING_DIRECTORY ${BUILD_OPENSSL_WORKING_DIR}
        )
    else()
        message(STATUS "${BUILD_OPENSSL_INSTALL_DIR} already exists, skips rebuilding..")
    endif()

    set(OPENSSL_ROOT_DIR "${BUILD_OPENSSL_INSTALL_DIR}" CACHE INTERNAL "OPENSSL ROOT DIR")
    message(MESSAGE "Setting OPENSSL ROOT DIR to newly built OpenSSL ${OPENSSL_ROOT_DIR}")
else()
endif()

if (MSVC)
    set(CURL_PLATFORM_OPTIONS "CMAKE_USE_WINSSL ON;CMAKE_USE_OPENSSL OFF")
elseif(APPLE)
    set(CURL_PLATFORM_OPTIONS "CMAKE_USE_SECTRANSP OFF;CMAKE_USE_OPENSSL ON")
else()
    set(CURL_PLATFORM_OPTIONS "CMAKE_USE_OPENSSL ON")
endif()

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

#set( CURL_LIBRARY ${CMAKE_SOURCE_DIR}/lib )
#set( CURL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include )

# Application

#set(CURL_LIBRARY  ${CMAKE_SOURCE_DIR}/cmake-build-debug/_deps/curl-build/lib)
#set(CURL_INCLUDE_DIR " ${CMAKE_SOURCE_DIR}/cmake-build-debug/_deps/curl-src/include")

#find_package(CURL REQUIRED)
#add_library(CURL SHARED)

include_directories(openai)

include_directories(include)

file(GLOB all_SRCS "${PROJECT_SOURCE_DIR}/src/*.cpp")

CPMAddPackage(
        NAME curl
        VERSION 7.67.0
        URL https://github.com/curl/curl/releases/download/curl-7_67_0/curl-7.67.0.tar.gz
        OPTIONS
        "CURL_STATICLIB ON"
        "BUILD_CURL_EXE OFF"
        "BUILD_SHARED_LIBS OFF"
        "BUILD_TESTING OFF"
        "WITH_STATIC_RT ON"
        "CMAKE_USE_LIBSSH2 OFF"
        ${CURL_PLATFORM_OPTIONS})

add_executable(howareyou ${all_SRCS})

target_link_libraries(howareyou
        PRIVATE ftxui::screen
        PRIVATE ftxui::dom
        PRIVATE ftxui::component
        #PRIVATE CURL::libcurl
        libcurl)


#find_package(curl REQUIRED) # probably with CONFIG REQUIRED
