file(GLOB t4_top_SOURCES src/*.*)
file(GLOB t4_loader_SOURCES src/loader/*.*)
file(GLOB t4_imgui_SOURCES src/imgui/*.*)
file(GLOB t4_scene_SOURCES src/scene/*.*)
file(GLOB t4_util_SOURCES src/util/*.*)

include_directories(src/)

add_executable(opent4 ${t4_top_SOURCES} ${t4_loader_SOURCES} ${t4_imgui_SOURCES} ${t4_scene_SOURCES} ${t4_util_SOURCES})

source_group("OpenT4" FILES ${t4_top_SOURCES})
source_group("OpenT4\\imGui" FILES ${t4_limgui_SOURCES})
source_group("OpenT4\\Loader" FILES ${t4_loader_SOURCES})
source_group("OpenT4\\Scene" FILES ${t4_scene_SOURCES})
source_group("OpenT4||Util" FILES ${t4_util_SOURCES})

target_link_libraries(opent4
    ${OSG_LIBRARIES}
    ${OSGUTIL_LIBRARIES}
    ${OSGDB_LIBRARIES}
    ${OSGVIEWER_LIBRARIES}
    ${OSGGA_LIBRARIES}
)
