---
layout: page
title: First steps in LHCb
subtitle: LoKi functors. What are they and how can we use them
minutes: 15
---

> ## Learning Objectives {.objectives}
>
> * Understand what LoKi functors are
> * Learn how to use them in a DecayTreeTuple
> * Be able to find functors that do what we want

A LoKi functor is a class that takes an object of type `TYPE1` and returns another of `TYPE2`.
They can be used both in C++ and in python code, and can be combined with each other using logical operations.

According to `TYPE2` there are 3 types of functors:
 
 - *Functions*, which return `double`.
 - *Predicates*, which return a `bool`.
 - *Streamers*, which return a `std::vector` of some other type `TYPE3`.

When filling tuples, the most used functors are functions, while predicates are typically used for selections.

According to `TYPE1`, there are many types of functors, the most important of which are (you can find a full list in the [LoKi FAQ](https://twiki.cern.ch/twiki/bin/view/LHCb/FAQ/LoKiFAQ#How_to_code_own_LoKi_functor)):

 - *Particle functors*, which take `LHCb::Particle*` as input. 
 - *Vertex functors*, which take `LHCb::VertexBase*` as input.
 - *MC particle functors*, which take `LHCb::MCParticle*` as input. 
 - *MC vertex functors*, which take `LHCb::MCVertex*` as input.
 - *Array particle functors*, which take a `LoKi::Range_` (an array of particles) as input.
 - *Track functors*, which take `LHCb::Track` as input.


