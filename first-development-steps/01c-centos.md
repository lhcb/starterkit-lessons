# Setting up a local LHCb environment - CentOS 7
{% objectives %}
* Get and configure the LHCb stack.
{% endobjectives %}

On CentOS 7, the following steps allow you to build the LHCb software:

First, mount [CVMFS](https://cernvm.cern.ch/portal/filesystem/downloads). It is obtainable through yum with:

```
sudo yum install https://ecsft.cern.ch/dist/cvmfs/cvmfs-release/cvmfs-release-latest.noarch.rpm
sudo yum install cvmfs cvmfs-config-default
sudo cvmfs_config setup
```

Open the file `/etc/cvmfs/default.local` and add the lines:

```
CVMFS_REPOSITORIES=lhcb.cern.ch,lhcbdev.cern.ch
CVMFS_HTTP_PROXY=DIRECT
CVMFS_QUOTA_LIMIT=25000
```

Verify these are available with `cvmfs_config probe`. If not, restart with `sudo service autofs restart`.

You can run on CentOS 7 using:

```
source /cvmfs/lhcb.cern.ch/group_login.sh -c x86_64-centos7-gcc62-opt
```

