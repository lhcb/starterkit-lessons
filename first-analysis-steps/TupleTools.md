


this is where the tuple tool is gunna go!


# TupleTools and branches

{% objectives "Learning Objectives" %}
* Lauren do something
{% endobjectives %} 

{% callout "Default DecayTreeTuple tools" %}
The default tools added in `DecayTreeTuple` are:

 - `TupleToolEventInfo`, which stores general information (event number, run number, GPS time, etc) of the event.
{% endcallout %} 

```python
Write code here
```

*Branches* let you define custom namespaces in your ntuple by means of a `dict`.
Its keys define the name of each branch (and, as a consequence, the prefix of the corresponding leaves in the ntuple), while the corresponding values are decay descriptors that specify which particles you want to include in the branch.


{% callout "Do I really have to type my decay descriptor that many times?" %}
No!
{% endcallout %} 

in our ntuple:

- Its a list!

{% challenge "Challenging" %}
Run the options in the same way as in the [minimal DaVinci 
job](minimal-dv-job.html) lesson.


```
$ root DVntuple.root

```

{% endchallenge %} 


Numbered list coming up:

  1. Its *name*, specified in the `addTupleTool` call after a `/`.  This is 
     very useful (and recommended) if we want to have different 

