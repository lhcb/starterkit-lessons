---
title: "C++ Performance"
teaching: 20
exercises: 10
questions:
- "How do I write fast code?"
objectives:
- "Learn about fast coding."
keypoints:
- "Learn about writing performant code."
---

# Optimizing code

One of the goals of coding is to make clean, clear code that closely matches the intent of the user, both in language and syntax. Really, almost all languages and features are truly optional; programs could technically be written with a tiny subset of any modern computing language. The point of classes, exceptions, structures, and the like is to make code match intent in a manor obvious to the user. Another goal of writing code is to be able to use one piece of code in multiple cases, avoiding rewriting or maintaining multiple copies. A related goal is to be able to create it quickly with minimal hassle.

Optimization may seem to be exactly the opposite of these goals, obfuscating your original intent, making performant versions for each use case, and extending the work needed to create the code; but when carefully performed, optimization can have a minimal impact in these. There are several key points to take away:

## Know where your code is taking time

This is key; most algorithms have a bottleneck somewhere; it's been said that 1% of your code tend to take 99% of the time. It is important to **profile** your code to understand where time is being spent. Only optimize the portions that take the most time; it is not worth the impact to readability to optimize something that takes less than 5% of the total time.

There are simple tools for profiling, as well as complex. The obvious manual methods, using `std::chrono::high_resolution_clock::now()` and printing times, might be faster for a single use, but in the long run will take far more of your time than learning a profiler.

The following are three common classes of profilers.

### Code insertion
These modify the execution of the program, such as `callgrind`. This causes your program to run much slower, and
can affect the proportion of time spent in calls. Small calls or IO tend to be heavily impacted by using these tools.


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

### Stack sampling profilers

These sample the program stack during execution (`gprof`, `linux-perf`, `google-perftools`, `igprof`), providing minimal changes to the runtime of the program. These trade knoledge of every call for a representation of a normal run.

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

Another option is igprof, which is available on the CERN stack. In comparison with the other sampling profilers, it provides several unique advantages. The following are benifits and drawbacks compared to gprof:

* Does not require special compilation flags.
* More robust when monitoring programs that do nontrivial stuff such as calling libraries or spawning new processes and threads.
* Has a fancy HTML frontend for reports.
* Much faster sampling rate -> misses less, but produces bigger output.
* It is a bit hard to use outside of CERN (e.g. no package in the Debian repos).
* Does not seem to have an option to produce line-by-line annotated source, a gprof feature which can be quite handy in perf analysis

Compared to valgrind/callgrind:

* Does not run your code in a virtual machine -> enormously faster, however output is less precise (e.g. no cache info).
* Does not bias your performance profile by inflating the relative cost of CPU w.r.t. IO by a factor of 100.
 
> ## Examples of usage
> 
> You can profile a program as follows:
> ```bash
$ igprof -d -pp -z -o my_program.pp.gz my_program
$ igprof-analyse -d -v -g my_program.pp.gz >& my_program.pp.out
```
> 
> The first line produces a profile, the second line converts it to a readable text report.
{: .discussion}

### Kernel sampling profilers

These are similar to the other sampling profilers, but they use extra information from the Linux kernel when sampling, such as CPU performance counters. This leads them to the following benifits and drawbacks: 

* They are hopelessely unportable to non-linux OSs. Compare them with XCode Instruments on OSX and the Windows Performance Toolkit.
* The information that they provide is hardware and kernel-specific, and in particular they won't run at all if your Linux kernel is too old.
* They tend to make the system protection features of modern Linux distros (e.g. SELinux) go crazy, making it a pain to get them to run.
* As a compensation for all these drawbacks, they promise much more detailed information, comparable to what one can usually only get through callgrind, thanks to the use of hardware performance counters.
* And they can also do system-wide profiling, which is useful when studying interactions between "unrelated" processes.

Some examples are `perf` and `oprofile`.

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
* Try to support and use pass by reference and move semantics. This allows you to avoid copying large data structures many times.
* Using `iterator++` instead of `++iterator`, the first is often making a copy wastefully.
* Forgetting to use noexcept if you can't throw an exception; this can allow the compiler to do more optimization for you.

# Common speedups

The following are common ways to speed up computation.

* Precompute expressions, and reuse the results. If you use the expression `sin(x)*cos(y)` multiple times, you can save processor steps by precomputing it to a temporary variable. Note that if you do this excessively, you might end up with temporary variables in the main memory instead of the cache, so look for many usages or something that takes many processor cycles.
* C++ allows return value optimization; if you return an object created in the function, and it otherwise would go out of scope and be destroyed, it is moved out of the function instead of copied. (Note: This is the case with most compilers for C++11, and is required by the language in most cases for C++17). Make sure all your return statements return the same object for this optimization. (Unnamed object get this automatically, too)
* Perfect forwarding: this is a handy trick in several cases; as a common example, you can construct an object inside a vector instead of copying it in, using `emplace_back`. The method forward the arguments after the first to the constructor (the first argument), but makes it in-place inside the vector instead of a move or copy.
* Watch for slow general functions, like `pow(x,2)`, that are designed to be flexible at runtime, when a simple compile time version `x*x` is available. Profile! 

## Approximations

Pay attention to the level of precision you need; if you can make an approximation to an expensive calculation, this can reduce your computing time significantly. Be careful, though, often it can be difficult to speed up functions in the standard library or high-performance libraries; test and measure any improvements you try to make.


# To multithread or not to multithread

One of the hottest topics in programming is the use of multiple threads; on a multithreaded computer or a GPU, the promise of speedup by factors of 2+ is enticing. But there are conditions to these gains besides the obvious requirement that the procedure must be able to be done in parallel; if you have a memory bottleneck for example (very common), that will remain the limiting factor. Another common bottleneck is I/O; that is often improved by threads even on a single core processor.

If you are working inside a framework, like much of the code for LHCb, and that framework is already multithreaded, it is often slower to then try to introduce more threads; the other CPUs are already busy. Much of the work for these components is in making them **thread-safe**, that is, making them able to run in parallel with other algorithms or with themselves without conflicts for writing/reading memory.

## Multithreading basics in C++11
Multithreading is difficult for most languages to handle well, since we write source code in a linear fashion. There are several common methods for multithreading in C++11:

### Thread
With `std::thread`, you can run a function immediately in parallel, with perfect forwarding. Values are not returned; you will need to pass in pointers, etc. to get results.

```cpp
std::thread t1(my_function, my_argument1);
// Starts running along side code
t1.join(); // Finish the function and return.
```

## Future

You often want to start a function, go do something else, and then get the result of that function. Futures and promises allow you to do that. A promise is a value that you promise to give at some point, but does not necessarily have a value right now. A future is an object associated with that promise that lets you wait for the result and retrieve it when it is ready. Here is some pseudocode (see similar example [here](/DevelopKit/code/perf/future.cpp)):

```cpp
void some_slow_function(std::promise<int> p) {
    std::this_thread::sleep_for(5s);
    p.set_value(42);
}

std::promise<int> p;
std::future<int> f = p.get_future();

std::thread t(some_slow_function, std::move(p));
// Could do other things here
std::cout << "The result is " << f.get() << std::endl;
t.join();
```

## Async

This usage of futures can be made much easier in some cases using `std::async`. Notice that the last example had to have a special `some_slow_function` that worked with a promise. Often you will just want to wrap an existing function that slow, and returns a value. The previous example then [becomes](/DevelopKit/code/perf/async.cpp):

```cpp
int some_slow_function(int time) {
    std::this_thread::sleep_for(time * 1s);
    return 42;
}

std::future<int> f = std::async(std::launch::async, some_slow_function, 5);
// Could do other things here
std::cout << "The result is " << f.get() << std::endl;
```


## Data Races

Multithreading's biggest issue tends to be data races. Let's make a pseudocode example:

```cpp
int x = 0
void thread_1() {
    x += 1;
}
void thread_2() {
    x -= 1;
}
// Run thread_1 and thread_2 in parallel
```

What is the value of `x` after running? The expected answer is 0, since 1 is being added, and 1 is being subtracted. But when  you run this, you'll randomly get -1, 0, and 1 as an answer. This is because you have to read the value of x in (one operation) and then add 1 to it in the register (one operation) then return it to the `x` memory location (one operation). When the operations are running in parallel, you might load the value in the second thread before the write happens in the first
thread, giving you the unexpected results seen previously.

There are several ways to manage these values. Besides futures and promises, which can be used between threads, the following low level constructs are available.

### Mutexes

A mutex such as `std::mutex` allows a lock to be placed around segments of code that must run sequentially. For example, you may notice that `std::cout` tends to get mangled when multiple threads are printing to the screen. If you place a mutex around each output, the mangling will no longer be an issue. For example:

```cpp
std::mutex m; // Must be the same mutex in the different threads

m.lock(); // This line only completes when m is not locked already
std::cout << "A line" << " that is not interleaved as long as protected by m" << std::endl;
m.unlock();
```

If you forget to unlock the mutex, your code will stall forever when it comes on a new lock statement. A `std::lock_guard` will accept a mutex, locking it immediately, and then unlocking when the lock guard goes out of scope. If you have if statements or code that can throw exceptions, this might be easier than making sure `.unlock` is called every place it is needed.

Mutexes solve the issue of linearising a piece of code, but at a huge cost: they make that part of your code completely sequential and also have some small overhead too.

### Atomics

If you are worried about setting and accessing a single value, a faster and simpler method than mutexes are atomics. These often have hardware level support, allowing them to run faster than a matching mutex. These are special version of values that insure that reads and writes never collide. The only difference vs. a normal variable is the use of `.load()` to access the value (other operations are overloaded to behave as expected). The data race example becomes:

```cpp
std::atomic<int> x = 0
void thread_1() {
    x += 1;
}
void thread_2() {
    x -= 1;
}
// Run thread_1 and thread_2 in parallel
```

And the code always gives a result of `x.load() == 0`.

### Conditions

The final method that can be used to communicate between threads is condition variables. This behaves a bit like an atomic, but can be waited on until a thread "notifies" that the variable is ready.


> ## Future reading
> 
> * [Performance in C++ for LHCb](https://gitlab.cern.ch/mschille/fellow-meeting-performance-talk/blob/master/talk.pdf)
> * [LHCb Code Analysis Tools](https://twiki.cern.ch/twiki/bin/view/LHCb/CodeAnalysisTools)
> 
{: .callout}

