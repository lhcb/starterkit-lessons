---
title: "Running and testing"
teaching: 10
exercises: 20
questions:
- "How to I work with testing?"
objectives:
- "Learn about running examples."
- "Learn about running tests."
keypoints:
- "Work with tests."
---

One of the most important components of writing code is testing. The following section will show you how to run examples in tests in Gaudi; this is also mostly compatible with the packages that are built on top of Gaudi, such as DaVinci and Gauss.

## Running an example

The Gaudi examples are available in `Gaudi/GaudiExamples`. On lxplus, you can grab them and build with a partial checkout:

~~~
lb-dev Gaudi v28r0
cd Gaudi_v28r0
git lb-use Gaudi
git lb-checkout Gaudi/v28r0 GaudiExamples
make
~~~
{: .input}

Now, you have Gaudi and just the GaudiExamples subdirectory (project). This is a good place to start looking around to see how Gaudi works; for now we'll try running an example.

~~~
./run gaudirun.py GaudiExamples/options/AlgSequencer.py
~~~
{: .input}

This will run a a few algorithms in sequence, and will output info messages for ten "events" (no data).

## Testing

> ## Searching the source
> 
> In this case, you can search the source in the current directory and below by:
> 
> ~~~
git grep AlgSequencer
~~~
> {: .input}
>
> To search the online source, you can do:
>
> ~~~
Lbglimpse AlgSequencer Ganga v28r0
~~~
> {: .input}
{: .discussion}

There are two methods to write tests:

### QMT tests

For an old-style QMT test, you add a QMT file in `tests/qmtest/gaudiexamples.qms/` that looks like `testname.qmt`. The file contains xml:

<pre><code> &lt;?xml version=&quot;1.0&quot; ?&gt;&lt;!DOCTYPE extension  PUBLIC '-//QM/2.3/Extension//EN'  'http://www. codesourcery.com/qm/dtds/2.3/-//qm/2.3/extension//en.dtd'&gt;
 &lt;extension class=&quot;GaudiTest.GaudiExeTest&quot; kind=&quot;test&quot;&gt;
   &lt;argument name=&quot;program&quot;&gt;&lt;text&gt;gaudirun.py&lt;/text&gt;&lt;/argument&gt;
   &lt;argument name=&quot;args&quot;&gt;&lt;set&gt;&lt;text&gt;$GAUDIEXAMPLESROOT/options/AlgSequencer.py&lt;/text&gt;&lt;/    set&gt;&lt;/argument&gt;
   &lt;argument name=&quot;use_temp_dir&quot;&gt;&lt;enumeral&gt;true&lt;/enumeral&gt;&lt;/argument&gt;
   &lt;argument name=&quot;reference&quot;&gt;&lt;text&gt;refs/AlgSequencer_pyopts.ref&lt;/text&gt;&lt;/argument&gt;
 &lt;/extension&gt;</code></pre>

This contains a few important points. The `AlgSequencer.ref` is a copy of the output that will be used to check the result. It is at `tests/refs/AlgSequencer_pyopts.ref`. The compare is smart enough to mask out numbers like time and date for you. The opts file to run is also here.

### Python tests

Another way to create a test is through a python system; a matching example is at `tests/qmtest/newFormat/algsequencer_pyopts_test.py`. The contents:

```python
# -*- coding: utf-8 -*-
import BaseTest
from BaseTest import *

class Test(BaseTest):

    def __init__(self):
        BaseTest.__init__(self)
        self.name = os.path.basename(__file__)[:-5]
        self.program="gaudirun.py"
        self.args=["$GAUDIEXAMPLESROOT/options/AlgSequencer.py"]
        self.reference = "refs/AlgSequencer_pyopts.ref"
```

## Running a test

You can run all the tests with `make test`. This interally runs the ctest command in the build directory. You can pass on arguments to the ctest file through the make command though the `ARGS` variable. For example, to run a single named test:

~~~
make test ARGS="-R algsequencer_pyopts"
~~~
{: .input}


The arguments that can be passed to `ctest` can be found with `ctest --help`.
Some examples:

* `-N`: just print the names of the tests
* `-R <regex>`: select tests by regular expression
* `-V`: verbose printout (extra details, command line)

Note that `gaudirun.py` can take read and run a `.qmt` file directly. This allows you to see the output of the run, as `ctest` hides the output. 


> ## See also
> 
> [Hackathon introduction](https://gitlab.cern.ch/lhcb/upgrade-hackathon-setup)
{: .callout}
