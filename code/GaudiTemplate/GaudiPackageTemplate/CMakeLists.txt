cmake_minimum_required(VERSION 2.8.5)

# Change this to the name of your package
gaudi_subdir(GaudiPackageTemplate)

# Change this to the name of your package
# You might need gaudi_add_library instead
gaudi_add_module(GaudiPackageTemplate src/*.cpp
# Any dependencies for linking
                LINK_LIBRARIES GaudiKernel)

