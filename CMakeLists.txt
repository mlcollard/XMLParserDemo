cmake_minimum_required(VERSION 2.20)

# project info
project(XMLParserDemo)

# find libxml2
find_package(LibXml2 REQUIRED)
include_directories(${LIBXML2_INCLUDE_DIR})
add_link_options(${LIBXML2_LIBRARIES})

# force release since used for timing
set(CMAKE_BUILD_TYPE Release)

# C++17 standard
set(CMAKE_CXX_STANDARD 17)

# Turn on all warnings, and make warnings errors
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

# overall driver
add_executable(driver Driver.cpp)

# SAX2 driver
add_executable(sax2driver SAX2Driver.cpp)

# TextReader driver
add_executable(textreaderdriver TextReaderDriver.cpp)

# YAXP driver
add_executable(yaxpdriver YAXPDriver.cpp)

# DOM driver
add_executable(domdriver DOMDriver.cpp)

# XPath driver
add_executable(xpathdriver XPathDriver.cpp)
