cmake_minimum_required(VERSION 3.0)
project(libkeyfinder VERSION 2.2.2)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

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
target_link_libraries(keyfinder fftw3)

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
  DESTINATION include/keyfinder)

add_subdirectory(tests)
