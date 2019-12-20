cmake_minimum_required(VERSION 3.5)

project(instruments LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Quick REQUIRED)

set(SOURCES
    main.cpp
    qml.qrc
    artificialhorizon.cpp
    autopilot.cpp
    autopilotmapilayer.cpp
    horizontalsituationindicator.cpp
    linearindicator.cpp
    MiniPID.cpp
    painterhelpers.cpp
    artificialhorizon.h
    autopilot.h
    autopilotmaplayer.h
    horizontalsituationindicator.h
    linearindicator.h
    )

if(ANDROID)
    add_library(instruments SHARED ${SOURCES})
else()
    add_executable(instruments ${SOURCES})
endif()

target_compile_definitions(instruments
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(instruments
  PRIVATE Qt5::Core Qt5::Quick)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# These variables must use CACHE, otherwise QtCreator won't see them.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android" CACHE INTERNAL "")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so CACHE INTERNAL "")
#    endif()
#endif()
