---
title: "Setting up a local LHCb environment - CernVM"
teaching: 30
exercises: 90
questions:
- "Do you want a portable, local install?"
objectives:
- "Get and configure CernVM."
keypoints:
- "Use CernVM Online."
- "Use a VM."
---

# Setting up a CernVM

To get a virtual machine setup, follow the instructions at the [CernVM website](https://cernvm-online.cern.ch). Since there are quite a few choices listed, here is a recommended method for setting up a CernVM.

In order to use a virtual machine, you will need to create a context. Select LHCb as your main group, and make sure you pay attention to your user settings; that's how you will log in later. Under CernVM preferences, you'll want to look at the CernVM Edition variable, and set it to desktop if you want a UI.

Get VirtualBox in order to run the VM. Download CernVM and follow the [manual installation instructions](http://cernvm.cern.ch/portal/vbinstallation) to get a local install. Spawning from online seems to have persistency issues, and the OVA image seems to have some hardware options baked in that might not match your computer.

Next you will need to pair your instance with your context. You'll get a context code that you'll enter as your "username" when booting up your instance. Make sure you enter a number sign preceding the pairing number, or it will try to log into a user with your number instead. After it pairs, you can use the username and password that you previously specified.

# Using a CernVM

You will need to run the setup script `source /cvmfs/lhcb.cern.ch/group_login.sh` to prepare an LHCb environment.
