cmake_minimum_required( VERSION 3.13 )

project( exporter
         VERSION 1.0.0
         DESCRIPTION "External run time libraries" )

# An OBJECT-library, used to only compile common sources once
# which are used in both math-libraries.
add_library( exporter SHARED )
target_sources( exporter
    PRIVATE   "exporter.h"
              "exporter.cpp"
    )

target_link_libraries( exporter PUBLIC )
