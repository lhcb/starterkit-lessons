


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
track_tool = dtt.addTupleTool('TupleToolTrackInfo')
```

*Branches* let you define custom namespaces in your ntuple by means of a `dict`.
Its keys define the name of each branch (and, as a consequence, the prefix of the corresponding leaves in the ntuple), while the corresponding values are decay descriptors that specify which particles you want to include in the branch.


{% callout "Do I really have to type my decay descriptor that many times?" %}
No!
{% endcallout %} 

in our ntuple:

- `DecayTreeTuple` for the more general tools.


{% challenge "Test your ntuple" %}
Run the options in the same way as in the [minimal DaVinci 
job](minimal-dv-job.html) lesson.
You will obtain a `DVntuple.root` file, which we can open and inspect with `ROOT`'s `TBrowser`:

```
$ root DVntuple.root
root [0]
Attaching file DVntuple.root as _file0...
root [1] TBrowser b
root [2]
```

Try to locate the branches we have added, which are placed in the `TupleDstToD0pi_D0ToKpi/DecayTree`, and plot some distributions by double-clicking the leaves.
{% endchallenge %} 


To add LoKi-based leaves to the tree, we need to use the `LoKi::Hybrid::TupleTool`, which is configured with 3 arguments:

  1. Its *name*, specified in the `addTupleTool` call after a `/`.  This is 
     very useful (and recommended) if we want to have different 

