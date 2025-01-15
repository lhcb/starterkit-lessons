# Containers and running legacy code

{% objectives "Learning Objectives" %}

* Understand how to use containers to run code requiring older operating systems

{% endobjectives %} 

When running code at LHCb there are several scenarios where a user may have to run legacy code intended for an older OS.
The most recent operating systems used is Redhat Enterprise Linux 9 (RHEL9).
There are however, different alternatives to this OS which are roughly equivalent e.g. AlmaLinux 9, Rocky Linux 9.

There are a few situations where you may need to run legacy code e.g.

* Running code from an older analysis
* Often times the code used by the Simulation working group requires older operating systems as well

The previously-used operating systems used by lxplus are listed below.
If not RHEL9, the OS you need will almost certainly be one of thse two:

 * Centos7 - Retired in 2023
 * Scientific Linux CERN 6 (SLC6) - Retired in 2018

{% callout "Running simulation" %}

The simulation framework is a common place where older operating systems are needed these are:

```
 | Simulation framework | Required OS |
 | -------------------- | ----------- |
 | Sim10                | Centos7     |
 | Sim09                | SLC6         |
```

{% endcallout %}

 
When running legacy code, it is not necessary to find a machine which runs the older OS nor is it necessary to make a virtual machine using the older OS. Instead we can use containers!
Containers are [virtualisation](https://en.wikipedia.org/wiki/OS-level_virtualization) software which will allow for multiple operating systems to be run on the same machine, so in this case a machine running RHEL9 can instead use another OS for a while as if it were just another environment.

The software used on lxplus to jump from one OS to another is called `apptainer`. To be able to run centos7 software on a RHEL9 machine the following command can be used and adapted:

```bash
apptainer exec --env LBENV_SOURCED= -B /afs/cern.ch/user -B /afs/cern.ch/work -B /cvmfs -e /cvmfs/lhcb.cern.ch/containers/os-base/centos7-devel/prod/amd64 bash --rcfile /cvmfs/lhcb.cern.ch/lib/LbEnv
```

We can break down this command in the following way:

```
 | Command                                | Explanation                                                     |
 | -------------------------------------- | --------------------------------------------------------------- |
 | Apptainer                              | The command being run.                                          |
 | exec -e <the centos7 environment> bash | Run a bash session with centos7.                                |
 | -B <directory>                         | These are 'binds', any directory (and its subdirectories) which |
 |                                        | should be visible within the container need to be specified.    |
 | --rcfile <group_login>                 | Specifies that LbEnv should be sourced.                         |
 | --env LBENV_SOURCED=                   | Resets the LBENV_SOURCED environment variable to reload LbEnv   |
```

The full list of available platforms is listed here[^1]:

```
| Platform | Location                                                        |
| -------- | --------------------------------------------------------------- |
| slc5     | /cvmfs/lhcb.cern.ch/containers/os-base/slc6-devel/prod/amd64    |
| slc6     | /cvmfs/lhcb.cern.ch/containers/os-base/slc6-devel/prod/amd64    |
| centos7  | /cvmfs/lhcb.cern.ch/containers/os-base/centos7-devel/prod/amd64 |
| el9      | /cvmfs/lhcb.cern.ch/containers/os-base/alma9-devel/prod/amd64   |
```

{% challenge "Checking your OS" %}

At any point the OS you are using can be checked with the command `cat /etc/system-release`

 * Try it now both in a RHEL9 environment and a singularity running Centos7 to see the operating system change!

{% endchallenge %} 

{% callout "Warning: minimal use and stability" %}

One thing to bear in mind about using containers is that they will be less stable than the default OS on your machine.
For this reason it is recommended to only use the containers to run code requiring that OS and do everything else (e.g. git stuff) outside of the container.
This is especially true for containers of older systems such as SLC5 and SLC6.

{% endcallout %}

[^1]: It's also possible to get an el9 (i.e. RHEL9) platform. This allows a system running a different linux distro locally (e.g. Ubuntu, Fedora, Arch, ...) to run software compiled for EL9!
