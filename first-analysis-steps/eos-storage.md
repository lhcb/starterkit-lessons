# Storing large files on EOS

{% objectives "Learning Objectives" %}

* Open and view files on EOS

{% endobjectives %} 

On `lxplus` EOS is mount under `/eos` .
If you list the contents of the `/eos` directory you should see various experiments areas, and the user area:

```
lhcb ship user
```

There are two useful workspaces on EOS:
- `/eos/user` which is your private EOS space, also accessible via [CERNBox](https://cernbox.cern.ch/), and has 1TB of space;
- `/eos/lhcb` which is the LHCb area and contains the following sub-directories:
  - `/eos/lhcb/user` which is your user area, where you can store your tuples, and has 2TB of space;
  - `/eos/lhcb/wg` which contains the Working Group areas (e.g. for preserving tuples for completed analyses);
  - `/eos/lhcb/grid/` which is the path to the CERN grid site; and many others.

Let's inspect your user area:

```bash
$ ls -al /eos/lhcb/user/a/another
```

Files stored on EOS can be open in `ROOT` the usual way:

```bash
$ root /eos/lhcb/user/a/another/myfavouritefile.root
```

You may also use the following path which is independent of the EOS mount (the mount is in general not very reliable):

```bash
$ root root://eoslhcb.cern.ch//eos/lhcb/user/a/another/myfavouritefile.root
```
