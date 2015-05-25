---
layout: page
title: First Steps in LHCb
subtitle: Pre-workshop Checklist
minutes: 10
---
> ## Learning Objectives {.objectives}
>
> * You will be ready for the workshop!

Please read and try the following steps **before** arriving. For
some of the steps the solution requires waiting for a day or so.
So please try them before arriving and try to fix what ever does
not work.

> Follow this guide before arriving, we will not have time to help
> you with problems on these issues during the workshop. This means
> you will end up watching instead of participating.

This will be an interactive workshop, so you will need to bring
a computer. There will be no machines for you to use in the room.

> Follow all the steps using the computer you plan to bring, not
> your dekstop or someone elses computer.

If this is the first time you are bringing your laptop to CERN, you
will have to [register it](https://network.cern.ch) before it can
access the internet.

Please bring an ethernet/network cable (even if you have wifi) and
your power supply, as well as a plug adaptor to swiss and european plugs.

> In the following we assume you use Mac OS or Linux, unfortunately
> we do not know how to do these things on Windows. If you do know,
> please contact us (lhcb-starterkit@cern.ch) so we can update these instructions.

Try the following steps with the computer you will use at the workshop:

 1. In your browser try and access the [web based book keeping](https://lhcb-portal-dirac.cern.ch/DIRAC/).
    If you need help with your grid certificate there is the
    [Grid certificate FAQ](https://twiki.cern.ch/twiki/bin/view/LHCb/FAQ/Certificate)
    and you can ask questions on `lhcb-distributed-analysis@cern.ch`.
 2. From a terminal (`xterm` on linux or `Terminal` on Mac) connect to lxplus with `ssh -X lxplus.cern.ch`.
    If your local username is different from your `lxplus` one use `ssh -X mylxplusname@lxplus.cern.ch`.
    Please try exactly this command even if you usually use an alias or other shortcut.
 3. Once connected, check your grid certificate works by typing
    `lhcb-proxy-init`. If you need help with your grid certificate there is the
    [Grid certificate FAQ](https://twiki.cern.ch/twiki/bin/view/LHCb/FAQ/Certificate)
    and you can ask questions on `lhcb-distributed-analysis@cern.ch`.
    `lhcb-proxy-init` will ask you for the password of your grid certificate and then print something like:

~~~ {.output}
Generating proxy...
Enter Certificate password:
Added VOMS attribute /lhcb/Role=user
Uploading proxy for lhcb_user...
Uploading proxy for private_pilot...
Proxy generated:
subject      : /DC=ch/DC=cern/OU=Organic Units/OU=Users/CN=thead/CN=667505/CN=Timothy Daniel Head/CN=proxy/CN=proxy
issuer       : /DC=ch/DC=cern/OU=Organic Units/OU=Users/CN=thead/CN=667505/CN=Timothy Daniel Head/CN=proxy
identity     : /DC=ch/DC=cern/OU=Organic Units/OU=Users/CN=thead/CN=667505/CN=Timothy Daniel Head
timeleft     : 23:53:59
DIRAC group  : lhcb_user
path         : /tmp/x509up_u25636
username     : thead
properties   : NormalUser
VOMS         : True
VOMS fqan    : ['/lhcb/Role=user']

Proxies uploaded:
 DN                                                                                 | Group         | Until (GMT)
 /DC=ch/DC=cern/OU=Organic Units/OU=Users/CN=thead/CN=667505/CN=Timothy Daniel Head | lhcb_user     | 2015/08/25 08:05
 /DC=ch/DC=cern/OU=Organic Units/OU=Users/CN=thead/CN=667505/CN=Timothy Daniel Head | private_pilot | 2015/08/25 08:05
~~~

 4. Check that XForwarding works by typing `xeyes` on lxplus. A set
    of eyes following your mouse should appear on your screen. Press
    `Ctrl-C` to exit.
 5. From `lxplus` connect to `lhcbstarterkit4` with `ssh lhcbstarterkit4`. If
    this does not work please email `lhcb-starterkit@cern.ch` with your lxplus
    username (Find out your lxplus username by running `whoami` on lxplus) and
    the error message you get.
 6. On `lhcbstarterkit4.cern.ch` run:

      1. `ipython --version` which should print `3.0.0`
      2. `git --version` which should print `git version 2.4.1`

      If either of the above two commands do not work please email `lhcb-starterkit@cern.ch`

If you can successfully execute all of the above steps you are ready to go for the workshop.
