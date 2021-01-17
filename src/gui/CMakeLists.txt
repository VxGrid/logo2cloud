cmake_minimum_required( VERSION 3.13 )

project( gui
         VERSION 1.0.0
         DESCRIPTION "My fancy gui." )


set( CMAKE_PREFIX_PATH "~/Qt/5.15.2/gcc_64/lib/cmake/Qt5/" )

#set(CMAKE_INCLUDE_CURRENT_DIR ON)
#set(CMAKE_PREFIX_PATH "~/Qt/Tools/CMake/bin/")
#set(CMAKE_PREFIX_PATH "/home/martin/Qt/5.15.2/gcc_64/"
#    "~/Qt/Tools/CMake/bin/")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt5 COMPONENTS Widgets LinguistTools REQUIRED)

add_executable( gui2 )


target_sources( gui2
    PRIVATE "main.cpp"
            "mainwindow.cpp"
            "mainwindow.h"
            "mainwindow.ui"
            "editor.h"
            "editor.cpp"
            "editor.ui"
            ${TS_FILES}
    )

target_link_libraries( gui2
    PRIVATE logo2cloud::logic
            Qt5::Widgets
    )

#  add_executable(logo2Cloud
#    main.cpp
#    mainwindow.cpp
#    mainwindow.h
#    mainwindow.ui
#    editor.h
#    editor.cpp
#    editor.ui
#    ${TS_FILES}
#  )

#target_link_libraries(logo2Cloud PRIVATE Qt5::Widgets)

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
