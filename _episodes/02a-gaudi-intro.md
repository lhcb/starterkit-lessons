---
title: "Introduction to the Gaudi framework"
teaching: 20
exercises: 30
questions:
- "How is Gauid currently used?"
objectives:
- "Learn the basics of the Gaudi framework."
keypoints:
- "Work with Gaudi."
---

# What is the Gaudi framework?

The Gaudi framework coordinates **Data Objects**, which can hold a variety of data. A **Transiant Event Store (TES)** stores data objects in a way to make them accessible to the rest of the framework, and parts of it can be made persistent in a ROOT format file. The data in the TES is created and accessed by  **Algorithms**, which produce data objects and process data objects. The Application Manager manages these components.



## Algorithms

This is the most important component of the framework for an user to know. Algorithms are called once per physics event, and (traditionally) implement three methods beyond constructor/destructor: `initialize`, `execute`, and `finalize`. 


## Hello World in Gaudi

The following is a minimum example to build an algorithm in Gaudi as a new package.

First, you will need to set up a new package. Here is an example `CMakeLists.txt` for a new LHCb package:

```cmake
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.5)

find_package(GaudiProject)

gaudi_project(HelloWorld v1r0
    USE Gaudi v27r1)

gaudi_add_library(GaudiHelloWorld *.cpp
    LINK_LIBRARIES GaudiKernel
    NO_PUBLIC_HEADERS)
    
include_directories(.)
```

This first sets the CMake version and loads the GaudiProject CMake module. Then a new project (HelloWorld) is declared, and Gaudi is set as a dependency. 
The library we are making is added as GaudiHelloWorld, linked to the Gaudi Kernel, and no headers are exported. Finally, the current directory is added to the CMake includes.

To create an algorithm, the following header file is used:

```cpp
#pragma once

#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/Property.h"
#include "GaudiKernel/MsgStream.h"

class HelloWorld : public Algorithm {
public:
    HelloWorld(const std::string& name, ISvcLocator* pSvcLocator); 
    StatusCode initialize() override;
    StatusCode execute() override;
    StatusCode finalize() override;
    StatusCode beginRun() override;
    StatusCode endRun() override;

private:
    bool m_initialized;

    /// These data members are used in the execution of this algorithm
    /// They are set in the initialisation phase by the job options service
    IntegerProperty m_int;
    DoubleProperty  m_double;
    StringProperty  m_string;
};
```

The implementation is simple:

```cpp
#include "HelloWorld.h"

DECLARE_COMPONENT(HelloWorld)

HelloWorld::HelloWorld(const std::string& name, ISvcLocator* ploc) :
    Algorithm(name, ploc) {
        m_initialized = false;
    }

StatusCode HelloWorld::initialize() {
  if( m_initialized ) return StatusCode::SUCCESS;

  info() << "initializing...." << endmsg;
  m_initialized = true;
  return StatusCode::SUCCESS;
}

StatusCode HelloWorld::execute() {
  info() << "executing...." << endmsg;
  return StatusCode::SUCCESS;
}

StatusCode HelloWorld::finalize() {
  info() << "finalizing...." << endmsg;

  m_initialized = false;
  return StatusCode::SUCCESS;
}

StatusCode HelloWorld::beginRun() {
  info() << "beginning new run...." << endmsg;

  m_initialized = true;
  return StatusCode::SUCCESS;
}

StatusCode HelloWorld::endRun() {
  info() << "ending new run...." << endmsg;

  m_initialized = true;
  return StatusCode::SUCCESS;
}
```

An example `gaudi_opts.py` options file that does nothing other than add our algorithm:

```python
from Gaudi.Configuration import *
from Configurables import HelloWorld

alg = HelloWorld()

ApplicationMgr().TopAlg.append(alg)
```

To run, the following commands can be used:
```bash
$ lb-project-init
$ make
$ ./build.x86_64-slc6-gcc49-opt/run gaudirun.py gaudi_opts.py
```

The code for these files is [here](/DevelopKit/code/gaudi/hello_world).
