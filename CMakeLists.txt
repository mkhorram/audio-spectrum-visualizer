cmake_minimum_required(VERSION 3.10)

project(audio-spectrum-visualizer VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)


add_executable(${PROJECT_NAME}
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui
    src/dlgsettings.cpp
    src/dlgsettings.h
    src/dlgsettings.ui
    src/wgtamplitude.cpp
    src/wgtamplitude.h
    src/wgtfrequencyviewer.cpp
    src/wgtfrequencyviewer.h
    )

target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
