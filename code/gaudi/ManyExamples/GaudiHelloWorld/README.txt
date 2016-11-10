Hello World Gaudi example:

To build, from parent directory:

lb-project-init
make
./build.x86_64-slc6-gcc49-opt/run gaudirun.py GaudiHelloWorld/options/gaudi_opts.py


Note that the master CMakeLists really only needs to USE the package Gaudi; the use of Lbcoms is for the other subdirectories.
