cmake_minimum_required(VERSION 2.8.5)

gaudi_subdir(GaudiHelloWorld)

gaudi_add_module(GaudiHelloWorld src/*.cpp
                 LINK_LIBRARIES GaudiAlg)

