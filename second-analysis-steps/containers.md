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
 * Scientific linux 6 (SL6) - Retired in 2018

{% callout "Running simulation" %}

The simulation framework is a common place where older operating systems are needed these are:

```
 | Simulation framework | Required OS |
 | -------------------- | ----------- |
 | Sim10                | Centos7     |
 | Sim09                | SL6         |
```

{% endcallout %}

 
When running legacy code, it is not necessary to find a machine which runs the older OS nor is it necessary to make a virtual machine using the older OS. Instead we can use containers!
Containers are [virtualisation](https://en.wikipedia.org/wiki/OS-level_virtualization) software which will allow for multiple operating systems to be run on the same machine, so in this case a machine running RHEL9 can instead use another OS for a while as if it were just another environment.

The software used on lxplus to jump from one OS to another is called `apptainer`. To be able to run centos7 software on a RHEL9 machine the following command can be used and adapted:

```bash
apptainer exec -B /eos -B /afs/cern.ch/work -B /cvmfs -e /cvmfs/lhcb.cern.ch/containers/os-base/centos7-devel/prod/amd64 bash --rcfile /cvmfs/lhcb.cern.ch/lib/group_login.sh
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
```

{% challenge "Checking your OS" %}

At any point the OS you are using can be checked with the command `cat /etc/os-release`

 * Try it now both in a RHEL9 environment and a singularity running Centos7 to see the operating system change!

{% endchallenge %} 
