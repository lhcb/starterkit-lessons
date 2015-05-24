---
layout: page
title: First Steps in LHCb
subtitle: Pre-workshop Checklist
minutes: 10
---
> ## Learning Objectives {.objectives}
>
> * Check you are ready for the workshop

Please read and try the following steps **before** arriving. For
some of the steps the solution is easy, but requires waiting for
a day or so. So please try them before arriving and try to fix
what ever does not work.

This will be an interactive workshop, so you will need to bring
a computer. There will be no machines for you to use in the room.

If this is the first time you are bringing your laptop to CERN, you
will have to [register it](https://network.cern.ch) before it can
access the internet.

Please bring an ethernet/network cable (even if you have wifi) and
your power supply.

> In the following we assume you use Mac OS or Linux, unfortunately
> we do not know how to do these things on Windows. If you do know,
> please contact us (lhcb-starterkit@cern.ch) so we can update these instructions.

Try the following steps with the computer you will use at the workshop:

1. In your browser, on the computer you are planning to bring to the
   workshop try and access the [web based book keeping](https://lhcb-portal-dirac.cern.ch/DIRAC/).
   If you need help with your grid certificate there is the
   [Grid certificate FAQ](https://twiki.cern.ch/twiki/bin/view/LHCb/FAQ/Certificate)
   and you can ask questions on `lhcb-distributed-analysis@cern.ch`.
1. Connect to lxplus with `ssh -X lxplus.cern.ch`.
2. Once connected, check your grid certificate works by typing
   `lhcb-proxy-init`. If you need help with your grid certificate
   there is the [Grid certificate FAQ](https://twiki.cern.ch/twiki/bin/view/LHCb/FAQ/Certificate)
   and you can ask questions on `lhcb-distributed-analysis@cern.ch`.
3. Check that XForwarding works by running `xeyes` on lxplus. A set
   of eyes following your mouse should appear on your screen. Press
   `Ctrl-C` to exit.
4. Try and login to `lhcbstarterkit4.cern.ch` from an lxplus node. If
   this does not work please email `lhcb-starterkit@cern.ch` with your lxplus
   username (Find out your lxplus username by running `whoami` on lxplus) and
   the error message you get.
4. On `lhcbstarterkit4.cern.ch` run:
   
     1. `ipython --version` which should print `3.0.0`
     2. `git --version` which should print `git version 2.4.1`
   
     If either of the above two commands do not work please email `lhcb-starterkit@cern.ch`

During the workshop we will provide you with a machine to connect to from your
laptop that has all the required software pre-installed and setup. If you can
successfully execute all of the above steps you are ready to go for the workshop.

If you know what you are doing and want to setup your lxplus environment so that
you can use it during the workshop, the following instructions are for you.

> These instructions are for people who know what they are doing. We encourage
> everyone to not try and install things themselves and use the machines we
> provide for them. We will have an install party at the end of the workshop
> to make sure you can keep using all the tools you learnt about once the
> workshop is over.

Only follow these instructions if you know what you are doing! We will not
provide you with any assitance until the last afternoon of the workshop.

1. Make sure you have `git` version 2.4.1
2. Install the [Anaconda](http://continuum.io/downloads) python distribution
   (or otherwise make sure you have `python` version 2.7, `ipython` version
   3.0.0, and know how to install more packages at a moments notice.)
