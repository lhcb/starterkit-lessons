CMAKE_MINIMUM_REQUIRED(VERSION 2.8.5)

find_package(GaudiProject)

gaudi_project(HelloWorld v1r0
    USE Gaudi v27r1)

#find_package(PythonLibs REQUIRED)
#find_package(pytools)

gaudi_add_library(GaudiHelloWorld *.cpp
    LINK_LIBRARIES GaudiKernel
    # PythonLibs
    NO_PUBLIC_HEADERS)
    
include_directories(.)

