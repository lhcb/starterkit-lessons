---
title: "New features in the distant future for LHCb Physists"
teaching: 15
exercises: 0
questions:
- "What can I use from the improvements C++ in 2020+?"
objectives:
- "Learn a few of the plans for C++"
keypoints:
- "Learn about C++ plans"
---

C++ is on a 3 year cycle, with groups working on Technical Specifications (TS's). Many TS's were proposed for C++17, but failed to make the cut. They may make it into a later version, however, and may need support from users like you. So here are the most interesting plans:

## Parallelism II

The parallel TS that was accepted has an extension that was not; it it planned for the future. This add a few things, like auto-deduction of the best execution policy.

## Concepts

This TS was sorely missed; even though it doesn't directly affect code, it does something even more important: It improves error messages and clarifies templated code structure. Although the syntax is a bit dense (it's C++, after all), what it does is simple. If you write a templated function, you probably have some concept of what the parameters should be. This allows you to specify that upfront. You can say something like "I expect the type to be a number" or "I expect to the type to
have a .Save() and .Load() methods", and the error message if this fails will occur in the right portion of the code, and will clearly define the problem. As a library user, you will only notice that error messages improve; as a library author, you will need to think about what template expect and add concepts to them in order to benefit.

## Modules

C++'s biggest remaining problem is the horrendous, hack of a system from the 1970's called "include". It only knows how to copy and paste code when compiling, causing huge, ugly, long compiles. A real module system would make combining code and compiling cleaner and faster (by a lot). This was not quite ready for 2017, but was close.

## Ranges

Ranges uses concepts so heavily that it was not going to go in without it. It can define a range (like 10-20) of an object (like a vector) without wasting memory. It also would allow the standard library to work on ranges, which would be defined for things like `std::vector`, allowing the following notation:

```cpp
std::for_each(vect.begin(), vect.end(), do_something);
```

to change to:

```cpp
std::for_each(vect, do_something);
```

Of course, the point would be to do something like this:

```cpp
std::for_each(vect[{2,5}], do_something);
```
which would take the second up to the fifth elements of vect and only operate on those. I'm assuming vectors will support view's iterating and slicing here, otherwise you might need to wrap a vector in a view from Ranges.

## Reflection

This is not really even a TS yet, just a Study Group (SG). However, it is a huge topic, and it being pushed forward by the ROOT team among others. It would allow a C++ class to know what it is, just like a Python class does, and would eliminate/fix/simplify most of what ROOT does, like the saving of arbitrary classes. ROOT's dictionary generator is just a hack for the missing Reflection feature. Read more (here)[https://root.cern.ch/blog/status-reflection-c]. Like
several of the newest proposals, it uses Concepts.

## Smaller changes

The `operator.` proposal for C++17  needed a last minute change, and may or may not make it into C++17 as a special case. It allows smart references just like we currently have smart pointers.

There has been some work on unified call syntax, which would allow `x.something(y)` to fall back to `something(x,y)` and vice versa.

Some experimental libraries that may make it in eventually are Transactional Memory, Concurrency, Networking, Coroutines, Graphics, and Numerics.

