
set( CMAKE_PREFIX_PATH "~/Qt/6.1.0/gcc_64/lib/cmake/" )

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt6 COMPONENTS Widgets LinguistTools REQUIRED)

set( TS_FILES "${CMAKE_CURRENT_SOURCE_DIR}/logo2Cloud_de_GER.ts" )
qt6_create_translation( qmFiles ${CMAKE_CURRENT_SOURCE_DIR} ${TS_FILES} )
qt6_add_translation( qmFiles ${TS_FILES} )

add_custom_target( translations DEPENDS ${qmFiles} )
set(CMAKE_LINK_WHAT_YOU_USE TRUE)
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
            Qt6::Widgets
    )


#set_target_properties(logo2Cloud
#    PROPERTIES LINK_WHAT_YOU_USE TRUE
#    )
