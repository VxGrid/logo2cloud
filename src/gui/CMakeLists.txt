project(logo2Cloud VERSION 1.0.2)
set(CMAKE_PREFIX_PATH "~/Qt/6.6.3/gcc_64/lib/cmake/" )

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools  REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools REQUIRED)

set(TS_FILES "${CMAKE_CURRENT_SOURCE_DIR}/logo2Cloud_de_GER.ts")
qt6_create_translation(qmFiles ${CMAKE_CURRENT_SOURCE_DIR} ${TS_FILES})
qt6_add_translation(qmFiles ${TS_FILES})

add_custom_target(translations DEPENDS ${qmFiles})
set(CMAKE_LINK_WHAT_YOU_USE TRUE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
#set(CMAKE_EXECUTABLE_SUFFIX ".html")

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    editor.h
    editor.cpp
    editor.ui
)

#add_executable( logo2Cloud )
#qt_add_executable(${PROJECT_NAME} MANUAL_FINALIZATION ${PROJECT_SOURCES})
qt_add_executable(${PROJECT_NAME} MANUAL_FINALIZATION)

target_sources(${PROJECT_NAME}
    PRIVATE "main.cpp"
            "mainwindow.cpp"
            "mainwindow.h"
            "mainwindow.ui"
            "editor.h"
            "editor.cpp"
            "editor.ui"
            ${translations}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    exporter
    logic
    Qt6::Widgets
)

add_dependencies(${PROJECT_NAME} exporter logic translations)

#set_target_properties(logo2Cloud
#    PROPERTIES LINK_WHAT_YOU_USE TRUE
#    )

set_target_properties(${PROJECT_NAME} PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(CMAKE_SYSTEM_NAME MATCHES "Emscripten")
    message("Add target link options for WASM")
    target_link_options(${PROJECT_NAME} PUBLIC -sASYNCIFY -Os)
endif()

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()


