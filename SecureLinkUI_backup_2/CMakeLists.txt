cmake_minimum_required(VERSION 3.16)

project(SecureLinkUI VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ✅ Multimedia + Widgets
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets Sql LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets Sql LinguistTools)

set(TS_FILES SecureLinkUI_en_CA.ts)

# 🔗 Include the client (C++ backend) files
set(CLIENT_DIR ${CMAKE_SOURCE_DIR}/../client)
include_directories(${CLIENT_DIR})
include_directories(${CMAKE_SOURCE_DIR}/shared)


# 🎯 Project sources including backend
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    loginwindow.cpp
    loginwindow.h
    login_backend.cpp
    login_backend.h
    ${CLIENT_DIR}/client_utils.cpp
    ${CLIENT_DIR}/client_utils.h
    ${CMAKE_SOURCE_DIR}/../shared/data_packet.cpp
    ${TS_FILES}
)

# ✅ Only call this AFTER defining the executable
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(SecureLinkUI
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        dashboardwindow.h dashboardwindow.cpp
        motionalertswidget.h motionalertswidget.cpp
        smartlightwindow.h smartlightwindow.cpp
        logtransferwindow.h logtransferwindow.cpp
        accesslogswidget.h accesslogswidget.cpp
        camerapage.h camerapage.cpp
        garagedoorwindow.h garagedoorwindow.cpp
        smartlockwindow.h smartlockwindow.cpp
        toggleswitch.cpp
        toggleswitch.h
        resources.qrc
    )

    # ✅ Link Multimedia + Widgets + MultimediaWidgets!
    target_link_libraries(SecureLinkUI PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Multimedia Qt${QT_VERSION_MAJOR}::MultimediaWidgets Qt${QT_VERSION_MAJOR}::Sql)

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(SecureLinkUI SHARED ${PROJECT_SOURCES})
    else()
        add_executable(SecureLinkUI ${PROJECT_SOURCES})
    endif()
    target_link_libraries(SecureLinkUI PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Multimedia Qt${QT_VERSION_MAJOR}::MultimediaWidgets Qt${QT_VERSION_MAJOR}::Sql)
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.SecureLinkUI)
endif()

set_target_properties(SecureLinkUI PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS SecureLinkUI
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(SecureLinkUI)
endif()
