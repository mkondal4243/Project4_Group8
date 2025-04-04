cmake_minimum_required(VERSION 3.16)

project(securelink_ui VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(TS_FILES securelink_ui_en_CA.ts)

set(PROJECT_SOURCES
        main.cpp
        securelinkwindow.cpp
        securelinkwindow.h
        securelinkwindow.ui
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(securelink_ui
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        signupwindow.h
        signupwindow.cpp
        forgotpasswordwindow.h forgotpasswordwindow.cpp
        dashboardwindow.h dashboardwindow.cpp
        logtransferwindow.h logtransferwindow.cpp
        accesslogswindow.h accesslogswindow.cpp
        motionalertswindow.h motionalertswindow.cpp
        motionalertswindow.h motionalertswindow.cpp
        smartlightwindow.h smartlightwindow.cpp
        smartgaragewindow.h smartgaragewindow.cpp
        camerapreviewwindow.h camerapreviewwindow.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET securelink_ui APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(securelink_ui SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(securelink_ui
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(securelink_ui PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.securelink_ui)
endif()
set_target_properties(securelink_ui PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS securelink_ui
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(securelink_ui)
endif()


find_package(Qt6 REQUIRED COMPONENTS Charts)
target_link_libraries(securelink_ui PRIVATE Qt6::Charts)



