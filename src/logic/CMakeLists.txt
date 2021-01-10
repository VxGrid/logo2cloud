cmake_minimum_required( VERSION 3.13 )

project( BusinessLogic
         VERSION 1.0.0
         DESCRIPTION "Internal logic-library." )

# An OBJECT-library, used to only compile common sources once
# which are used in both math-libraries.
add_library( logic OBJECT )
target_sources( logic
    PRIVATE   "pointcloudgenerator.cpp"
    PUBLIC    "${CMAKE_CURRENT_SOURCE_DIR}/pointcloudgenerator.h"
    )

# Required include search-paths and constexpr support.
target_include_directories( logic
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../exporter" )


target_link_libraries( logic PUBLIC exporter )
add_library( logo2cloud::logic ALIAS logic )
