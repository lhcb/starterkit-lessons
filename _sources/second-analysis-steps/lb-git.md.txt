# Using git to develop LHCb software

{% objectives "Learning Objectives" %}

* Learn how to clone specific LHCb packages to a local development directory
* Learn how to make changes and upload them to be reviewed by others

{% endobjectives %}

{% callout "Prerequisites" %}

Before starting, you should have a basic understanding of how to use `git`,
similar to what has been taught during the starterkit.

{% endcallout %}

In this lesson, we'll show you a complete workflow for developing the LHCb software
using the `git` version control system.
At LHCb, we use GitLab to manage our git repositories.
Among other features, GitLab allows you to browse the source code for each project,
and to review new changes (called *merge requests*) in a convenient web interface.
You can find the CERN GitLab instance at [https://gitlab.cern.ch](https://gitlab.cern.ch).

In principle, there are multiple ways of interacting with the LHCb software repositories:

 1. A vanilla git workflow using only the standard git commands.
    This requires you to `clone` and compile an entire LHCb project at a time.
    This approach is recommended when your development is expected to last
    multiple days or more or if you collaborate with others.
 2. An LHCb-specific workflow using a set of `lb-*` subcommands.
    This allows you to check out individual packages inside a project, and
    streamlines the modification of a few packages at a time.
    This approach is mostly appropriate for quick and isolated changes.
    (This is closer to the previously used `getpack` command.)

Here, we want to focus on the second workflow.

Before jumping in by creating a project in GitLab, you should make sure that
your local git configuration and your settings on GitLab are sufficiently set up:

Your name and email address should be set up in your local `git` configuration.
To ensure that this is the case, run
```
$ git config --global user.name "Your Name"
$ git config --global user.email "Your Name <your.name@cern.ch>"
```
and put in your information.

Next, connect to [https://gitlab.cern.ch](https://gitlab.cern.ch) and log in
with your CERN credentials.

Visit [https://gitlab.cern.ch/profile/keys](https://gitlab.cern.ch/profile/keys) and add an SSH key.

Run this LHCb-specific configuration command:
```
git config --global lb-use.protocol ssh
```
This makes sure the LHCb commands use the ssh protocol instead of https.

In the GitLab web interface, create a new project by clicking on the "New project" button.
Give your project the name "LHCbSK".

We will now set up a local LHCb development area that is connected to the git
repository you just created.
In order to create a new dev environment, run
```
$ lb-dev --list LHCb
```
to see which versions of the LHCb project are available.
Pick the newest one and run
```
$ lb-dev --name LHCbSK LHCb/<version>
```
where you need to insert the version you picked earlier.
This will automatically run `git init` to create a new git repository with some initial files (`Makefile`, `CMakeLists.txt`, etc.).
In order to connect your repository with the remote one, run
```
cd LHCbSK
git remote add origin ssh://git@gitlab.cern.ch:7999/<username>/LHCbSK.git
```
where you need to substitute your username.
Run
```
git push -u origin master
```
once to define `master` as the remote branch that we should push to by default.

{% callout "Pushing your dev project to GitLab" %}

You don't necessarily need to create a remote git repository on GitLab for your
local dev project, especially if its lifetime will be short.
However, in case you plan to collaborate with someone, a remote repo where you
can synchronize your work is a must.
Pushing to GitLab is also good practice if you use this dev project for ganga's
`GaudiExec`.

{% endcallout %}

For the purpose of this tutorial, we've set up an `SKTest` project containing
the package `TheTestPackage`.
In order to be able to work on parts of the `SKTest` project, first run
```
git lb-use SKTest
```
This will define a remote `SKTest` and fetch the git repository from GitLab.

You can then run
```
git lb-checkout SKTest/master TheTestPackage
```
to check out the files inside the `SKTest` project that belong to the
`TheTestPackage` package.
This performs `git checkout` under the hood and commits the files into the
local (synthetic) branch.
You can check how this looks in the history with `git log`.

You can now run
```
make
```
to build the project, make some changes, run `make` again, etc.
You can also use
```
make test
```
to run the tests.
Once you're happy with the changes, run
```
git add <your-changed-files>
```
where you have to specify all files with changes that you want to store,
and
```
git commit
```
to store them in a new commit.
This will open up a text editor that will allow you to type in a commit message.

{% callout "Committing often" %}

There's no cost to committing often.
You should try to make a new commit every time you've made
modifications that can be considered a single unit of changes.

{% endcallout %}

Now push your changes to your newly created GitLab project
```
git push
```
and check that they appear on the website.

Once you want to upload your commits to the SKTest project for review, run
```
git lb-push SKTest <username>-new-feature
```
to create a new `<username>-new-feature` branch on the main GitLab repository
and upload your commits to it.
This will calculate the changes between your local dev repository and the
remote project repository, and create corresponding commits for the remote
project.
You can now create a merge request by going to
[https://gitlab.cern.ch/LHCb-SVN-mirrors/SKTest/merge_requests/new](https://gitlab.cern.ch/LHCb-SVN-mirrors/SKTest/merge_requests/new),
selecting the `<username>-new-feature` branch as the source and `master` as the target.
Add a title for your merge request and explain what you've done in the main text.

{% challenge "Merge conflicts" %}

- Working with a partner, try to create a merge conflict by making changes to
the same file.
  If you don't have someone else to work with, you can simulate this by creating a second development area.
- Now, try to resolve the merge conflict.
  This is a bit complicated because of the way the local repository is set up.
  Take a look at [https://twiki.cern.ch/twiki/bin/view/LHCb/Git4LHCb#Replacement_for_svn_update_in_lo](https://twiki.cern.ch/twiki/bin/view/LHCb/Git4LHCb#Replacement_for_svn_update_in_lo) for pointers.

{% endchallenge %}

{% callout "Working with entire projects" %}

Working with entire projects is just as simple, but requires a little more
care with the environment.
Most importantly, be prepared that the project repo clones and the build
products may need a few GB of disk space and that the compilation may take a
long time.
A step-by-step guide is available at [https://twiki.cern.ch/twiki/bin/view/LHCb/Git4LHCb#Building_everything_locally_exam](https://twiki.cern.ch/twiki/bin/view/LHCb/Git4LHCb#Building_everything_locally_exam).
A more integrated approach that automatically takes care of the environment is [https://gitlab.cern.ch/lhcb/upgrade-hackathon-setup](https://gitlab.cern.ch/lhcb/upgrade-hackathon-setup) and a variation of it that lets you use some of the nightly builds
to spare compiling many projects is at [https://gitlab.cern.ch/lhcb-HLT/trigger-dev](https://gitlab.cern.ch/lhcb-HLT/trigger-dev).

{% endcallout %}
