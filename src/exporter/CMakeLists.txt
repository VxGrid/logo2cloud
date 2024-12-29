#cmake_minimum_required( VERSION 3.13 )

#project( exporter
#         VERSION 1.0.0
#         DESCRIPTION "External run time libraries" )
project(exporter)
set(BUILD_XYZ_WRITER ON CACHE BOOL "Decide if you want to include XYZ export")
set(BUILD_LAS_WRITER ON CACHE BOOL "Decide if you want to include LAS/LAZ export")
set(BUILD_E57_WRITER ON CACHE BOOL "Decide if you want to include E57 export")

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
    if(CMAKE_SYSTEM_NAME MATCHES "Emscripten")
        message("Build and Link LAS into WASM exporter")
        set(PATH2LASZIP "/home/martin/Dokumente/libs/laszip/einstall" CACHE FILEPATH "Filepath to laszip dir ~/LASzip")
        #set(PATH2LASZIP_APICFile "${PATH2LASZIP}/dll/laszip_api.c") # Filepath to laszip_api.c"
        set(PATH2LASZIP_IncludeDir "${PATH2LASZIP}/include") # Dir to include where laszip/laszip_api.h is in
        set(PATH2LASZIP_APILIB "${PATH2LASZIP}/lib/liblaszip.a;${PATH2LASZIP}/lib/liblaszip_api.a") # Filepath to lib laszip_api.lib or .so file
    else()    
        message("Build and Link LAS into exporter")
        set(PATH2LASZIP "/home/martin/Dokumente/libs/laszip/install" CACHE FILEPATH "Filepath to laszip dir ~/LASzip")
        #set(PATH2LASZIP_APICFile "${PATH2LASZIP}/dll/laszip_api.c") # Filepath to laszip_api.c"
        set(PATH2LASZIP_IncludeDir "${PATH2LASZIP}/include") # Dir to include where laszip/laszip_api.h is in
        set(PATH2LASZIP_APILIB "${PATH2LASZIP}/lib/liblaszip.a;${PATH2LASZIP}/lib/liblaszip_api.a") # Filepath to lib laszip_api.lib or .so file
    endif()

    target_compile_definitions(${PROJECT_NAME} PUBLIC BUILD_LAS_WRITER)

    target_include_directories(${PROJECT_NAME} PUBLIC
        ${PATH2LASZIP_IncludeDir} #"/LASzip/install/include"
        ${PATH2LASZIP_IncludeDir}/laszip
    )

    target_sources(${PROJECT_NAME} PRIVATE
        "exporterLAS.h"
        "exporterLAS.cpp"
        #${PATH2LASZIP_APICFile} #"/LASzip/dll/laszip_api.c" # interface
    )

    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${PATH2LASZIP_APILIB} #"/LASzip/install/lib/liblaszip_api.so"
        ${CMAKE_DL_LIBS}
    )
endif()


if(BUILD_E57_WRITER)
    target_compile_definitions(${PROJECT_NAME} PUBLIC BUILD_E57_WRITER)

    find_package(XercesC REQUIRED)
    message("Xerces found: ${XercesC_FOUND} ${XercesC_INCLUDE_DIRS}")
    #set(XercesC_VERSION "3.3.0")
    #set(XercesC_INCLUDE_DIRS "/home/martin/Dokumente/libs/xerces/wasminstall/include")
    #set(XercesC_FOUND TRUE)
    #set(XercesC_LIBRARIES "/home/martin/Dokumente/libs/xerces/wasminstall/lib/libxerces-c.a")

    set(E57INCLUDEDIR "/home/martin/Dokumente/libs/e57/install/include" CACHE FILEPATH "Path to E57 include dir")
    set(E57LIBS "/home/martin/Dokumente/libs/e57/install/lib/libE57Format.a" CACHE FILEPATH "Path to E57 libs")

    target_include_directories(${PROJECT_NAME} PUBLIC
        ${E57INCLUDEDIR}
    )

    target_sources(${PROJECT_NAME} PRIVATE
        "exporterE57.h"
        "exporterE57.cpp"
    )

    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${E57LIBS}
        XercesC::XercesC
    )

endif()


