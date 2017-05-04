---
layout: lesson
title: Second analysis steps in LHCb
---

These are the lessons for the second-stage workshop of the [Starterkit 
series][starterkit].
They build on those from [the first workshop][first-ana], teaching LHCb 
software that's more advanced and more focused on specific tasks.

Unlike the first workshop, there may be some lessons here that aren't 
applicable to everyone's analysis, but all the lessons should still provide a 
useful insight in to how things work under the hood.
It may also be that some lessons don't depend on any others; the prerequisites 
will be clearly stated at the beginning of each lesson.

If you have any problems or questions, you can [open an 
issue][second-ana-issues] on the [GitHub repository where these lessons are 
developed][second-ana-repo], or you can [send an email to 
`lhcb-starterkit@cern.ch`](mailto:lhcb-starterkit@cern.ch).

> ## Prerequisites {.prereq}
>
> Before starting, you should be familiar with the [first analysis 
> steps](https://lhcb.github.io/first-analysis-steps/) and satisfy all of its 
> prerequisites.
>

## Topics

1. [Using git to develop LHCb software](lb-git.html)
1. [Building your own decay. The Selection Framework](building-decays-part0.html)
1. [Building your own decay. A Historical Approach](building-decays-part1.html)
1. [Building your own decay. Modern Selection Framework](building-decays-part2.html)
1. [What to do when something fails](fixing-errors.html)
1. [Run a different stripping line on simulated data](rerun-stripping.html)
1. [Replace a mass hypothesis](switch-mass-hypo.html)
1. [Reuse particles from a decay tree](filter-in-trees.html)
1. [HLT intro](hlt-intro.html)
1. [TisTos DIY](tistos-diy.html)
1. [Ganga Scripting](ganga-scripting.html)
1. [Managing files in Ganga](managing-files-with-ganga.html)

## Other Resources

The [Starterkit site][starterkit] should be the go-to place for any related
material. If there's something here that's missing, wrong, or could be
improved, please [leave us an issue][second-ana-issues].

[starterkit]: https://lhcb.github.io/starterkit/
[first-ana]: https://lhcb.github.io/first-analysis-steps/
[second-ana-issues]: https://github.com/lhcb/second-analysis-steps/issues
[second-ana-repo]: https://github.com/lhcb/second-analysis-steps/issues
