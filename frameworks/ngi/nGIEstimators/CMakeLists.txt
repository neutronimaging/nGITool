cmake_minimum_required(VERSION 3.14)

set(TARGET_NAME "nGIEstimators")
project(nGITool LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    ../../../../imagingsuite/core/kipl/kipl/include
    ../../../../imagingsuite/core/algorithms/ImagingAlgorithms/include
    ../../../../imagingsuite/core/modules/ModuleConfig/include
    ../nGIFramework/include
)

if (APPLE)
    include_directories(
        ../../../../ExternalDependencies/macos/include
    )
endif()

if (WIN32)
    include_directories(
        ../../../../imagingsuite/external/include/
    )
endif()

link_directories(../../../../install/lib)

set(INC_FILES
    include/nGIBLUEEstimator.h
    include/nGILSEstimator.h
    include/nGIStandardEstimator.h
    include/nGIEstimators.h
    include/nGIPenalizedMLEstimator.h
    include/ngiestimators_global.h
    # stdafx.h
)

set(SRC_FILES
    src/nGIEstimators.cpp
    src/nGIPenalizedMLEstimator.cpp
    src/nGIBLUEEstimator.cpp
    src/nGILSEstimator.cpp
    src/nGIStandardEstimator.cpp
    # stdafx.h
    # stdafx.cpp
    # dllmain.cpp
)


add_library(${TARGET_NAME} SHARED
  ${SRC_FILES}
  ${INC_FILES}
)


set_target_properties(${TARGET_NAME} PROPERTIES CXX_VISIBILITY_PRESET hidden)

target_compile_definitions(${TARGET_NAME} PRIVATE NGIESTIMATORS_LIBRARY HAVE_NEXUS)

add_definitions(-DHAVE_NEXUS)

message(${CMAKE_HOST_SYSTEM_PROCESSOR})

if (WIN32)
    
    target_link_libraries(${TARGET_NAME}
    PUBLIC
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
            nGIFramework
            LibXml2::LibXml2
            TIFF::TIFF
            # ${LAPACK_LIBRARIES}
            # # LAPACK::LAPACK
            # ${BLAS_LIBRARIES}
            # # BLAS::BLAS
            ${ARMADILLO_LIBRARIES}
            # FFTW::Double
            # FFTW::Float
            ${CFITSIO_LIBRARIES}
            # ${NEXUS_LIBRARIES}
    )
endif()

if (LINUX)
    target_link_libraries(${TARGET_NAME}
        PUBLIC
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
