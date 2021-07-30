# Moore and Authoring a Trigger Selection

{% objectives "Learning Objectives" %}

* To author a trigger selection

{% endobjectives %}

Moore is a set of Python files used to configure Gaudi applications. These files are then passed to `gaudirun.py` which can use them for example to configure a trigger selection.

This is one of the few pieces of LHCb software which is incredibly well [documented][moore-doc] and the lesson taught in the ImpactKit follows a [tutorial][moore-hlt2] given in the Moore gitbook.

[moore-doc]: https://lhcbdoc.web.cern.ch/lhcbdoc/moore/master/index.html
[moore-hlt2]: https://lhcbdoc.web.cern.ch/lhcbdoc/moore/master/tutorials/hlt2_line.html