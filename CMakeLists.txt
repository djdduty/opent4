cmake_minimum_required(VERSION 2.6)
project(OpenT4)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
        "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel."
        FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS None Debug Release RelWithDebInfo MinSizeRel)
ENDIF()


message(STATUS "Configuring OpenT4...")

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake-extras/")

set(OPENMW_VERSION_MAJOR 0)
set(OPENMW_VERSION_MINOR 1)
set(OPENMW_VERSION_RELEASE 0)

set(OPENMW_VERSION "${OPENMW_VERSION_MAJOR}.${OPENMW_VERSION_MINOR}.${OPENMW_VERSION_RELEASE}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

# Options
option(OSG_STATIC "Link static build of OpenSceneGraph into the binaries" FALSE)

find_package(OpenSceneGraph 3.4.0 REQUIRED osgDB osgViewer osgText osgGA osgAnimation osgParticle osgUtil osgFX)
include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS})

set(USED_OSG_PLUGINS
                    osgdb_bmp
                    osgdb_dds
                    osgdb_jpeg
                    osgdb_osg
                    osgdb_png
                    osgdb_serializers_osg
                    osgdb_tga
                  )

get_filename_component(OSG_LIB_DIR ${OSGDB_LIBRARY} DIRECTORY)
set(OSGPlugins_LIB_DIR "${OSG_LIB_DIR}/osgPlugins-${OPENSCENEGRAPH_VERSION}")

# Definitions
ADD_DEFINITIONS(-std=c++11)
if(OSG_STATIC)
    add_definitions(-DOSG_LIBRARY_STATIC)

    find_package(OSGPlugins REQUIRED COMPONENTS ${USED_OSG_PLUGINS})
    list(APPEND OPENSCENEGRAPH_LIBRARIES ${OSGPlugins_LIBRARIES})
endif()

add_subdirectory(${PROJECT_SOURCE_DIR}/opent4)

install(DIRECTORY "${PROJECT_SOURCE_DIR}/opent4/src/" DESTINATION include/opent4 FILES_MATCHING PATTERN "*.h")
