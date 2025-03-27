cmake_minimum_required(VERSION 3.10)

# Remove when sharing with others.
list(APPEND CMAKE_PREFIX_PATH "/home/sysadmin/qtcreator-6.0.2")

project(Diagnostictools)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 17)

find_package(QtCreator REQUIRED COMPONENTS Core)
find_package(QT NAMES Qt5 REQUIRED COMPONENTS Widgets)
set(QtX Qt${QT_VERSION_MAJOR})
# Найдите библиотеку ProjectExplorer
find_library(PROJECT_EXPLORER_LIB NAMES ProjectExplorer PATHS ~/qtcreator-6.0.2/lib/qtcreator/plugins)
# Свяжите целевой объект с зависимостями
# Add a CMake option that enables building your plugin with tests.
# You don't want your released plugin binaries to contain tests,
# so make that default to 'NO'.
# Enable tests by passing -DWITH_TESTS=ON to CMake.
option(WITH_TESTS "Builds with tests" NO)

if(WITH_TESTS)
  # Look for QtTest
  find_package(${QtX} REQUIRED COMPONENTS Test)

  # Tell CMake functions like add_qtc_plugin about the QtTest component.
  set(IMPLICIT_DEPENDS Qt::Test)

  # Enable ctest for auto tests.
  enable_testing()
endif()
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/datavisualiser ${CMAKE_CURRENT_SOURCE_DIR}/dataqueryengine)
add_qtc_plugin(Diagnostictools
  PLUGIN_DEPENDS
    QtCreator::Core
  DEPENDS
    ${QtX}::Widgets
    QtCreator::ExtensionSystem
    QtCreator::Utils
  SOURCES
    .github/workflows/build_cmake.yml
    .github/workflows/README.md
    README.md
    diagnostictools.h
    diagnostictools_global.h
    diagnostictoolsconstants.h
    diagnostictoolstr.h
    dataqueryengine/dataqueryengine.h
    datavisualiser/datavisualiser.h
    datavisualiser/datavisualiser.ui
    datavisualiser/plotwidget.h
    dataqueryengine/dataqueryengine.cpp
    datavisualiser/datavisualiser.cpp
    datavisualiser/plotwidget.cpp

    diagnostictoolsplugin.cpp
    diagnostictoolsfactory.cpp
    diagnostictoolsfactory.h
)
target_link_libraries(Diagnostictools PRIVATE ${PROJECT_EXPLORER_LIB})
# Отключение всех qDebug, qInfo, qWarning; Disabling all qDebug, qInfo, qWarning
add_definitions(-DQT_NO_DEBUG_OUTPUT -DQT_NO_WARNING_OUTPUT -DQT_NO_INFO_OUTPUT)
# Отключение всех qCritical, qFatal; Disabling all qCritical, qFatal
#add_definitions(-DQT_NO_CRITICAL_OUTPUT)
#add_definitions(-DQT_NO_FATAL_OUTPUT)

