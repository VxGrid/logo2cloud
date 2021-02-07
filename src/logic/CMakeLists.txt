add_library( logic OBJECT )
target_sources( logic
    PRIVATE   "pointcloudgenerator.cpp"
    PUBLIC    "${CMAKE_CURRENT_SOURCE_DIR}/pointcloudgenerator.h"
    )


get_directory_property(BUILD_XYZ "${CMAKE_CURRENT_SOURCE_DIR}/../exporter" DEFINITION BUILD_XYZ_WRITER)
if (${BUILD_XYZ})
    add_definitions(-DBUILD_XYZ_WRITER)
endif()

get_directory_property(BUILD_LAS DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../exporter DEFINITION BUILD_LAS_WRITER)
if (${BUILD_LAS})
    add_definitions(-DBUILD_LAS_WRITER)
endif()


target_include_directories( logic
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../exporter" )


target_link_libraries( logic PUBLIC exporter )
add_library( logo2cloud::logic ALIAS logic )
