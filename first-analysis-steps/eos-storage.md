# Storing large files on EOS

{% objectives "Learning Objectives" %}

* Open and view files on EOS

{% endobjectives %} 

EOS is a large area of disk space provided by CERN for users. See [here](http://information-technology.web.cern.ch/services/eos-service) for more information.

On `lxplus` EOS is mounted with a fuse mount under `/eos` .
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

You may also use the following path which is independent of the EOS mount:

```bash
$ root root://eoslhcb.cern.ch//eos/lhcb/user/a/another/myfavouritefile.root
```

In general the fuse mount of EOS is not very reliable and it is not recommended to use it for moving or accessing large numbers of files. Instead you can use the [eos commands](http://eos-docs.web.cern.ch/eos-docs/clicommands.html), for example `eos cp`, or the [xrootd protocol](https://xrootd.slac.stanford.edu/doc/xrdcl-docs/www/xrdcldocs.html#x1-40003), for example `xrdcp`.
