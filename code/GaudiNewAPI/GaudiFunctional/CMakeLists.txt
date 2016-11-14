cmake_minimum_required(VERSION 2.8.5)

gaudi_subdir(GaudiFunctional)

gaudi_add_module(GaudiFunctional src/*.cpp
    LINK_LIBRARIES GaudiKernel GaudiAlgLib )

