#cmake_minimum_required( VERSION 3.13 )

#project( exporter
#         VERSION 1.0.0
#         DESCRIPTION "External run time libraries" )

set( BUILD_XYZ_WRITER ON CACHE BOOL "Decide if you want to include XYZ export")
set( BUILD_LAS_WRITER ON CACHE BOOL "Decide if you want to include LAS/LAZ export")

add_library( exporter STATIC )

target_sources( exporter PRIVATE
    "exporter.h"
    "exporter.cpp"
    )

if(BUILD_XYZ_WRITER)
    message("Build and Link XYZ into exporter")
    target_sources( exporter PRIVATE
        "exporterXYZ.h"
        "exporterXYZ.cpp"
        )
endif ()

if(BUILD_LAS_WRITER)
    message("Build and Link LAS into exporter")
    set( PATH2LASZIP_APICFile "" CACHE FILEPATH "Filepath to laszip_api.c")
    set( PATH2LASZIP_IncludeDir "" CACHE PATH "Dir to include where laszip/laszip_api.h is in")
    set( PATH2LASZIP_APILIB "" CACHE FILEPATH "Filepath to lib laszip_api.lib or .so file")

    target_include_directories( exporter PUBLIC
        ${PATH2LASZIP_IncludeDir} #"/LASzip/install/include"
        )

    target_sources( exporter PRIVATE
        "exporterLAS.h"
        "exporterLAS.cpp"
        ${PATH2LASZIP_APICFile} #"/LASzip/dll/laszip_api.c" # interface
        )

      target_link_libraries( exporter PUBLIC
          ${PATH2LASZIP_APILIB} #"/LASzip/install/lib/liblaszip_api.so"
          ${CMAKE_DL_LIBS}
          )
endif ()


