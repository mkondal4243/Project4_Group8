cmake_minimum_required(VERSION 3.16)
project(SecureLinkUI VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ✅ Qt5 components only
find_package(Qt5 REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets Sql LinguistTools)

set(TS_FILES SecureLinkUI_en_CA.ts)

# 🔗 Correct include paths
set(CLIENT_DIR ${CMAKE_SOURCE_DIR}/client)
set(SHARED_DIR ${CMAKE_SOURCE_DIR}/shared)

include_directories(${CLIENT_DIR})
include_directories(${SHARED_DIR})

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp mainwindow.h mainwindow.ui
    loginwindow.cpp loginwindow.h
    login_backend.cpp login_backend.h
    ${CLIENT_DIR}/client_utils.cpp
    ${CLIENT_DIR}/client_utils.h
    ${SHARED_DIR}/data_packet.cpp
    ${SHARED_DIR}/data_packet.h
    ${SHARED_DIR}/base64.cpp
    ${SHARED_DIR}/base64.h
    dashboardwindow.h dashboardwindow.cpp
    motionalertswidget.h motionalertswidget.cpp
    smartlightwindow.h smartlightwindow.cpp
    logtransferwindow.h logtransferwindow.cpp
    accesslogswidget.h accesslogswidget.cpp
    camerapage.h camerapage.cpp
    garagedoorwindow.h garagedoorwindow.cpp
    smartlockwindow.h smartlockwindow.cpp
    toggleswitch.cpp toggleswitch.h
    resources.qrc
    ${TS_FILES}
)

add_executable(SecureLinkUI ${PROJECT_SOURCES})

target_link_libraries(SecureLinkUI
    Qt5::Widgets
    Qt5::Multimedia
    Qt5::MultimediaWidgets
    Qt5::Sql
    shared
)

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})

# 🎯 macOS bundle settings
set_target_properties(SecureLinkUI PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.SecureLinkUI
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
