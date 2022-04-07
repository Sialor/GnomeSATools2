cmake_minimum_required(VERSION 3.1)

project(GnomeSATools)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(CMAKE_PREFIX_PATH "C:/Qt/5.15.2/msvc2019_64/lib/cmake")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SOURCES 
    ${PROJECT_SOURCE_DIR}/src/confirmclose.cpp
    ${PROJECT_SOURCE_DIR}/src/dffarchive.cpp
    ${PROJECT_SOURCE_DIR}/src/imgarchiver.cpp
    ${PROJECT_SOURCE_DIR}/src/imgcanvas.cpp
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/mainwindow.cpp
    ${PROJECT_SOURCE_DIR}/src/menu.cpp
    ${PROJECT_SOURCE_DIR}/src/txdarchive.cpp
    ${PROJECT_SOURCE_DIR}/src/mainwindow.ui
    ${PROJECT_SOURCE_DIR}/src/confirmclose.ui
    ${PROJECT_SOURCE_DIR}/res/icons/icons.qrc)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

add_executable(GnomeSATools ${SOURCES})

target_include_directories(GnomeSATools PUBLIC include/)

target_link_libraries(GnomeSATools Qt5::Core Qt5::Gui Qt5::Widgets)