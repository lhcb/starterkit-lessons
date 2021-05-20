# Advanced Dirac

At this point we have used Ganga for a number of things related to
Davinci and basic file management. However, Ganga is a much more
flexible tool that. Using `Python` and `IPython` we can set up more
complicated workflows that manage much of your analysis for you!

{% objectives " Learning Objectives" %}

* Learn core concepts of the GangaTasks package
* Understand advanced ganga features for job management
* Apply these to real analysis issues and define a simple workflow

{% endobjectives %}

{% callout "Pre-Requisites" %} 

This later part of this tutorial will be based off the 
D^{* +} \to  D^{0}(\to K^{-}K^{+})\pi^{+} MC options file so make 
sure you have it to hand!

{% endcallout %}

The first and most important package to introduce is GangaTasks. This 
package is designed to help busy analysts from spending more time 
managing GRID jobs than working on physics. It has the following
core features.

* Automatically submit jobs and keep a certain ammount running at all
times.
* Automatically create new jobs based on previous jobs in a chain.
* Automatically resubmit jobs up to a threshold number of resubmits!

So as you can imagine, tasks is a powerful tool...time to play with it
!
