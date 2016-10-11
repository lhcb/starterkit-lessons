---
title: "C++ Performance"
teaching: 10
exercises: 10
questions:
- "How do I write fast code?"
objectives:
- "Learn about fast coding."
keypoints:
- "Learn about writing performant code."
---

# The way to optimize

One of the goals of coding is to make clean, clear code that closely matches the intent of the user, both in language and syntax. Really, almost all languages and features are truly optional; programs could technically be written with a tiny subset of any modern computing language. The point of classes, exceptions, structures, and the like is to make code match intent in a manor obvious to the user. Another goal of writing code is to be able to use one piece of code in multiple cases, avoiding rewriting or maintaining multiple copies. A related goal is to be able to create it quickly with minimal hassle.

Optimization may seem to be exactly the opposite of these goals, obfuscating your original intent, making performant versions for each use case, and extending the work needed to create the code; but when carefully performed, optimization can have a minimal impact in these. There are several key points to take away:

## Know where your code is taking time

This is key; most algorithms have a bottleneck somewhere; it's been said that 1% of your code tend to take 99% of the time. It is important to **profile** your code to understand where time is being spent. Only optimize the portions that take the most time; it is not worth the impact to readability to optimize something that takes less than 5% of the total time.

There are simple tools for profiling, as well as complex. The obvious manual methods, using `std::chrono::high_resolution_clock::now()` and printing times, might be faster for a single use, but in the long run will take far more of your time than learning a profiler. There are two common classes of profilers; one samples the stack during execution (`gprof`, `linux-perf`, `google-perftools`), and the other modifies the execution of the program (`callgrind`). The later is much slower, and
can affect the proportion of time spent in calls, etc.

> ## Example for sample profiling
> 
> To use, you'll want the compiler to add profile info (`-pg`) and optionally debug info (`-g`). You also may want the compiler to avoid inline functions (`-fno-inline`). The binary can now be run through gprof:
> ```bash
$ gprof ./my_program
```
> 
> You can also use the kernal and CPU to help (may require root privileges):
>
> ```bash
$ perf record ./my_program
$ perf report ./my_program
```
> 
> Google also has a profiling tool:
>
> ```bash
$ CPUPROFILE=prof.out LD_PRELOAD=/usr/lib/libprofiler.so.0 ./my_program
$ google-pprof -text ./my_program prof.out
```
>
{: .discussion}

> ## Example for callgrind
>
> The following example shows the procedure for obtaining a graphical result of the time spent in each portion of your program:
>
> ```bash
$ g++ -g -O2 -std=c++11 -pedantic -Wall -Wextra my_program.cpp -o my_program
$ valgrind -tool=callgrind -dump-instr=yes -collect -jumps=yes -cache -sim=yes -branch-sim=yes ./my_program
$ kcachegrind
```
>
{: .discussion}

## Keep clarity a focus

Your code should have well defined, independent blocks that do as few jobs each as possible, and it should be structured to match a user's expected behavior (good documentation can allow minor deviations from this). This is, in almost all cases, something you should not have to sacrifice for speed. Package highly optimized code inside these blocks, with a clear indication of what they are supposed to do. Good function/class/etc names go a long way. Try to avoid "ugly" coding practices, as described below.

## Stay general as long as possible

If you need to write an algorithm, think of how it can be broken up. Try to solve everything generally first. If you have code that has to sort a list, it's better to separate the list sorting from your algorithm. Then, you can often find an optimised library version of the general tasks; you can write unit tests that test each part; and you can keep optimizations localised to the code they affect. Sometimes you cannot do this, but always try first.

Use template features when possible to perform tasks at compile time. Modern C++ has `constexpr` expressions that allow compile time computation to be done, which allow you to avoid hard coding calculated numbers in the code, but to leave the original expressions. You can also use macros as a last resort.

## Use the language


Some common issues with writing C++ are:

* Using globals. Don't.
* Forgetting `const`. This should be used for every interface, and in C++11, is an indication of multithread safety.
* Custom memory management inside a framework (like Gaudi) that does that for you. This can be made into a more general rule: if a framework provides something, use it. Then the framework can be upgraded, and your code will receive the benefits.
* Try to support and use pass by reference and move semantics. This allows you to avoid copying large data strutures many times.
* Using `iterator++` instead of `++iterator`, the first is often making a copy wastefully.
* Forgetting to use noexcept if you can't throw an exception; this can allow the compiler to do more optimization for you.

# Common speedups

The following are common ways to speed up computation.

* Precompute expressions, and reuse the results. If you use the expression `sin(x)*cos(y)` multiple times, you can save processor steps by precomputing it to a temporary variable. Note that if you do this excessively, you might end up with temporary variables in the main memory instead of the cache, so look for many usages or something that takes many processor cycles.
* C++ allows return value optimization; if you return an object created in the function, and it otherwise would go out of scope and be destroyed, it is moved out of the function instead of copied. (Note: This is the case with most compilers for C++11, and is required by the language in most cases for C++17). Make sure all your return statements return the same object for this optimization. (Unnamed object get this automatically, too)
* Perfect forwarding: this is a handy trick in several cases; as a common example, you can construct an object inside a vector instead of copying it in, using `emplace_back`. The method forward the arguments after the first to the constructor (the first argument), but makes it inplace inside the vector instead of a move or copy.




# To multithread or not to multithread

One of the hottest topics in programming is the use of multiple threads; on a multithreaded computer or a GPU, the promise of speedup by factors of 2+ is enticing. But there are conditions to these gains besides the obvious requirement that the procedure must be able to be done in parallel; if you have a memory bottleneck for example (very common), that will remain the limiting factor. Another common bottleneck is I/O; that is often improved by threads even on a single core processor.

If you are working inside a framework, like much of the code for LHCb, and that framework is already multithreaded, it is often slower to then try to introduce more threads; the other CPUs are already busy. Much of the work for these components is in making them **thread-safe**, that is, making them able to run in parallel with other algorithms or with themselves without conflicts for writing/reading memory.


> ## Future reading
> 
> * [https://gitlab.cern.ch/mschille/fellow-meeting-performance-talk/blob/master/talk.pdf](Performance in C++ for LHCb)
> 
{: .callout}

