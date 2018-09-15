cmake_minimum_required(VERSION 3.1.0)

# set project name
project(ticktock)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)

#set include directories
include_directories(
    ../../src
)
# Populate a CMake variable with the sources
set(ticktock_SRCS
    main.cpp
)

add_library(thistam STATIC ../../src/sm2.cpp)
target_link_libraries(thistam Qt5::Core )
add_library(thistam_baha STATIC ../../src/baha_qt.cpp)
target_link_libraries(thistam_baha Qt5::Core )

# Tell CMake to create the executable
add_executable(${PROJECT_NAME} ${ticktock_SRCS})

# Use the Widgets module from Qt 5
target_link_libraries(${PROJECT_NAME} Qt5::Core thistam thistam_baha)
