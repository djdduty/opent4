file(GLOB t4_top_SOURCES src/*.*)
file(GLOB t4_loader_SOURCES src/loader/*.*)

include_directories(src/)

add_executable(opent4 ${t4_top_SOURCES} ${t4_loader_SOURCES})

source_group("OpenT4" FILES ${t4_top_SOURCES})
source_group("OpenT4\\Loader" FILES ${t4_loader_SOURCES})

target_link_libraries(opent4
    ${OSG_LIBRARIES}
    ${OSGUTIL_LIBRARIES}
    ${OSGDB_LIBRARIES}
    ${OSGVIEWER_LIBRARIES}
    ${OSGGA_LIBRARIES}
)
