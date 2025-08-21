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
software using the Git version control system.  At LHCb, we use GitLab to
manage our Git repositories.  Among other features, GitLab allows you to browse
the source code for each project, and to review proposed changes (called *merge
requests*) in a convenient web interface.  You can find the CERN GitLab
instance at [https://gitlab.cern.ch](https://gitlab.cern.ch).

In principle, there are multiple ways of interacting with the LHCb software
repositories:

1. A vanilla Git workflow using only the standard Git commands.  This requires
   you to `clone` and compile an entire LHCb project at a time.
1. An LHCb-specific workflow using a set of `lb-*` Git subcommands.  This allows
   you to check out individual packages inside a project, and streamlines the
   modification of a few packages at a time.

Here, we want to focus on the second workflow. The first workflow will be
discussed briefly at the bottom ofthis page. Note that, although the 
`git lb-*` commands are much easier for small changes to existing packages 
where recompiling an entire project would be cumbersome, for any serious 
development the usage of vanilla Git is much more stable. Please consider 
using it if you can spare the compilation time.

{% callout "Initial setup" %}

Before jumping in by creating a project in GitLab, you should make sure that
your local Git configuration and your settings on GitLab are sufficiently set
up.

 - Your name and email address should be set up in your local Git
   configuration.  To ensure that this is the case, run
   ```
   git config --global user.name "Your Name"
   git config --global user.email "your.name@cern.ch"
   ```
   and put in your information.

 - Next, visit [https://gitlab.cern.ch/-/user_settings/ssh_keys](https://gitlab.cern.ch/-/user_settings/ssh_keys)
   (logging in with your CERN credentials) and add an SSH key.

 - Finally, run this LHCb-specific configuration command:
   ```
   git config --global lb-use.protocol ssh
   ```
   This makes sure the LHCb commands use the ssh protocol instead of https.

{% endcallout %}

This lesson introduces the commands:

 - `lb-dev` for setting up a new development environment
 - `git lb-use` and `git lb-checkout` for downloading LHCb software packages

If you want to make changes to a software package, you will need to set up a
development environment.
Since Centos7 is not supported anymore on lxplus, you should use a container for the following tasks.
```bash
apptainer exec --bind $PWD --env LBENV_SOURCED= --bind /cvmfs:/cvmfs:ro /cvmfs/lhcb.cern.ch/containers/os-base/centos7-devel/prod/amd64/ bash --rcfile /cvmfs/lhcb.cern.ch/lib/LbEnv
```
Now, `lb-dev` is your friend here:
```bash
lb-dev -c best --name DaVinciDev DaVinci/v45r8
```

The output should look similar to this:

```
Successfully created the local project DaVinciDev for x86_64_v2-centos7-gcc10-opt in .

To start working:

  > cd ./DaVinciDev
  > git lb-use DaVinci
  > git lb-checkout DaVinci/vXrY MyPackage

then

  > make
  > make test

and optionally (CMake only)

  > make install

To build for another platform call

  > make platform=<platform id>

You can customize the configuration by editing the files 'build.conf' and 'CMakeLists.txt'
(see https://twiki.cern.ch/twiki/bin/view/LHCb/GaudiCMakeConfiguration for details).
```

{% callout "lb-dev created local projects are Git repositories" %}

When `lb-dev` creates the local project directory and creates the initial
files there, it also calls `git init` and commits the first version of
the files to the local Git repository (try running `git log` in there).

You can then use Git to keep track of your development, and share your code
with others (for example with a [new project in
gitlab.cern.ch](https://gitlab.cern.ch/projects/new)).

You can even create a new project in GitLab without leaving the terminal!
Just define the git remote and a new project will be created as soon as you push.

```
git remote add ssh://git@gitlab.cern.ch:7999/<username>/<project-name>.git
git push -u origin master
```

{% endcallout %}

Follow those instructions to compile the software:

```bash
cd DaVinciDev
make
```

Once that's done, let's try it out:

```bash
./run gaudirun.py
```

We just successfully ran the simplest possible Gaudi job with `gaudirun.py`!
The `./run` script runs the command that follows in the project "runtime"
environment where the right varialbes are set.
It works similar to `lb-run`, without the need to specify a package and version.

{% discussion "Obtain a shell with the runtime environment" %}

Sometimes it is convenient to get a shell with the runtime environment
so that you don't need to prefix commands with `./run` all the time.
To do this, simply run

```bash
./run bash -l
```

While useful, it is generally advised to stick to prefixing commands with
`./run` (or `lb-run`). This way the command starts in the same reproducible
environment every time and there's less risk of getting to an inconsistent
state.

{% enddiscussion %}

Your new development environment won't be very useful without any software to
modify and build.  So let's check out one of the existing LHCb packages! These
are stored in the [LHCb Git repositories](https://gitlab.cern.ch/lhcb).

In order to obtain the source code of the package you want to work on, we'll
use the method described at the [Git4LHCb](http://lhcb-core-doc.web.cern.ch/lhcb-core-doc/GitForLHCbUsers.html) page.
These are a set of subcommands, starting with `git lb-`, that are designed to make
developing LHCb software easier.
For example, if you want to modify a `TupleTool`
(which you discovered in the [TupleTools and branches](add-tupletools) lesson),
start by executing the following in the `DaVinciDev` directory:

```bash
git lb-use Analysis
git lb-checkout Analysis/v21r8 Phys/DecayTreeTuple
```

{% discussion "What if `git` asks for my password?" %}

Make sure you succesfully completed the instructions in the initial
setup callout above.

{% enddiscussion %}

Under the hood, `git lb-use` will add and fetch the
[`Analysis`](https://gitlab.cern.ch/lhcb/Analysis) repository as a remote in
Git – check this with `git remote -v`!  `git lb-checkout` will then perform a
*partial* checkout of the `run2-patches` branch of the Analysis repository, only
adding the files under the
[`Phys/DecayTreeTuple`](https://gitlab.cern.ch/lhcb/Analysis/tree/run2-patches/Phys/DecayTreeTuple).
directory.

{% callout "Which project to use in `git lb-use`?" %}

The project name to pass to `git lb-use` depends on the directories you want
to check out and work on, and not on the project name you passed to `lb-dev`.
Moreover you can call `git lb-use` several times for different remote
projects in the same local project:

```bash
lb-dev --name DaVinciDev DaVinci/v45r8
cd DaVinciDev
git lb-use Analysis
git lb-use Stripping
git lb-use DaVinci
```

Note that in order for this to work, projects you specify in `lb-use` must
be dependencies of the project you specify in `lb-dev`. In other words, the top-level
project should be at the top of the [dependency
chain](http://lhcb.web.cern.ch/computing/).

{% endcallout %}

After checking out new packages, always remember to make them known to your
local project with `make configure` (sometimes it might be necessary to fully
clean up the build with `make purge`):

```bash
make configure
```

You can now modify the `Phys/DecayTreeTuple` package and run `make` to build
it with your changes. You can test your changes with the `./run` script.

```bash
./run gaudirun.py ntuple_options.py
```

If you have made changes that you'd like to be integrated into the official
LHCb repositories, you can use `git lb-push` to push it to a new branch in the
central Git repository. But please read the [instructions
page](http://lhcb-core-doc.web.cern.ch/lhcb-core-doc/GitForLHCbUsers.html#using-git-for-lhcb-development)
first.

Note that no one has permission to push directly to the official protected branches (e.g. `master` or `run2-patches`) of LHCb projects.
To get your changes merged there from the branch to which you `lb-push`-ed, you need to create a merge request (MR), so the project maintainer can check your code.
This can be done on the [project repository web page](https://gitlab.cern.ch/lhcb/Analysis/-/merge_requests/new), for example.

{% callout "Quick link to create a merge request" %}

When pushing to a branch in a project in GitLab you will see a message like:

```
remote:
remote: Create merge request for my-branch:
remote:   https://gitlab.cern.ch/lhcb/Analysis/merge_requests/new?merge_request%5Bsource_branch%5D=my-branch
remote:
```

You can use the URL in the message to quickly create a merge request for the
changes you just pushed.

{% endcallout %}

When your merge request is approved and merged (which can be after some additional commits on your part), your changes are part of an official branch of the respective project, and your contributions are part of the LHCb software stack.
Congratulations!

{% callout "Nightlies" %}

Typically before being approved every merge request will be tested in the so-called [nightly builds](https://lhcb-nightlies.web.cern.ch).
The nightly build infrastructure allows for building and testing the entire stack of LHCb physics projects, either overnight (in standard slots such as `lhcb-run2-patches`) or on demand (using `/ci-test` in GitLab discussions).
The relevant maintainers will help with the exact workflow (e.g. who schedules the testing), which varies a bit depending on the project.

The automatic builds and tests can show compilation problems (e.g. new warnings) and unintended consequences of the proposed changes. 
To achieve the latter, they run some reference jobs and compare the output of the new build with a reference file.

If the aim of the commit was to change the ouput, e.g. because you increased
the track reconstruction efficiency by a factor of two, mention it in the
merge request description, such that the maintainers of the affected project can
update the reference file.

Often it is useful to work directly from nightly builds (e.g. to debug an issue on another platform).
You can use a nightly build version of a project with:

```bash
lb-dev -c x86_64-centos7-gcc9-opt --nightly lhcb-run2-patches DaVinci/run2-patches
```

A more detailed description of the command is found at [SoftwareEnvTools](https://twiki.cern.ch/twiki/bin/view/LHCb/SoftwareEnvTools).

{% endcallout %}

If you want to take a look the source code, without checking it out, you can
easily access the repository through the [GitLab web
interface](https://gitlab.cern.ch/lhcb). This website also provides search
functionality, but the output is not always easy to read, especially if it
returns many hits. To search a project much quicker, you can use `lb-glimpse`.
It allows you to search for a given string in the source code of a particular
LHCb project (and all its dependencies).

```bash
lb-glimpse "PVRefitter" DaVinci/v45r8
```
This works with every LHCb project and released version. Since it's a shell
command, you can easily process the output using `less`, `grep`, and other
tools.

To get an idea of how a certain component of the LHCb software works, you can
also access the doxygen documentation. One set of doxygen web pages is
generated for several related projects, and is linked from the projects web
sites, like [for DaVinci](http://lhcbdoc.web.cern.ch/lhcbdoc/davinci/).
For example, the doxygen documentation for DaVinci v45r8 is
[here](https://lhcb-doxygen.web.cern.ch/lhcb-doxygen/davinci/v45r8/index.html).
See also the [LHCb Computing web page](https://lhcb.web.cern.ch/computing/) for a
list of projects.

{% callout "Working with a full project checkout" %}

The `git lb-*` commands are not strictly necessary, but they're very convenient
if you just want to quickly edit one package. Otherwise you'd have to build
the entire project in which the package is residing, instead of using the
precompiled version. However, if you develop across multiple packages, or
want to use more sophisticated `git` commands, nothing prevents you from
checking out an entire project – just don't be surprised if it takes hours
to compile on lxplus! Note that repeated compilation will be much faster
and using a dedicated multi-core machine can speed things up a lot.

To check out a project, run the following:

```bash
git clone https://:@gitlab.cern.ch:8443/lhcb/DaVinci.git
```

replacing `DaVinci` with the project name of your choice. Next, initialise
and compile it:

```bash
lb-project-init
make
```

optionally followed by `make test` to run the tests and/or `make install` to
install it to the `InstallArea` directory. That's all! You now have a vanilla
Git repository containing all the source files of the project.
You can find out more about working with full projects at the
[Git4LHCb](http://lhcb-core-doc.web.cern.ch/lhcb-core-doc/GitForLHCbUsers.html)
page.

{% endcallout %}
