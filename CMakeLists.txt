# define minimum cmake version
cmake_minimum_required(VERSION 3.30 FATAL_ERROR)

# execute_process(
#   COMMAND git describe --tags --abbrev=0 | cut -d'v' -f 2
#   WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
#   OUTPUT_VARIABLE KIPTOOL_VERSION
#   OUTPUT_STRIP_TRAILING_WHITESPACE
# )

project(nGITool VERSION 1.0 LANGUAGES CXX)

set(CMAKE_FIND_DEBUG_MODE OFF)

if(NOT DEFINED CMAKE_CXX_STANDARD)

endif(NOT DEFINED CMAKE_CXX_STANDARD)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE
      "Release"
      CACHE
        STRING
        "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
        FORCE
  )
endif(NOT CMAKE_BUILD_TYPE)



file(TO_CMAKE_PATH "$ENV{QTPATH}/lib/cmake/;${CMAKE_BINARY_DIR}/generators" PREFIX_PATH)
file(TO_CMAKE_PATH "${PROJECT_SOURCE_DIR}/cmake/;${CMAKE_BINARY_DIR}/generators" MODULE_PATH)

# Set environment variables so that CMAKE can find the packages
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${PREFIX_PATH})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${MODULE_PATH})
message("CMAKE_PREFIX_PATH is: ${CMAKE_PREFIX_PATH}")
message("CMAKE_MODULE_PATH is: ${CMAKE_MODULE_PATH}")
message("QTPATH is: $ENV{QTPATH}")
set(CMAKE_FIND_PACKAGE_PREFER_CONFIG TRUE)
find_package(Python3 COMPONENTS Interpreter Development)

find_package(Armadillo REQUIRED)
find_package(pybind11 REQUIRED)
find_package(libxml2 REQUIRED)
find_package(OpenBLAS REQUIRED)
find_package(TIFF REQUIRED)
find_package(FFTW3 REQUIRED)
find_package(cfitsio REQUIRED)
find_package(ZLIB REQUIRED)
find_package(HDF5 REQUIRED)

# Add imagingsuite install path to CMAKE_PREFIX_PATH
set(IMGSUITE_BUILD_DIR "${PROJECT_SOURCE_DIR}/../build-imagingsuite/Release")
list(APPEND CMAKE_PREFIX_PATH "${IMGSUITE_BUILD_DIR}")
list(APPEND CMAKE_PREFIX_PATH "${IMGSUITE_BUILD_DIR}/lib")

# find_package(kipl REQUIRED CONFIG)
# find_package(ModuleConfig REQUIRED CONFIG)
# find_package(ImagingAlgorithms REQUIRED CONFIG)
# find_package(ReaderConfig REQUIRED CONFIG)
# find_package(QtAddons REQUIRED CONFIG)
# find_package(ReaderGUI REQUIRED CONFIG)
# find_package(QtModuleConfigure REQUIRED CONFIG)
# find_package(QtImaging REQUIRED CONFIG)

if (WIN32)
  find_package(dirent)
endif()

if (WIN32)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Release)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/Release)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/Release)
elseif (LINUX)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
elseif (APPLE)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  include_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
endif()

if("${Python3_VERSION_MAJOR}.${Python3_VERSION_MINOR}" VERSION_LESS "3.5")
  message(
    FATAL_ERROR
      "Python v3 interpreter must be greater than or equal to 3.5. Found ${Python3_VERSION_MAJOR}.${Python3_VERSION_MINOR}"
  )
endif()

# Set path to imagingsuite build/install directory
set(IMAGINGSUITE_INSTALL_DIR "${CMAKE_SOURCE_DIR}/../build-imagingsuite/Release" CACHE PATH "Path to imagingsuite installation")

# Add imagingsuite library path
link_directories("${IMAGINGSUITE_INSTALL_DIR}/lib")

# Add imagingsuite include paths
include_directories(
    ../imagingsuite/core/kipl/kipl/include
    ../imagingsuite/core/modules/ModuleConfig/include
    ../imagingsuite/core/modules/ReaderConfig
    ../imagingsuite/core/algorithms/ImagingAlgorithms/include
    ../imagingsuite/GUI/qt/QtAddons
    ../imagingsuite/GUI/qt/QtImaging
    ../imagingsuite/GUI/qt/QtModuleConfigure
    ../imagingsuite/core/modules/ReaderGUI

    link_directories("${IMGSUITE_BUILD_DIR}/lib")
)


execute_process(
  COMMAND git describe --tag
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
  RESULT_VARIABLE HAVE_GIT_VERSION_INFO
  OUTPUT_VARIABLE GIT_VERSION_INFO
  OUTPUT_STRIP_TRAILING_WHITESPACE
  ERROR_QUIET
)
if(${HAVE_GIT_VERSION_INFO} EQUAL 0)
  message(STATUS "Got version from Git: ${GIT_VERSION_INFO}")
  add_definitions(-DVERSION="${GIT_VERSION_INFO}")
else()
  set(GIT_VERSION_INFO "${PROJECT_VERSION}")
  message(WARNING "Could not get version from Git, using project version: ${GIT_VERSION_INFO}")
  add_definitions(-DVERSION="${GIT_VERSION_INFO}")
endif()

include(CTest)

execute_process(
  COMMAND git describe --tag
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
  RESULT_VARIABLE HAVE_GIT_VERSION_INFO
  OUTPUT_VARIABLE GIT_VERSION_INFO OUTPUT_STRIP_TRAILING_WHITESPACE
)
if(${HAVE_GIT_VERSION_INFO} EQUAL 0)
  message(STATUS "Got version from Git: ${GIT_VERSION_INFO}")
  add_definitions(-DIMGSUITE_VERSION="${GIT_VERSION_INFO}")
endif()

if(WIN32)
add_compile_options(
  /openmp 
  /O2
  )
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer")
  add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
    -Wunused
    -Wshadow
    -Winit-self
    -Wpointer-arith
    -Woverloaded-virtual
    -Wold-style-cast
    -Wcast-qual
    -Wcast-align
  )
  add_compile_options(
    $<$<COMPILE_LANGUAGE:CXX>:-Woverloaded-virtual>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-operator-names>
  )
endif()

set(PYTHONDIR kipl)
set(INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR})

set(EXPORT_NAME ${PROJECT_NAME}-targets)

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
SET(CMAKE_SKIP_BUILD_RPATH FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_BUILD_RPATH_USE_ORIGIN TRUE)

if(NOT APPLE)
  set(CMAKE_INSTALL_RPATH $ORIGIN/../lib)
else()
  set(CMAKE_INSTALL_RPATH @loader_path/../Frameworks @loader_path/../../../lib @loader_path/lib @loader_path)
endif()
# turn on testing
enable_testing()

## modules
add_subdirectory(frameworks/ngi/nGIFramework)
add_subdirectory(frameworks/ngi/nGIEstimators)
add_subdirectory(frameworks/ngi/nGIPreprocessing)

## applications
add_subdirectory(applications/nGItool)

