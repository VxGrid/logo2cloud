cmake_minimum_required( VERSION 3.13 )

project( gui
         VERSION 1.0.0
         DESCRIPTION "My fancy gui." )


set( CMAKE_PREFIX_PATH "~/Qt/5.15.2/gcc_64/lib/cmake/Qt5/" )

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt5 COMPONENTS Widgets LinguistTools REQUIRED)

set( TS_FILES "${CMAKE_CURRENT_SOURCE_DIR}/logo2Cloud_de_GER.ts" )
qt5_create_translation( qmFiles ${CMAKE_CURRENT_SOURCE_DIR} ${TS_FILES} )
qt5_add_translation( qmFiles ${TS_FILES} )

add_custom_target( translations DEPENDS ${qmFiles} )

add_executable( logo2Cloud )

add_dependencies( logo2Cloud translations )

target_sources( logo2Cloud
    PRIVATE "main.cpp"
            "mainwindow.cpp"
            "mainwindow.h"
            "mainwindow.ui"
            "editor.h"
            "editor.cpp"
            "editor.ui"
            ${translations}
    )

target_link_libraries( logo2Cloud
    PRIVATE logo2cloud::logic
            Qt5::Widgets
    )
