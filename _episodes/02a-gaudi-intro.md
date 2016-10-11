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

The Gaudi framework coordinates **Data Objects**, which can hold a variaty of data. A **Transiant Event Store (TES)** stores data objects in a way to make them accessable to the rest of the framework, and parts of it can be made persistant in a ROOT format file. The data in the TES is created and accessed by  **Algorithms**, which produce data objects and process data objects. The Application Manager manages these components.



## Algorithms

This is the most important component of the framework for an user to know. Algorithms are called once per physics event, and (traditionally) implement three methods beyond constructor/destructor: `initialize`, `execute`, and `finalize`. 




