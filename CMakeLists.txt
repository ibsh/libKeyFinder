cmake_minimum_required(VERSION 3.0)
project(libkeyfinder)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(BUILD_STATIC_LIBS)
  message(STATUS "Building libKeyFinder as a static library")
  add_library(keyfinder STATIC
    audiodata.cpp
    chromagram.cpp
    chromatransform.cpp
    chromatransformfactory.cpp
    fftadapter.cpp
    keyclassifier.cpp
    keyfinder.cpp
    lowpassfilter.cpp
    lowpassfilterfactory.cpp
    spectrumanalyser.cpp
    temporalwindowfactory.cpp
    toneprofiles.cpp
    windowfunctions.cpp
    workspace.cpp
    constants.cpp
  )
else()
  message(STATUS "Building libKeyFinder as a dynamic library")
  add_library(keyfinder SHARED
    audiodata.cpp
    chromagram.cpp
    chromatransform.cpp
    chromatransformfactory.cpp
    fftadapter.cpp
    keyclassifier.cpp
    keyfinder.cpp
    lowpassfilter.cpp
    lowpassfilterfactory.cpp
    spectrumanalyser.cpp
    temporalwindowfactory.cpp
    toneprofiles.cpp
    windowfunctions.cpp
    workspace.cpp
    constants.cpp
  )
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package(FFTW REQUIRED)
target_link_libraries(keyfinder PUBLIC FFTW::FFTW)
target_include_directories(keyfinder PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

install(TARGETS keyfinder DESTINATION lib)
install(FILES
  audiodata.h
  chromagram.h
  chromatransform.h
  chromatransformfactory.h
  fftadapter.h
  keyclassifier.h
  keyfinder.h
  lowpassfilter.h
  lowpassfilterfactory.h
  spectrumanalyser.h
  temporalwindowfactory.h
  toneprofiles.h
  windowfunctions.h
  workspace.h
  constants.h
  exception.h
  binode.h
  DESTINATION include/keyfinder)

set(LIBKEYFINDER_VERSION 2.2.2)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/libKeyFinder.pc.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/libKeyFinder.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libKeyFinder.pc DESTINATION lib/pkgconfig)

add_subdirectory(tests)
