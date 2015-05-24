---
layout: page
title: First Steps in LHCb
subtitle: Pre-workshop Checklist
minutes: 10
---
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
> please contact us so we can update these instructions

Try the following steps with the computer you will use at the workshop:

1. Connect to lxplus with `ssh -X lxplus.cern.ch`.
2. Once connected, check your grid certificate works by typing
   `lhcb-proxy-init`. If you need help with your grid certificate
   there is the [Grid certificate FAQ](https://twiki.cern.ch/twiki/bin/view/LHCb/FAQ/Certificate)
   and you can ask questions on `lhcb-distributed-analysis@cern.ch`
3. Check that XForwarding works by running `xeyes` on lxplus. A set
   of eyes following your mouse should appear on your screen. Press
   `Ctrl-C` to exit.
4. Try and login to `lhcbstarterkit4.cern.ch` from an lxplus node. 
