cmake_minimum_required(VERSION 2.8)

project(macintegration-plugin)
set(PLUGIN_NAME macintegration)

set(SOURCES macintegration.cpp macnotify.mm machelper.mm)
set(HEADERS macintegration.h macnotify.h machelper.h)
set(MOC_HEADERS macintegration.h macnotify.h machelper.h)

include_directories("${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}")

list(APPEND ADD_LIBS "-framework Cocoa")

include("${CMAKE_SOURCE_DIR}/cmake/config.cmake")
include("${CMAKE_SOURCE_DIR}/translations/languages.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/plugin.cmake")

