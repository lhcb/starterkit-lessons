---
title: "Setup Simple"
teaching: 10
exercises: 10
questions:
- "Can you set up an incremental build?"
objectives:
- "Prepare a package and check out a portion of it."
keypoints:
- "Usage of lb-git commands."
---


> ## Prerequisites for building LHCb software
> 
> You should have a valid LHCb environment, using one of the ways previously mentioned, or by working on `lxplus`.
> You may need to run `source /cvmfs/lhcb.cern.ch/group_login.sh` to set up your environment variables.
{: .prereq}

## Git

LHCb uses the git versioning system.

### Prerequisites for git

Before using git, you should go to your [gitlab.cern.ch](https://gitlab.cern.ch) account and set up an ssh key. The procedure is the same as with github (and the public key is the same). In short, on Linux or Mac, make sure you have key pair in `~/.ssh/id_rsa.pub` and `~/.ssh/id_rsa`. If you don't have one, run

```bash
    ssh-keygen
```

and use the default location. If you don't put in a passphrase, you can directly push and pull without a password; if you do put a passphrase, you can run

```bash
    ssh-add
```

when you start a terminal to enter your passphrase once per session. Your computer keychain (Mac, Ubuntu, etc) can also store your password once per login.

> ## If you encounter problems
> 
> If you get an error about your ssh agent not running, you can execute the output of the ssh-agent command with `eval $(ssh-agent)` and try again.
{: .discussion}

Once you verify you have an ssh key pair, you need to go to the gitlab website, go to your profile (possibly hidden behind the hamburger button), then edit, then SSH Keys [(or just go here)](https://gitlab.cern.ch/profile/keys), and paste the contents of the `id_rsa.pub` file into the key edit box. You can give it a title that allows you to identify the computer or VM associated with the key.
{: .prereq}

### Defaults

You can set the lb-commands to default to ssh by running the following line to save a global (all repository) config variable:

```bash
git config --global lb-use.protocol ssh
```

You can also set the git default push policy for git if you have a version between 1.7.11 and 2.0:

```bash
git config --global push.default simple
```

This is not mandatory, but is a more reasonable default (and is the default in git 2.0. Note that SLC6 systems, like lxplus, have a version less than 1.7.11. On these systems, you can still set the default to `current`, which is better than the original default of `matching`.

> ## Aside: Explanation of defaults for the curious
>
> This aside is will only make sense if you know a little bit about git. If you are working on a branch `local_branch` and you push without explicitly telling git what to do, it will do the following:
> 
> * Upstream: Push to the upstream branch (must set an upstream branch), even if the name does not match the current branch.
> * Simple: If you have an upstream branch set, *and* it has the same name, it will push to that. Why is this a desired behavior? Because you may make a new branch but forget to change the upstream branch, so you could end up pushing your new branch to a differently named remote branch! (Default in git 2.0+)
> * Current: Push to a branch with the same name on the remote.
> * Matching: push all branches with the same names as remote branches to the remote. Great way to make a mess by pushing branches you did not intend. (Default in git<2.0).
{: .discussion}

> ## Coming from SVN?
> 
> If you have a background with SVN, you might want to look at the two part tutorial from IBM for subversion users, [Part 1](http://www.ibm.com/developerworks/linux/library/l-git-subversion-1/) and [Part 2](http://www.ibm.com/developerworks/linux/library/l-git-subversion-2/).
{: .discussion}

## LHCb simplifications

While vanilla git commands are covered in the [alternate lesson](02b-setupcomplete), this lesson uses special LHCb commands added to git. These are special programs that sit in the LHCb bin, and have names that start with `git-`. Git automatically converts a command like `git x` to `git-x`, so these seamlessly blend with the built in commands. To differentiate them, they all start with `lb-`.

### Setting up a satellite project

If you want to setup a satellite project (local project) for small changes, you'll need to prepare that project:

```bash
lb-dev Project vXrY
cd ProjectDev_vXrY
```

For example, the project might be `DaVinci`, and the version might be `v40r3p1`. Then, you'll been to tell git where it can checkout subprojects from:

```bash
git lb-use Project
```

This sets up a remote that points to the LHCb repository Project on GitLab.

To get the code from Project/Branch to checkout into Some/Package:

```bash
git lb-checkout Project/Branch Some/Package
```

The branch can be `master`, or a specific version `vXrY`, or any other valid branch. See the [GitLab](https://gitlab.cern.ch) page to browse projects and branches. 



> ## Future reading
>
> The twiki page [Git4LHCb](https://twiki.cern.ch/twiki/bin/view/LHCb/Git4LHCb) is currently the best source for git examples.
{: .callout}
