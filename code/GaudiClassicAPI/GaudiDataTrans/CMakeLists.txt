cmake_minimum_required(VERSION 2.8.5)

gaudi_subdir(GaudiDataTrans)


include(GaudiObjDesc)
god_build_headers(xml/*.xml)
god_build_dictionary(xml/*.xml)

gaudi_add_module(GaudiDataTrans src/*.cpp
    LINK_LIBRARIES GaudiKernel GaudiAlgLib )

