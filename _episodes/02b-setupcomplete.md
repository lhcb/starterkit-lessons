---
title: "Setup Complete"
teaching: 20
exercises: 60
questions:
- "Can you run LHCb software from scratch?"
objectives:
- "Set up a development environment."
keypoints:
- "Usage of raw git commands."
---

# Git for LHCb

You can set up a development environment with complete LHCb packages, and build them from scratch. This allows you to make large scale changes without locating sub-packages online, and is similar to the standard procedures in other projects. The downside to this is that they take a while to compile, due to the size of LHCb projects. Since the following is very similar to standard git procedures, most online git tutorials are also helpful in understanding the meaning of the various
git command used.

Before using git, you should go to your gitlab.cern.ch account and set up an ssh key. The procedure is the same as with github (and the public key is the same). In short, on Linux or Mac, make sure you have key pair in `~/.ssh/id_rsa.pub` and `~/.ssh/id_rsa`. If you don't have one, run

```bash
    ssh-keygen
```

and use the default location. If you don't put in a passphrase, you can directly push and pull without a password; if you do put a passphrase, you can run

```bash
    ssh-add
```

when you start a terminal to enter your passphrase once per session. Your computer keychain (Mac, Ubuntu, etc) can also store your password once per login.

> If you get an error about your ssh agent not running, you can execute the output of the ssh-agent command with `eval $(ssh-agent)` and try again.
{: .note}

Once you verify you have an ssh key pair, you need to go to the gitlab website, go to your profile (possibly hidden behind the hamburger button), then edit, then SSH Keys [(or just go here)](https://gitlab.cern.ch/profile/keys), and paste the contents of the `id_rsa.pub` file into the key edit box. You can give it a title that allows you to identify the computer or VM associated with the key.

To get a complete package for development, the following command will work:

```bash
    git clone 
```

> The twiki page [Git4LHCb](https://twiki.cern.ch/twiki/bin/view/LHCb/Git4LHCb) is currently the best source for git examples.



