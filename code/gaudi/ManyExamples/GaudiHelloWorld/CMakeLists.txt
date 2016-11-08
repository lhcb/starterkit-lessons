CMAKE_MINIMUM_REQUIRED(VERSION 2.8.5)

gaudi_subdir(GaudiHelloWorld)

gaudi_add_module(GaudiHelloWorld src/*.cpp
                LINK_LIBRARIES GaudiAlgLib GaudiKernel)

#gaudi_install_python_modules()

