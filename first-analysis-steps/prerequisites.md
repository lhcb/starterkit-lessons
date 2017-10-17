# Pre-workshop checklist

{% objectives "Learning Objectives" %}
* You will be ready for the workshop!
{% endobjectives %}

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
> your desktop or someone else's computer.

If this is the first time you are bringing your laptop to CERN, you
will have to [register it](https://network.cern.ch) before it can
access the internet. Both the WiFi and the ethernet connection need to be
registered.

Please bring an ethernet/network cable (even if you have WiFi) and
your power supply, as well as a plug adaptor to Swiss and European plugs.

{% callout "Windows" %}
In the following we assume you use Mac OS X or Linux. If you are running
Windows, step 2 is replaced by a list of instructions given at the bottom
of this page.
{% endcallout %}

Try the following steps with the computer you will use at the workshop:

 1. In your browser try and access the [web-based book keeping](https://lhcb-portal-dirac.cern.ch/DIRAC/).
    If you need help with your grid certificate there is the
    [Grid certificate FAQ](https://twiki.cern.ch/twiki/bin/view/LHCb/FAQ/Certificate)
    and you can ask questions on `lhcb-distributed-analysis@cern.ch`.
 2. From a terminal (`xterm` on Linux or `Terminal` on Mac OS X) connect to
    lxplus with `ssh -X lxplus.cern.ch`.
    If your local username is different from your `lxplus` one use `ssh -X mylxplusname@lxplus.cern.ch`.
    Please try exactly this command even if you usually use an alias or other shortcut.
    >If, just below the `Password:` line, you get a message `Warning: untrusted X11 forwarding setup failed: xauth key data not generated`:
    >* Logout (using `logout` or `Ctrl-d`)
    >* Login using [`-Y`](https://man.openbsd.org/ssh#Y) instead of [`-X`](https://man.openbsd.org/ssh#X)
    >* This will switch to trusted X11 forwarding and you may see a message like `Warning: No xauth data; using fake authentication data for X11 forwarding.`

 3. Once connected, check your grid certificate works by typing
    `lhcb-proxy-init`. If you need help with your grid certificate there is the
    [Grid certificate FAQ](https://twiki.cern.ch/twiki/bin/view/LHCb/FAQ/Certificate)
    and you can ask questions on `project-lcg-vo-lhcb-admin@cern.ch`.
    `lhcb-proxy-init` will ask you for the password of your grid certificate and then print something like:

  ```
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
  ```

 4. Check that X11 forwarding works by typing `xeyes` on lxplus. A set
    of eyes following your mouse should appear on your screen. Press
    `Ctrl-C` from the terminal to exit.
    >If you're not connected to the CERN network at CERN, do not worry if the X11 forwarding is slow--this is normal.

If you can successfully execute all of the above steps, you are ready to go for
the workshop!

{% callout "Using Bash" %}
The [Bash shell](http://en.wikipedia.org/wiki/Bash_(Unix_shell%29) will be used
throughout the workshop.
The default for new LHCb computing accounts is now Bash. If you have an older
account, the default used to be a shell called `tcsh`
(“tee-cee-shell”), which has subtly different ways of doing things
in comparison with Bash.

It is recommended to change your default shell to Bash if this is the case, which is much more
widely used than `tcsh` and also supported by LHCb, by visiting the your
[CERN account page](https://account.cern.ch), then clicking “Resources and
services”, then “List services”, “LXPLUS and Linux”, “Settings”, then change
“Unix shell” to `/bin/bash`, and click “Save Selection”.

If you don't want to change your default shell, just execute the `bash`
command when you login to lxplus.
{% endcallout %}


{% callout "Windows-specific Instructions" %}
On Windows, some additional steps are required before you can connect via SSH.

Set up steps (you only have to perform this once):

1. Download the [Xming installer](http://sourceforge.net/projects/xming/files/latest/download).
2. Run the installer.
3. Download [PuTTY](http://the.earth.li/~sgtatham/putty/latest/x86/putty.exe).

The following steps have to be executed each time you want to connect:

1. Start PuTTY.
2. In the list on the left, unfold `Connection` and `SSH`, then click the `X11` item.
3. In the window that appears, make sure the check box labeled `Enable X11 forwarding` is checked.
4. Return to the previous window by selecting `Session` int he list on the left.
5. In the text box labeled `Host Name (or IP address)`, type `lxplus.cern.ch`.
6. Make sure the `Port` text box contains the number `22`.
7. Click the `Open` button on the bottom of the screen.
8. A window appears with the text `login as:`. Type your CERN username, followed by Enter.
9. The window should say `Using keyboard-interactive authentication. Password:`. Type your password, again followed by Enter.
10. You now have a remote SSH session at an lxplus server node!
{% endcallout %}
