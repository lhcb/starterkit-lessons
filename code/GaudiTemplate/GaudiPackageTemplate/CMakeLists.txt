cmake_minimum_required(VERSION 2.8.5)

# Change this to the name of your package
gaudi_subdir(GaudiPackageTemplate)

# Uncomment to use GOD, make sure you are USEing a project that includes LHCb
#include(GaudiObjDesc)
#god_build_headers(xml/*.xml)
#god_build_dictionary(xml/*.xml)

# Change this to the name of your package
# You might need gaudi_add_library instead
gaudi_add_module(GaudiPackageTemplate src/*.cpp
# Any dependencies for linking
                LINK_LIBRARIES GaudiKernel)

