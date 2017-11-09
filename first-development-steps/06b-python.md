# Python in the upgrade era

{% objectives "Learning Objectives" %}
- "Learn about the new Python."
{% endobjectives %}


# A new Python

About ten years ago, Guido Van Rossum, the Python author and Benevolent Dictator for Life (BDFL), along with the Python community, decided to make several concurrent backward incompatible changes to Python 2.5 and release a new version, Python 3.0. The main changes were:

* Using unicode strings as default, with the old string type becoming a full featured binary type
* Changing several builtins, for example
    * The print statement became a function, allowing more consistent syntax and the use of the word print as a name
    * The confusing `input` removed, and `raw_input` now renamed to `input`
    * Simpler `exec`
    * Division is now split between `/` float division and `//` truncating division
* Improved exception tracing, with chaining
* Improved function call syntax with annotations and keyword only arguments, replacing little used tuple parameter unpacking
* More class constructor features, such as nicer metaclass syntax, keyword arguments, `__prepare__`
* Renamed standard libraries, to be more consistent
* Removal of a lot of depreciated features, including old-style classes
* Removal of a lot of depreciated syntax that had become learner stumbling blocks
* Adding `nonlocal` variables
* Extended tuple unpacking, like `first, *rest = makes_a_tuple()`
* Removing the proliferation of `.pyc` files, instead using `__pycache__` directories
* Automatic selection of C-based standard library modules over pure Python ones if available
* Unified the `int` and `long` types into one unlimited length integer type

Unfortunately, this list was comprehensive enough to break virtually every python script ever written. So, to ease the transition, 3.0 and 2.6 were released simultaneously, with the other, backward compatible new features of 3.0 being also included in 2.6. This happened again with the releases of 3.1 and 2.7. Not wanting to maintain two Pythons, the BDFL declared that 2.7 was the last Python 2 series release.

{% callout "Side note about speed" %}
These changes (mostly the unicode one) also made Python much slower in version 3.0. Since then, however, there have been many speed and memory improvements. Combined with new C extensions for some modules, Python 3 is now usually as fast or faster than Python 2.
{% endcallout %}


The original, officially sanctioned upgrade path was one of the biggest issues with moving to Python 3. A script, `2to3`, was supposed to convert code to Python 3, and then the old version could be eventually dropped. This script required a lot of manual intervention (things like the unicode strings require knowledge of the programmer's intent), and required library authors to maintain two separate versions of the code. This hindered initial adoption with many major libraries unwilling to support two versions for
Python 3 support.

Unofficial authors tried making a new script, `3to2`, which worked significantly better, but still was hindered by the dual copies of code issue.

Another decision also may have slowed adoption. Part way through the development of Python 3.2 up to 3.4, the decision was made to avoid adding any new features, to give authors time to adopt code to a stable Python 3. This statement could be taken in reverse; why update to Python 3 when it does not have any new features to improve your program? The original changes (as listed above) were not enough to cause mass adoption.

This dreary time in Python development is now drawing to a close, thanks to a change in the way authors started approaching Python compatibility. There is such a good overlap between Python 2.6 or Python 2.7 and Python 3.3+ that a single code base can support them both. The reason for this is the following three things:

* Good Python 2 is almost the same as Python 3. The things that were dropped were mostly things you shouldn't do in Python 2 anyway.
* Several changes in syntax are available in Python 2 using `__future__`
* The remaining changes can mostly be wrapped in libraries

These were capitalized by the unofficial library authors, and now almost every library is available as a single code base for Python 2 and 3. Most of the new standard libraries, and even a few language features, are regularly backported to Python 2, as well.

# Libraries to ease in the transition

## Six

The original compatibility library, [six](https://pythonhosted.org/six/) (so named because 2 times 3 is 6), provides tools to make writing 2 and 3 compatible code easy. You just import six, and then access the renamed standard libraries from `six.moves`. There are wrappers for the changed features, such as `six.with_metaclass`.

These features are not hard to wrap yourself, so many libraries implement their own `six` wrapper to reduce dependencies and overhead.

## Future

This is a newer library with a unique approach. Instead of forcing a usage of a special wrapper, the idea of [future](http://python-future.org) is to simply allow code to be written in Python 3, but work in Python 3. For example, `from builtins import input` will do nothing on Python 3 (`builtins` is where `input` lives), but on Python 2 with `future` installed, `builtins` is part of `future` and will import the `future` version. You can even patch in the Python 3 standard library names
with a `standard_library.install_aliases()` function.

Future also comes with it's own version of the conversion scripts, called `futurize` and `pasteurize`, which use the future library to make code that runs on one version run on both versions. An alpha feature, the `autotranslate` function, can turn a library that supports only Python 2 into a Python 3 version on import.

## Backports

Several of the new libraries and features have been backported to Python 2. I'm not including ones that were backported in an official Python release, like `argparse`.

* `pathlib`: A simple, object oriented path library from Python 3.4 
* `enum`: A python package for enumerations from Python 3.4
* `mock`: A version of `unittest.mock` from Python 3.3 
* `futures`: This is the `concurrent.futures` package in Python 3.2
* `statistics`: From Python 3.4
* `selectors34`: The `selectors` package from Python 3.4 
* `typing`: Type hints from Python 3.5
* `trollius`: The `asyncio` package, with a new syntax for `yield from`, from Python 3.4
* Smaller changes: `configparser`, `subprocess32`, `functools32`, and the various `backports`-dot-something packages. 

# New features in modern Python

These are features that have been released in a version of Python after 3.0 that are not in the older Python 2 series:

* Matrix multiplication operator, `@` (3.5)
* Special `async` and `await` syntax for asynchronous operations (3.5)
* Unpacking improvements, so that the `*` and `**` operators work in more places like you'd expect (3.5)
* Function signatures now in easy to use object (3.3)
* Improvements to Windows support (Windows launcher, recent versions of Visual C++) (3.2, 3.4, 3.5, 3.6)
* Delegation to a subgenerator, `yield from`, finally allows safe factorisation of generators (3.3)


### Formatted string literals (3.6)

Finally! You can write code such as the following now:

```python
x = 2
print(f"The value of x is {x}")
```

This is indicated by the `f` prefix, and can take almost any valid python expression. It does not have the scope issues that the old workaround, `.format(**locals())` encounters. 


### Syntax for variable annotations (3.6)

This will be great for type hints, IDE's, and Cython, but the syntax is a little odd for Python. It's based on function annotations.
A quick example:

```python
an_empty_list_of_ints: List[int] = []
will_be_a_str_later: str 
```

This stores the variable name and the annotation in an `__annotations__` dictionary for the module or the class that they are in.

## Simi-ordered dictionaries (3.6)

Python dictionaries are now partially ordered; due to huge speedups in the C definition of ordered dicts, the `dict` class is now guarantied to iterate in order as long as nothing has been changed since the `dict` creation. This may sound restrictive, but it enables many features; you can now discover the order keyword arguments were passed, the order class members were added, and the order of `{}` dicts. If you want to continue to  keep or control the order, you should move the `dict` to an
`OrderedDict`, as before. This makes ordered dictionaries much easier to create, too.

{% callout "Warning" %}
Only class member order and keyword argument order are ensured by the language; the ordering of `{}` is an implementation detail. This detail works in both CPython 3.6 and all versions PyPy, however. This may become language mandated in the future.
{% endcallout %}


### Other smaller features:

* Underscores in numeric literals. You can add arbitrary spacers to numbers now, such as `1_000_000`.
* Windows encoding improvements.
* Simpler customization of class creation, using `__init_subclass__` class method.
* Descriptor access to the name of the class and the descriptor, using `__set_name__`. 
* A file system path protocol, `__fspath__()`, allows any object to indicate that it represents a path. Finally pathlib works without wrapping it in a `str()`!
* Better support for `async` list comprehensions, and `async` generators.
* A `secrets` module for password related randomization functions.

## Status of Python

The current status of the python releases is as follows:

* Python 2.5: Dead.
* Python 2.6: Most libraries are dropping support, officially discontinued, but still on some legacy systems, like the default environment in SL6.
* Python 2.7: The officially supported Python 2 release, critical security flaws fixed till roughly 2020. PyPy supports 2.7.10. Windows version is stuck requiring Visual Studio 2008 for builds.
* Python 3.0-3.2: Never used significantly, no library support.
    * Python 3.3: Better backwards compatibility makes this the first generally used Python 3, with Windows downloads [outpacing](http://ianozsvald.com/2013/04/15/more-python-3-3-downloads-than-python-2-7-for-past-3-months/) Python 2.7 for the first time. `u""` was added back in as a no-op. Note that PyPy3 is currently based on Python 3.3.5.
* Python 3.4: Addition of asyncio features and pathlib provided even more interest.
* Python 3.5: New features, such as matrix multiplication, are accelerating the transition from Python 2.
* Python 3.6: The addition of format strings make simple scripts much easier and cleaner.

{% keypoints "Further reading" %}
* [The old Python wiki page](https://wiki.python.org/moin/Python2orPython3)
* [What's new in Python](https://docs.python.org/3.7/whatsnew/3.6.html)
* [10 awesome features of Python that you can't use](https://asmeurer.github.io/python3-presentation/slides.html#1)
{% endkeypoints %}



