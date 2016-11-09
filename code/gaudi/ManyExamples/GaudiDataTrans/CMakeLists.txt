cmake_minimum_required(VERSION 2.8.5)

gaudi_subdir(GaudiDataTrans)

gaudi_add_module(GaudiDataTrans src/*.cpp
                LINK_LIBRARIES GaudiKernel)

