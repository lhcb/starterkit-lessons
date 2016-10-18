CMAKE_MINIMUM_REQUIRED(VERSION 2.8.5)

find_package(GaudiProject)

gaudi_project(DataTrans v1r0
    USE Gaudi v27r1)

gaudi_add_library(GaudiDataTrans *.cpp
    LINK_LIBRARIES GaudiKernel
    NO_PUBLIC_HEADERS)
    
include_directories(.)

