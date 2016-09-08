---
title: "The Gaudi framework"
teaching: 60
exercises: 60
questions:
- "Why is Gaudi used?"
objectives:
- "Learn about the Gaudi framework."
keypoints:
- "Work with Gaudi."
---

## Goals of the design

Gaudi is being improved with the following goals in mind:

* Improve scaling by making the interface simpler and more uniform. This allows simpler code, thereby allowing more complex code to be written.
* Improve memory usage, to be cache frienly and avoid pointers to pointers. Use C++11 move
* Thread-safe code, with no state or local threading

In order to reach those goals, the following choices should be made when designing Gaudi algorithms:

* Proper const usage: Since C++11, const and mutable are defined in terms of data races (see [this video](https://channel9.msdn.com/posts/C-and-Beyond-2012-Herb-Sutter-You-dont-know-blank-and-blank))
* No global state: impossible to multithread
* Tools: should be private or public
* Ban `beginEvent`/`endEvent`, since there might be multiple events
* No explicit `new`/`delete` usage
* Remove `get/put<MyData>("")`

## Gaudi::Functional



## AnyDataHandle

In order to 
