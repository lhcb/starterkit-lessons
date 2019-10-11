# Creating a grid proxy without LbScripts

On the grid VOMS (Virtual Organization Membership Service) is used to provide authentication when accessing grid resources.
This works by using your certificate to create a local proxy credential which can then be used by applications for a limited time.

LbScripts provides a useful command `lhcb-proxy-init` which is used to create a proxy.
It is sometimes useful however, to create a grid proxy when LbScripts is unavailable.
The most commonly use for this is to be able to access data at grid sites using XRootD.
Fortunately VOMS can be easily installed and used without requiring the full LHCb environment.

## Installation

The VOMS client can be installed in the following ways, if your operating system isn't listed see [here](https://github.com/italiangrid/voms-clients) (and consider [making a pull request](/CONTRIBUTING) to update this guide).

#### macOS with [homebrew](https://brew.sh)

```bash
brew cask install java
brew install voms
```

#### Ubuntu

```bash
sudo apt-get install voms-clients
```

#### CentOS

```bash
sudo yum install epel-release
sudo yum install voms-clients
```

#### Arch Linux

Install [voms from the AUR](https://aur.archlinux.org/packages/voms/).

## Getting the required files and certificates

In order to create a proxy you must have a copy of your personal grid certificate, assuming this is already set up on lxplus this can be copied using:

```bash
scp -r lxplus.cern.ch:.globus/ ~/.globus
```

Additionally, you will need a copy of the central LHCb grid certificates.
If you have cvmfs installed this is already available at `/cvmfs/lhcb.cern.ch/etc/grid-security/`.
If not, they can be copied from lxplus to `~/grid-security/` using:

```bash
scp -r lxplus.cern.ch:/cvmfs/lhcb.cern.ch/etc/grid-security/ ~/grid-security
```

Note these will need updating occasionally by rerunning the above commands.

## Creating a proxy

Once the voms have been installed and the files have been copied a grid certificate with a 168 hour (1 week) validity can be created using:

```bash
export X509_CERT_DIR="$HOME/grid-security/certificates"
export X509_VOMS_DIR="$HOME/grid-security/vomsdir"
voms-proxy-init -voms "lhcb:/lhcb/Role=user" -vomses "$HOME/grid-security/vomses" -r --valid 168:00 -cert "$HOME/.globus/usercert.pem" -key "$HOME/.globus/userkey.pem"
```

Alternatively, an alias can be created to allow proxies to be created using an `lhcb-proxy-init`.
This can be setup by appending the following to `~/.bashrc` and then opening a new terminal.

```bash
export X509_CERT_DIR="$HOME/grid-security/certificates"
export X509_VOMS_DIR="$HOME/grid-security/vomsdir"
alias lhcb-proxy-init='voms-proxy-init -voms "lhcb:/lhcb/Role=user" -vomses "$HOME/grid-security/vomses" -r --valid 168:00 -cert "$HOME/.globus/usercert.pem" -key "$HOME/.globus/userkey.pem"'
```

## Managing the proxy

The status of the current proxy can be checked using `voms-proxy-info`.

The current proxy can be deleted using `voms-proxy-destroy`.
