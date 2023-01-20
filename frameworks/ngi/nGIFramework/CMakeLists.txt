cmake_minimum_required(VERSION 3.14)

set(TARGET_NAME "nGIFramework")
project(nGITool LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (APPLE)
include_directories(
    ../../../../imagingsuite/core/kipl/kipl/include
    ../../../../imagingsuite/core/algorithms/ImagingAlgorithms/include
    ../../../../imagingsuite/core/modules/ModuleConfig/include
    ../../../../ExternalDependencies/macos/include
    )
endif()

if (WIN32)
include_directories(
    ../../../../imagingsuite/core/kipl/kipl/include
    ../../../../imagingsuite/core/algorithms/ImagingAlgorithms/include
    ../../../../imagingsuite/core/modules/ModuleConfig/include
    ../../../../imagingsuite/external/include/
    )
endif()

link_directories(../../../../install/lib)

set(INC_FILES
    include/EstimatorBase.h
    include/ModuleItem.h
    include/ProjectionReader.h
    include/nGIEngine.h
    include/nGIFactory.h
    include/nGIProcessor.h
    include/InteractionBase.h
    include/PreprocModuleBase.h
    include/nGIConfig.h
    include/nGIException.h
    include/nGIFramework_global.h
    include/ngigenerator.h
    # stdafx.h
    # targetver.h
)

set(SRC_FILES
    src/EstimatorBase.cpp
    src/ModuleItem.cpp
    src/ProjectionReader.cpp
    src/nGIEngine.cpp
    src/nGIFactory.cpp
    src/ngigenerator.cpp
    src/InteractionBase.cpp
    src/PreprocModuleBase.cpp
    src/nGIConfig.cpp
    src/nGIException.cpp
    src/nGIProcessor.cpp
    # src/stdafx.h
    # stdafx.cpp
)


add_library(${TARGET_NAME} SHARED
  ${SRC_FILES}
  ${INC_FILES}
)


set_target_properties(${TARGET_NAME} PROPERTIES CXX_VISIBILITY_PRESET hidden)

target_compile_definitions(${TARGET_NAME} PRIVATE NGIFRAMEWORK_LIBRARY HAVE_NEXUS)

add_definitions(-DHAVE_NEXUS)

message(${CMAKE_HOST_SYSTEM_PROCESSOR})

if (WIN32)
    
    target_link_libraries(${TARGET_NAME}
    PRIVATE
        kipl
        ModuleConfig
        ImagingAlgorithms
        LibXml2::LibXml2
        TIFF::TIFF
        cfitsio::cfitsio
        )
endif()

if (APPLE)
    target_link_libraries(${TARGET_NAME}
        PUBLIC
            kipl
            ImagingAlgorithms
            ModuleConfig
            ReaderConfig
            LibXml2::LibXml2
            TIFF::TIFF
            ${CFITSIO_LIBRARIES}
    )
endif()

if (LINUX)
    target_link_libraries(${TARGET_NAME}
        PRIVATE
        kipl
        ImagingAlgorithms
        ModuleConfig
        xml2
        fftw3
        fftw3f
        cfitsio
        tiff
        armadillo
        lapack
        blas
        NeXus
        NeXusCPP
    )
endif()
install(TARGETS ${TARGET_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
