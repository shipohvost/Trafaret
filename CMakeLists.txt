cmake_minimum_required(VERSION 3.30)

project(
    Trafaret
    VERSION 0.1
    LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(RELEASE_DIR build/release)
set(DEBUG_DIR build/debug)
set(SRC_DIR src)
set(INC_DIR inc)

set(HEADER_NAMES mainwindow.h hexlineedit.h)
set(SOURCE_NAMES main.cpp mainwindow.cpp mainwindow.ui hexlineedit.cpp)

set(HEADERS "")
foreach(HEADER ${HEADER_NAMES})
    list(APPEND HEADERS "${INC_DIR}/${HEADER}")
endforeach()

set(SOURCES "")
foreach(SOURCE ${SOURCE_NAMES})
    list(APPEND SOURCES "${SRC_DIR}/${SOURCE}")
endforeach()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Trafaret MANUAL_FINALIZATION ${SOURCES} ${HEADERS})
else()
    add_executable(Trafaret ${SOURCES} ${HEADERS})
endif()

target_link_libraries(Trafaret PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_include_directories(${PROJECT_NAME} PRIVATE ${INC_DIR})

include(GNUInstallDirs)
install(
    TARGETS Trafaret
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Trafaret)
endif()
