cmake_minimum_required(VERSION 2.6)

project(StockPerf)

find_package(KDE4 REQUIRED)
include(KDE4Defaults)

find_library(HTMLTIDY_LIBS tidy)

if(NOT HTMLTIDY_LIBS)
  message(FATAL_ERROR "libtidy is required, please install htmltidy")
endif(NOT HTMLTIDY_LIBS)

add_definitions(${QT_DEFINITIONS} 
                ${KDE4_DEFINITIONS})

include_directories(${CMAKE_SOURCE_DIR} 
                    ${CMAKE_BINARY_DIR} 
                    ${KDE4_INCLUDES})

add_subdirectory(dataengine)
add_subdirectory(applet)
