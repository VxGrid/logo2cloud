project(logic
    DESCRIPTION "Logic to convert the image into a point cloud."
)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME} PRIVATE
    "pointcloudgenerator.cpp"
    "pointcloudgenerator.h"
)


# get_directory_property(BUILD_XYZ "${CMAKE_CURRENT_SOURCE_DIR}/../exporter" DEFINITION BUILD_XYZ_WRITER)
# if (${BUILD_XYZ})
#     add_definitions(-DBUILD_XYZ_WRITER)
# endif()

# get_directory_property(BUILD_LAS DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../exporter DEFINITION BUILD_LAS_WRITER)
# if (${BUILD_LAS})
#     add_definitions(-DBUILD_LAS_WRITER)
# endif()


target_include_directories(${PROJECT_NAME}
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../exporter"
)


target_link_libraries(${PROJECT_NAME} PUBLIC exporter)
