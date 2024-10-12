#cmake_minimum_required( VERSION 3.13 )

#project( exporter
#         VERSION 1.0.0
#         DESCRIPTION "External run time libraries" )
project(exporter)
set(BUILD_XYZ_WRITER ON CACHE BOOL "Decide if you want to include XYZ export")
set(BUILD_LAS_WRITER OFF CACHE BOOL "Decide if you want to include LAS/LAZ export")

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME} PRIVATE
    "exporter.h"
    "exporter.cpp"
)

if(BUILD_XYZ_WRITER)
    message("Build and Link XYZ into exporter")
    target_sources(${PROJECT_NAME} PRIVATE
        "exporterXYZ.h"
        "exporterXYZ.cpp"
        )
    target_compile_definitions(${PROJECT_NAME} PUBLIC BUILD_XYZ_WRITER)
endif ()

if(BUILD_LAS_WRITER)
    target_compile_definitions(${PROJECT_NAME} PUBLIC BUILD_LAS_WRITER)
    message("Build and Link LAS into exporter")
    set(PATH2LASZIP_APICFile "/home/martin/Dokumente/cpp/LASzip/dll/laszip_api.c" CACHE FILEPATH "Filepath to laszip_api.c")
    set(PATH2LASZIP_IncludeDir "/home/martin/Dokumente/cpp/LASzip/include/" CACHE PATH "Dir to include where laszip/laszip_api.h is in")
    set(PATH2LASZIP_APILIB "/home/martin/Dokumente/cpp/LASzip/build/lib/liblaszip.so" CACHE FILEPATH "Filepath to lib laszip_api.lib or .so file")

    target_include_directories(${PROJECT_NAME} PUBLIC
        ${PATH2LASZIP_IncludeDir} #"/LASzip/install/include"
    )

    target_sources(${PROJECT_NAME} PRIVATE
        "exporterLAS.h"
        "exporterLAS.cpp"
        ${PATH2LASZIP_APICFile} #"/LASzip/dll/laszip_api.c" # interface
    )

    target_link_libraries(${PROJECT_NAME} PUBLIC
        ${PATH2LASZIP_APILIB} #"/LASzip/install/lib/liblaszip_api.so"
        ${CMAKE_DL_LIBS}
    )
endif ()


