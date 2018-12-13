cmake_minimum_required(VERSION 2.8)

include(FindPkgConfig)

# Project name
project(get_shapes)

# Source directory
set(SOURCE_DIR "src")

# Source file(s)
set(SOURCES "${SOURCE_DIR}/get_shapes.cc")


### Libraries

## Standard libraries, here only cfitsio
pkg_check_modules(PKGS REQUIRED cfitsio)
message(STATUS "cfitsio: ${PKGS_LIBRARY_DIRS}")
message(STATUS "cfistio: ${PKGS_LIBRARIES}")


## Non-standard libraries: Look for library and include files

# Shapelens
find_library(SHAPELENS_LIB shapelens)
if(NOT SHAPELENS_LIB)
	message(FATAL_ERROR "shapelens library not found")
else()
	message(STATUS "Found shapelens library ${SHAPELENS_LIB}")
endif()

find_path(SHAPELENS_INC_DIR shapelens/ShapeLens.h)
if(NOT SHAPELENS_INC_DIR)
	message(FATAL_ERROR "shapelens include files not found")
else()
	message(STATUS "Found shapelens include files in ${SHAPELENS_INC_DIR}")
endif()


# TMV, used by shapelens
find_library(TMV_LIB tmv)
if(NOT TMV_LIB)
        message(FATAL_ERROR "tmv library not found")
else()
        message(STATUS "Found tmv library ${TMV_LIB}")
endif()

find_path(TMV_INC_DIR TMV.h)
if(NOT TMV_INC_DIR)
        message(FATAL_ERROR "tmv include files not found")
else()
        message(STATUS "Found tmv include files in ${TMV_INC_DIR}")
endif()


#set(LIBS ${LIBS} ${CFITSIO_LIBRARIES})
link_directories(${PKGS_LIBRARY_DIRS})
include_directories(${PKGS_INCLUDE_DIRS} ${SHAPELENS_INC_DIR} ${TMV_INC_DIR})

set(CMAKE_C_FLAGS "-ansi -fPIC")

# Executable
add_executable(get_shapes ${SOURCES})
target_link_libraries(get_shapes ${PKGS_LIBRARIES} ${SHAPELENS_LIB} ${TMV_LIB})

#INSTALL(TARGETS get_shapes)


