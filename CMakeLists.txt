cmake_minimum_required(VERSION 3.10)

project(audio-spectrum-visualizer VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets Multimedia REQUIRED)

set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_RELEASE} -fprofile-arcs -ftest-coverage")

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui
    src/dialogsettings.cpp
    src/dialogsettings.h
    src/dialogsettings.ui
    src/widgetamplitude.cpp
    src/widgetamplitude.h
    src/widgetfrequencyviewer.cpp
    src/widgetfrequencyviewer.h

    src/audioinputhandler.cpp
    src/audioinputhandler.h
    src/ringbuffer.hpp

    src/resource_file.qrc)

target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Multimedia)
