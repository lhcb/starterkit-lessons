# Using SVN

{% objectives "Learning Objectives" %}
* Understand the differences between Git and SVN
* Learn how to interact with the LHCb SVN repositories
{% endobjectives %}


All of the code used within LHCb is stored in a single place, and all of the 
analysis documentation is stored in another repository.
These places are version-controlled _repositories_ that you can interact with 
using a program called `svn` (Subversion).

The principles of SVN are similar to those of Git, which you learnt during the 
Software Carpentry workshop: the history of file changes are saved in the 
repository; changes are marked using commits with associated messages, and 
multiple people can collaborate using the same repository, resolving merge 
conflicts if necessary.
However, there are some subtle differences between SVN and Git, which we'll go 
over in this lesson.

With Git, there are a few key actions you need to know to get going, and this 
is also true for SVN:

* Downloading a local copy of the repository;
* Making changes to the files within, or adding or removing files and 
  directories;
* Discarding local, uncommitted changes;
* Committing the changes (adding an entry to the repositories history); and
* Sending the commits to the remote repository.

When collaborating with others, there are a few additional things you need to 
be able to do:

* Updating your local repository with any newer changes in the remote one; and
* Resolving merge conflicts.

Doing these things with SVN is very similar to doing with Git, so we'll quickly 
go through the equivalent SVN command for doing each of these things with Git.

To copy a remote repository locally, use `svn checkout`, or `svn co` for short:

```bash
# Get some documentation, saved to the ANA-2015-004 folder
svn co svn+ssh://svn.cern.ch/reps/lhcbdocs/Notes/ANA/2015/004 ANA-2015-004
# Get some code
svn co svn+ssh://svn.cern.ch/reps/lhcb/Analysis/trunk/Phys/DecayTreeTuple
```

Notice that `lhcbdocs` is the name of the _repository_, but we can choose to 
checkout just a subfolder within it, in this case `Notes/ANA/2015/004`, rather 
than the whole repository.
This is **different** from Git, where you clone an entire repository.

After making changes to the files in the repository, you can check the status 
of the local copy of the repository and the remote copy.

```bash
# Check locally
svn status
# Check remotely with the -u flag
svn status -u
```

If you have changes you wish to commit back to the remote repository, use `svn 
commit`, or `svn ci` for short:

```bash
# Commit all changes in the local copy to the remote copy
svn ci -m "Add cool stuff."
# Commit only changes to certain files
svn ci -m "Add cool stuff to two chapters." selection.tex theory.tex
```

This is **different** from Git, as `git commit` will only add what's in the 
staging area to the history.
SVN does not have a staging area, so when you `svn commit` all changes in the 
repository are effectively automatically staged, in the same way when you use 
the `-a` flag with `git commit`.
Using `svn commit` will also automatically push the changes to the remote copy 
of the repository, which is also **different** from Git.

If you've made a change to the file but haven't committed it yet, you can 
discard your changes with `svn revert`:

```bash
svn revert my_changed_file.tex
```

If want to revert a file to some older version, you just need to find the 
appropriate revision number (like a Git commit hash) and use `svn merge` to 
essentially merge the old version of the file with the new version:

```bash
# Find the current and old revision number by looking at the history
svn log
# Revert the current revision number 123 to the old 109
svn merge -r 123:109 some_file.tex
# Save the changes
svn ci -m "Revert some_file.tex to revision 109."
```

When you have new files to add to the repository you can use `svn add`, and you 
can use `svn rm` to remove files you no longer want:

```bash
# Add a new file
svn add python.tex
# Remove some file that's no longer needed
svn rm cpp.tex
```

Finally, to update your local copy of the repository with newer changes in the 
remote copy, use `svn update`:

```bash
# Reminder: check the status of the remote repository
svn status -u
# Now fetch the newer changes and merge them in to the local repository
svn update
```

If you have conflicts when committing, for example if you have made changes to 
a file that has since been updated remotely, you will need to manually resolve the conflicts by editing the problematic file(s).

```bash
svn ci -m "Making a change that will conflict with the central repo."
# A conflict is detected, need to update
svn update
```

You can now edit the file, exit our editor, then type `r` to tell SVN that the conflict has been resolved.

You can play around with SVN by creating a folder in the [LHCb documents repository](https://svnweb.cern.ch/cern/wsvn/lhcbdocs), where every LHCb user is allowed to create their own folder in the `/Users` directory.

```bash
# Make a user's folder if it doesn't already exist
svn mkdir svn+ssh://svn.cern.ch/reps/lhcbdocs/Users/$USERNAME
# Then create the playground
svn mkdir svn+ssh://svn.cern.ch/reps/lhcbdocs/Users/$USERNAME/TestFolder
```

where you should substitute `$USERNAME` with your lxplus username.
This will open your editor, where you should enter a message describing the folder and then save it and close it.

You can checkout this repository and begin playing with it using the commands above.

```bash
svn co svn+ssh://svn.cern.ch/reps/lhcbdocs/Users/$USERNAME/TestFolder
```


{% callout "Using SVN outside the lxplus cluster" %}
When you work on lxplus, SVN automatically fetches your identity through your Kerberos token.

If you want to `getpack` and/or `commit` from other places, e.g. your local machine or the [online cluster](https://lbtwiki.cern.ch/bin/view/Online/WebHome), you will have to either setup SVN access via public SSH keys, or input your password several times for every action.

You can follow [this link](http://information-technology.web.cern.ch/book/how-start-working-svn/accessing-svn-repository#accessing-sshlinux) for more information. However, here are the steps to take to setup SSH access:

```bash
# Create an SSH key pair if you do not have one:
ssh-keygen -t rsa
# Copy your key to lxplus and run set_ssh:
scp ~/.ssh/id_rsa.pub USERNAME@lxplus.cern.ch:~/.ssh/
ssh USERNAME@lxplus
/afs/cern.ch/project/svn/public/bin/set_ssh
exit
# Now try to login to the SVN server:
ssh USERNAME@svn.cern.ch 
```

If this does not prompt you for a password, and you see a login message, you're done. Otherwise, make sure you are using the same key pair on which you ran `set_ssh`.
{% endcallout %}

