# Developing LHCb Software

{% objectives "Learning Objectives" %}
* Learn how to work with and modify LHCb software projects and packages
* Learn how to find and search the source code and its documentation
{% endobjectives %} 

{% prereq "Prerequisites" %}
Before starting, you should have a basic understanding of how to use `git`,
similar to what has been
[taught](https://hsf-training.github.io/analysis-essentials/git) during the
Starterkit.
{% endprereq %} 

In this lesson, we'll show you a complete workflow for developing the LHCb
software using the `git` version control system.  At LHCb, we use GitLab to
manage our git repositories.  Among other features, GitLab allows you to browse
the source code for each project, and to review new changes (called *merge
requests*) in a convenient web interface.  You can find the CERN GitLab
instance at https://gitlab.cern.ch.

In principle, there are multiple ways of interacting with the LHCb software
repositories:

 1. A vanilla git workflow using only the standard git commands.  This requires
 you to `clone` and compile an entire LHCb project at a time.
 1. An LHCb-specific workflow using a set of `lb-*` subcommands.  This allows
 you to check out individual packages inside a project, and streamlines the
 modification of a few packages at a time.  (This is closer to the previously
 used `getpack` command.)

Here, we want to focus on the second workflow. The first workflow will be
discussed briefly at the [bottom](#working-with-a-full-project-checkout) of
this page. Note that, although the lb-git commands are much easier for small
changes to existing packages where recompiling an entire project would be
cumbersome, for any serious development the usage of vanilla git is much more
stable. Please consider using it if you can spare the compilation time.

{% callout "Initial setup" %}
Before jumping in by creating a project in GitLab, you should make sure that
your local git configuration and your settings on GitLab are sufficiently set
up.

 - Your name and email address should be set up in your local `git`
configuration.  To ensure that this is the case, run
```
git config --global user.name "Your Name"
git config --global user.email "Your Name <your.name@cern.ch>"
```
and put in your information.

 - Next, connect to https://gitlab.cern.ch and log in with your CERN 
   credentials. Visit https://gitlab.cern.ch/profile/keys and add an SSH key.

 - Finally, run this LHCb-specific configuration command:
   ```
   git config --global lb-use.protocol ssh
   ```
   This makes sure the LHCb commands use the ssh protocol instead of https.
{% endcallout %} 

This lesson introduces you the commands:

 - `lb-dev` for setting up a new development environment
 - `git lb-use` and `git lb-checkout` for downloading LHCb software packages

If you want to make changes to a software package, you will need to set up a
development environment. `lb-dev` is your friend here:

```bash
lb-dev --name DaVinciDev DaVinci/v45r1
```

The output should look similar to this:

```
Successfully created the local project DaVinciDev in .

To start working:

  > cd ./DaVinciDev
  > git lb-use DaVinci
  > git lb-checkout DaVinci/vXrY MyPackage

then

  > make
  > make test

and optionally (CMake only)

  > make install

You can customize the configuration by editing the files 'build.conf' and
'CMakeLists.txt' (see http://cern.ch/gaudi/CMake for details).
```

{% callout "lb-dev created local projects are Git repositories" %}
When `lb-dev` creates the local project directory and create the initial
files there, it also calls `git init` and commits to the local Git repository
the first version of the files (try with `git log` in there).

You can then use git to keep track of your development, and share your code
with others (for example with a [new project in
gitlab.cern.ch](https://gitlab.cern.ch/projects/new)).
{% endcallout %} 

Follow those instructions to compile the software:

```bash
cd DaVinciDev
git lb-use DaVinci
make
```

Once that's done, you can do

```bash
./run bash -l
```

inside the directory. This will (similar to `lb-run`) give you a new bash
session with the right environment variables set, from which you can run
project-specific commands such as `gaudirun.py`.

Your new development environment won't be very useful without any software to
modify and build.  So let's check out one of the existing LHCb packages! These
are stored in the [LHCb Git repositories](https://gitlab.cern.ch/lhcb).

In order to obtain the source code of the package you want to work on, we'll
use the [Git4LHCb](https://twiki.cern.ch/twiki/bin/view/LHCb/Git4LHCb) scripts.
These are a set of aliases, starting with `git lb-`, that are designed to make
developing LHCb software easier.  For example, if you want to write a custom
stripping selection, execute the following in the `DaVinciDev` directory:

```bash
git lb-use Stripping
git lb-checkout Stripping/master Phys/StrippingSelections
make configure
```

Under the hood, `git lb-use` will add the
[`Stripping`](https://gitlab.cern.ch/lhcb/Stripping) repository as a remote in
git – check this with `git remote -v`!  `git lb-checkout` will then perform a
*partial* checkout of the master branch of the Stripping repository, only
adding the files under
[`Phys/StrippingSelections`](https://gitlab.cern.ch/lhcb/Stripping/tree/master/Phys/StrippingSelections).

{% callout "Which project to use in `git lb-use`?" %}
The project name to pass to `git lb-use` depends on the directories you want
to check out and work on, and not on the project name you passed to `lb-dev`.
Moreover you can call `git lb-use` several times for different remote
projects in the same local project:

```bash
lb-dev --name DaVinciDev DaVinci/v45r1
cd DaVinciDev
git lb-use Analysis
git lb-use Stripping
git lb-use DaVinci
```

Not that in order for this to work, projects you specify in `lb-use` may not
depend on the project you specify in `lb-dev`. In other words, the top-level
project should be at the top of the [dependency
chain](http://lhcb-comp.web.cern.ch/lhcb-comp/).
{% endcallout %} 

You can now modify the `StrippingSelections` package and run `make purge &&
make` to build it with your changes.  You can test your changes with the
`./run` script.  It works similar to `lb-run`, without the need to specify a
package and version:
```bash
./run gaudirun.py options.py
```

{% callout "What if `git` asks for my password?" %}
Make sure you succesfully completed the instructions under [initial
setup](#initial-setup).
{% endcallout %} 

If you have made changes that you'd like to be integrated into the official
LHCb repositories, you can use `git lb-push` to push it to a new branch in the
central git repository. But please read the instructions in the [TWiki
page](https://twiki.cern.ch/twiki/bin/view/LHCb/Git4LHCb#Using_Git_for_LHCb_development)
first.

Depending on the project, you may be required to document your changes in the
release notes which are found in `doc/release.notes`.

Note that no-one has permission to push directly to the `master` branch of any
project. In order to get your changes merged there from the branch to which you
`lb-push`ed, you need to create a merge request, so the project maintainer can
inspect your code. This can be done on the [project repository web 
page](https://gitlab.cern.ch/lhcb/Stripping/merge_requests/new), for
example.

{% callout "Quick link to create a merge request" %}
When pushing to a branch in a project in GitLab you will see a message like:

```
remote:
remote: Create merge request for my-branch:
remote:   https://gitlab.cern.ch/lhcb/Stripping/merge_requests/new?merge_request%5Bsource_branch%5D=my-branch
remote:
```

You can use the URL in the message to quickly create a merge request for the
changes you just pushed.
{% endcallout %} 

When your merge request is approved (which can be after some additional commits
on your part), your changes are part of the `master` branch of the respective
project, and your contributions are officially part of the LHCb software stack.
Congratulations!

{% callout "Nightlies" %}
It is advisable to test new developments on the so-called [nightly
builds](https://lhcb-nightlies.cern.ch). Each project is built overnight
(hence the name), and all pending merge requests are applied. You can use a
nightly build version of a project with:

```bash
lb-dev --nightly lhcb-head DaVinci/HEAD
```

A more detailed description of the command is found here:

 * [SoftwareEnvTools](https://twiki.cern.ch/twiki/bin/view/LHCb/SoftwareEnvTools)

Sometimes mistakes happen and the committed code is either not compiling or
does not do what it is supposed to do.  Therefore the nightly tests are
performed. They first try to build the full software stack.

If that is successful, they run some reference jobs and compare the output of
the new build with a reference file.  The results of the nightly builds can
be found here.

* [Nightly builds summaries](https://lhcb-nightlies.cern.ch)

If the aim of the commit was to change the ouput, e.g. because you increased
the track reconstruction efficiency by a factor of two, mention it in the
merge request description, such that the manager of the affected project can
update the reference file.
{% endcallout %} 

If you want to take a look the source code, without checking it out, you can
easily access the repository through the [GitLab web
interface](https://gitlab.cern.ch/lhcb). This website also provides search
functionality, but the output is not always easy to read, especially if it
returns many hits. To search a project much quicker, you can use `Lbglimpse`.
It allows you to search for a given string in the source code of a particular
LHCb project.

```bash
Lbglimpse "PVRefitter" DaVinci v45r1
```
This works with every LHCb project and released version. Since it's a shell
command, you can easily process the output using `less`, `grep`, and other
tools.

To get an idea of how a certain component of the LHCb software works, you can
also access the doxygen documentation. One set of doxygen web pages is
generated for several related projects, and is linked in all the projects web
sites, like [for DaVinci](https://cern.ch/LHCb-release-area/DOC/davinci/).
See also the [LHCb Computing web page](http://cern.ch/lhcb-comp/) for a
list of projects.

{% callout "Working with a full project checkout" %}
The `lb-git` commands are not strictly necessary, but they're very convenient
if you just want to quickly edit one package. Otherwise you'd have to build
the entire project in which the package is residing, instead of using the
precompiled version. However, if you develop across multiple packages, or
want to use more sophisticated `git` commands, nothing prevents you from
checking out an entire project – just don't be surprised if it takes O(hours)
to compile!

To check out a project, run the following:

```bash
git clone https://:@gitlab.cern.ch:8443/lhcb/DaVinci.git
```

replacing `DaVinci` with the project name of your choice. Next, initialise
and compile it:

```bash
lb-project-init make
```

optionally followed by `make test` to run the tests and/or `make install` to
install it to the `InstallArea` directory. That's all! You now have a vanilla
git repository containing all the source files of the project.
{% endcallout %} 
