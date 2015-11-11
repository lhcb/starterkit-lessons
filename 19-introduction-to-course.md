---
layout: page
title: First Steps in LHCb
subtitle: Goals of the course
minutes: 10
---
> ## Learning Objectives {.objectives}
>
> * Understand what we'll be doing, and why we'll be doing it.

The first LHCb Starterkit is all about getting you the data you need to do your 
physics analysis.

We'll start from the very beginning, explaining how proton-proton collisions 
make their way to long-term storage, and will end with having a ROOT ntuple 
containing all the variables you might want to look at for making your 
measurement.

We want this course to give you the confidence to be able to start by yourself, 
to understand what the code you'll be writing does and why you're writing it, 
and to teach others how to do the same.
Each lesson follows on from the previous one, but each can also serve as a 
standalone reference when you need to revisit a particular topic.

We'll be looking at:

* How data flows through the LHCb processing chain, what software is involved, 
  and how the data are stored;
* How the data flow is different for simulated (Monte Carlo) events;
* Where the data ends up, how it's indexed, and how you can find and access the 
  data you need;
* How to get candidate decays from the LHCb data format to ROOT ntuples; and
* How to add more variables to the ntuples.

We'll also cover how to efficiently run software locally and on the Grid using 
Ganga, as well as how to ask good questions when you're stuck and where to ask 
them.

The lessons will start with a lot of explaining, but then we'll get in to the
hands-on stuff.

These lessons have been put together by a group of people who are passionate 
about teaching good software practices and demystifying code.
Over the course of 2015, these lessons have been written on 
[GitHub](https://github.com), a code sharing and collaboration website.
You can find the source code of these lessons in the 
[`lhcb/first-analysis-steps`](https://github.com/lhcb/first-analysis-steps) 
repository, and _you_ can contribute!
Please [submit an issue](https://github.com/lhcb/first-analysis-steps/issues) 
if you spot a mistake or you think something isn't clear enough, or you can 
[make the changes 
yourself](https://github.com/lhcb/first-analysis-steps/blob/master/CONTRIBUTING.md) 
and open a pull request.
If you're not already familiar with git, you could check out our [analysis 
essentials](http://lhcb.github.io/analysis-essentials/) course.

So, enough with the introduction, let's dive in!
