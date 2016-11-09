---
title: "Hello World in the Gaudi Framework"
teaching: 20
exercises: 30
questions:
- "How do I make a simple Gaudi program?"
objectives:
- "Learn how to write a simple test Gaudi program."
keypoints:
- "Hello world in Gaudi."
---

## Hello World in Gaudi

The following is a minimum example to build an algorithm in Gaudi as a new package.

### Project

First, you will need to set up a new project, with a package in it. This is done to match the way that most Gaudi projects work, with packages inside.  This is the recommended directory structure for this example:

```
└── AGaudiProject
    ├── CMakeLists.txt
    └── GaudiHelloWorld
        ├── CMakeLists.txt
        ├── src
        │   ├── HelloWorld.cpp
        │   └── HelloWorld.h
        └── options
            └── gaudi_opts.py
```

Here is an example top-level `CMakeLists.txt` for a new LHCb package:

```cmake
cmake_minimum_required(VERSION 2.8.5)

find_package(GaudiProject)

gaudi_project(AGaudiProject v1r0
              USE Gaudi v27r1)
```

This first line sets the CMake version, and the second loads the GaudiProject CMake module. Then a new project (`AGaudiProject`) is declared, and Gaudi is set as a dependency. Gaudi automatically looks for directories in the current one for packages. This is done because it makes it simple to grab a few packages, and they are automatically picked up by the project, and if the rest of the project is built and ready elsewhere in the path, the local and remote portions are combined. This makes
it easy to build a small piece of a project with rebuilding the entire project, or add a piece to a project.

A further organizational tool are subprojects; which are simply one more layer of folders. We don't need it for this project, but to add it is as simple as adding one more directory to the path. No extra CMakeLists are needed. It is commonly used to factor out common code between multiple projects, with the subprojects living in separate git repositories.

An example of this would be writing an Algorithm for DaVinci; DaVinci is the project, Phys is the subproject, and DaVinciUser is the package, and is the only piece you need to clone in git. The code lives in the Phys git repository, which means it could be used by other projects too.

The package needs to be created in the `GaudiHelloWorld` subdirectory, and has a CMakeLists.txt that looks like this:

```cpp
cmake_minimum_required(VERSION 2.8.5)

gaudi_subdir(GaudiHelloWorld)

gaudi_add_module(GaudiHelloWorld src/*.cpp
                LINK_LIBRARIES GaudiKernel)
```

This is tagged as a Gaudi subdirectory. The module we are making is added as `GaudiHelloWorld`, and linked to the Gaudi Kernel. More advanced algorithms may need to be linked to more libraries, including some that are discovered by `find_package`.

### The header file

To create an algorithm, the following header file is used:

```cpp
#pragma once

#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/Property.h"
#include "GaudiKernel/MsgStream.h"

class HelloWorldEx : public Algorithm {
public:
    HelloWorldEx(const std::string& name, ISvcLocator* pSvcLocator); 
    StatusCode initialize() override;
    StatusCode execute() override;
    StatusCode finalize() override;
    StatusCode beginRun() override;
    StatusCode endRun() override;
};
```

This creates a new algorithm, and overrides the 5 user-accessible functions. Many algorithms will not need to override all of these. The constructor is needed primarily to delegate to the Algorithm constructor, and is also needed if you want to use it to initialize member variables.

> ## Note on inheriting constructors
> 
> If you don't need to add anything to the constructor, you can inherit the default constructor by replacing the constructor line above with:
>
> ```cpp
using Algorithm::Algorithm;
```
>
> And if all you need a non-default constructor for is to initialize member variables, that can be done in the inline in their definition instead.
{: .callout}

### The implementation

The implementation is simple:

```cpp
#include "HelloWorldEx.h"

DECLARE_COMPONENT(HelloWorldEx)
```

Here, we include our header file and use the `DECLARE_COMPONENT` macro to register this as a Gaudi factory in Gaudi's Registry singleton.

```cpp
HelloWorldEx::HelloWorldEx(const std::string& name, ISvcLocator* ploc) :
    Algorithm(name, ploc) {
    }
```

The only requirement for the constructor is to pass on the two arguments to the Algorithm constructor, written here with C++ forwarding syntax. Any code here will be run when the C++ object is created, so can be used for local initialization.

```cpp
StatusCode HelloWorldEx::initialize() {
  
  StatusCode sc = Algorithm::initialize();
  if(sc.isFailure() ) return sc;

  info() << "Hello World: Inilializing..." << endmsg;
  return StatusCode::SUCCESS;
}
```

This is an optional initialization method. If run, it should first call the base class's initialize method, and return the status code if it was not successful. After that, any initialization code you may need can be added. In this example, we are simply printing a message.

```cpp
StatusCode HelloWorldEx::execute() {
  info() << "Hello World: Executing..." << endmsg;
  return StatusCode::SUCCESS;
}
```

This is the execute method. It will run once per event. This usually is the most important method, and ideally is the only one present. Most of the other methods break the concept of stateless algorithms, making this hard to move into a multithreaded framework.

```cpp
StatusCode HelloWorldEx::finalize() {
  info() << "Hello World: Finalizing..." << endmsg;
  return Algorithm::finalize(); // must be executed last
}
```

This is the final method. If you do implement it, you should end by passing on to the base class finalize method.

```cpp
StatusCode HelloWorldEx::beginRun() {
  info() << "Hello World: Begining run..." << endmsg;
  return StatusCode::SUCCESS;
}

StatusCode HelloWorldEx::endRun() {
  info() << "Hello World: Ending run..." << endmsg;
  return StatusCode::SUCCESS;
}
```

If you need to perform operations at the beginning or ending of a run, these methods are available.



### Performing the run

An example `gaudi_opts.py` options file that uses our algorithm:

```python
from Gaudi.Configuration import *
from Configurables import HelloWorldEx

ApplicationMgr().EvtMax = 10
ApplicationMgr().EvtSel = "NONE"

alg = ExHelloWorld()
ApplicationMgr().TopAlg.append(alg)
```

Note the `EvtSel = "NONE"` statement; that sets the input events to none, since we are not running over a real data file or detector.

To run, the following commands can be used:

```bash
$ lb-project-init
$ make
$ ./build.x86_64-slc6-gcc49-opt/run gaudirun.py GaudiHelloWorld/options/gaudi_opts.py
```

> ## Code
>
> The code for these files is in the DevelopKit repository, under `code/gaudi/ManyExamples`.
{: .callout}

> ## Futher reading
> 
> There are further examples in the repository for [Gaudi/GaudiPython/src/Test](https://gitlab.cern.ch/lhcb/Gaudi/tree/future/GaudiPython/src/Test).
> 
{: .callout}
