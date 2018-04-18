# Managing files in Ganga

{% objectives "Learning Objectives" %}
* Choose whether job output is saved locally or on the Grid
* Choose where to look for job input files
* Move files from any grid site to CERN, for analysis using EOS
{% endobjectives %} 

<!--
Ganga - Tasks, DiracFile and LocalFile, alternative to MassStorageFile with 
replication to CERN
Dirac - Replicating to CERN for use with EOS
Dirac CLI - downloading LFNs, finding where an LFN is replicated, finding what 
grid sites are available
EOS - xrdfs local usage
-->

Ganga allows you to define a job and have it run anywhere: on your local machine, 
on the [batch system][batch], or on the Grid.
This is very convenient as you don't need to worry about the specifics of each
platform.

Ganga treats files in a similar way to jobs, in that you only need to change
the object you're using to tell Ganga to use local files, files on the Grid, or
files on EOS.
In this lesson, we'll see how you can efficiently manage input and output files
using Ganga.


{% callout "Ganga versions" %}
It's generally advised to use the latest available version of Ganga. Functionality is not removed and there are no compatibility issues between versions (it is just python!). If you
encounter problems, you should first search [the archives of the
`lhcb-distributed-analysis` mailing list][da-archive]. If you don't find an
answer, you can talk to the Ganga developers directly on the [GitHub issues
page for Ganga][ganga-issues], or by sending an email to
`lhcb-distributed-analysis`.
{% endcallout %}

{% callout "Making a fresh start" %}
Throughout this lesson, we'll be using the most up-to-date version of Ganga 
that's available, 7.0.1 .
To make sure there will be no files from older versions of Ganga to interfere, 
we will move them to a backup location.

```shell
$ cd ~
$ mkdir ganga-backup
# See what's in your home directory that's related to Ganga
$ ls -la | grep -i ganga
# Then move everything
$ mv gangadir .gangarc* .ganga.log .ganga.py .ipython-ganga ganga-backup
```

You can move this back after the lesson if you want to restore your old 
settings and data.
{% endcallout %}

We'll be doing everything in Ganga, so let's start it up.

```shell
$ ganga
```

If it's your first time starting Ganga, you'll be asked if you want to create a 
default `.gangarc` file with the default settings.

> Would you like to create default config file `~/.gangarc` with standard 
> settings ([y]/n) ?

Answer with `y`. The `.gangarc` file defines the configuration of Ganga, and 
the defaults are normally good enough.

You'll then be dropped in a [IPython shell][ipython-shell].
We will create a job that runs a Python script that accepts a path to an input 
text file as an argument, and saves a file that contains the text of the file 
reversed. For example, it would save a file containing ‘!dlrow olleH’ if it was 
given a file containing ‘Hello world!’ as input.

[Download the script][reverse-script] to lxplus and set it to be executable. 
You can execute these commands inside Ganga, if you like, by prefixing them 
with a `!`.

```shell
$ wget https://lhcb.github.io/second-analysis-steps/code/01-managing-files-with-ganga/reverse.py
$ chmod +x reverse.py
$ ./reverse.py
Usage: reverse.py <file>
```

In Ganga, create a `Job` object with a descriptive name and take a look at it.

```python
j = Job(name='Reverser')
print j
```

You'll see that Ganga has created a `Job` which will execute the `echo` 
command, passing the list of arguments `['Hello World']`. Each element of this 
list will be passed as a positional argument to the `echo` command.

We'll replace the command name and the arguments, so that our `reverse.py` 
script is run with a text file as input.

```python
j.application.exe = File('reverse.py')
j.application.args = [File('input.txt')]
```

We haven't made `input.txt`, so let's make it by executing a couple of shell 
commands inside Ganga.

```python
!echo -e 'Hello world!\nThis is the second line.' > input.txt
!cat input.txt
```

Before submission, we just need to tell Ganga what to do with the output. The 
script saves the output to a file called like the input, but with `-reversed` 
appended before the file extension (`.txt` in this case), so we tell Ganga 
explicitly to move this file to the local job output directory.

```python
j.outputfiles = [LocalFile('input-reversed.txt')]
```

Now we can submit the job.

```python
j.submit()
```

By default, jobs run on the machine you're running Ganga on, as their `backend` 
property is set to an instance of the `Local` backend.

The job will finish very quickly, and we can inspect the output files.

```python
j.peek()
j.peek('input-reversed.txt')
```

There are a couple of file-related things to take note of in what we just did:

1. The `File` object is used to define local files that should be available in 
   the ‘working directory’ of the job (wherever it executes). We need both the 
   script and the input text file to be in the working directory, so both of 
   the paths to the files on our local machine are wrapped in `File`.
2. The `LocalFile` object is used to define what files in the working directory 
   of the job should be saved in the local job output directory, in this case 
   the file with `-reversed` in it.

Note that there are several files in the job output directory, seen with 
`j.peek()`, that we didn't explicitly ask for, most notably `stdout` and 
`stderr`. These two files are essentially the logs of the job, and Ganga always 
saves them in the local job output directory as they're almost always useful.  
For Gaudi jobs, Ganga will also automatically download the `summary.xml` file, 
which contains useful information about algorithm counters.

```python
df = DiracFile('input.txt', localDir='.')
df.put(uploadSE='CERN-USER')
print df.lfn
```

Grid files that are replicated at CERN are directly accessible via EOS. We can 
see that our file's on EOS by looking at the LFN Ganga gave us. We just need to 
add the prefix `/eos/lhcb/grid/user` to the LFN.

```shell
eos ls /eos/lhcb/grid/user//lhcb/user/a/apearce/GangaFiles_22.24_Wednesday_18_May_2016
```

{% callout "Using MassStorageFile" %}
The `MassStorageFile` object uploads job output directly to EOS. However, 
using `MassStorageFile` for this purpose is actively discouraged by the Ganga 
developers as it is highly inefficient: a file made on the Grid will first be 
downloaded to the machine running Ganga, and then uploaded to EOS.

Instead, always use `DiracFile` for large outputs, and then replicate them to 
`CERN-USER` if you want to be able to access them on EOS.
{% endcallout %}

If you have any `DiracFile`, you can ask for it to be replicated to a grid site 
it's not currently available at.

```python
df.replicate('CERN-USER')
```

{% callout "Automating replication to CERN" %}
If you have a job with subjobs, you can automate this to replicate all output 
files to CERN, so that you can run your analysis directly on the files on 
EOS.

```python
j = jobs(...)
for sj in j.subjobs:
  # Get all output files which are DiracFile objects
  for df in sj.outputfiles.get(DiracFile):
    # No need to replicate if it's already at CERN
    if 'CERN-USER' not in df.locations:
      df.replicate('CERN-USER')
```

After you did this your files will go into "/eos/lhcb/grid/lhcb/{u}/{user}/"+LFN.

You could make a function from this and put it in your `.ganga.py` file, whose 
contents is available in any Ganga session.
{% endcallout %}

You can download a `DiracFile` locally using the `get` method. If you already 
know an LFN, you can use this to quickly download it locally to play around 
with it. All you need to do is prefix the LFN with `LFN:`, and Ganga will 
assume that the file already exists on the Grid somewhere (whereas before it 
assumed the file was local).

```python
df2 = DiracFile('LFN:' + df.lfn)
# The directory used for the download must exist first
!mkdir foo
dfr2.localDir = "foo"
dfr2.get()
!cat foo/input.txt
```

We can tell Ganga to upload the job output to the Grid automatically.

```python
# Clone the job
j2 = Job(j)
j2.outputfiles = [DiracFile('*-reversed.txt')]
j2.submit()
```

Here we use a ‘pattern’ to tell Ganga that any file ending in `*-reversed.txt` 
should be uploaded to Grid storage. Both `DiracFile` and `LocalFile` support 
these patterns.

To download the output, we use `.get` as usual.

```python
j2.outputfiles.get(DiracFile)[0].get()
```

Being able to manipulate files with Ganga can be very useful. Particularly for 
Gaudi-based jobs where:

1. We often specify large sets of `DiracFiles` as input, from the bookkeeping, 
   but often want to download a file or two locally when testing options;
2. We want to duplicate a large number of output LFNs to `CERN-USER` so that we 
   can use them directly with EOS and XRootD commands;
3. We want to job output to be download locally automatically when the job 
   completes.

{% callout "Defining inputfiles" %}
The `inputfiles` attribute of a `Job` object works in a similar way to 
`outputfile`. In our example, the reverser script that the `Executable` 
application uses doesn't know how to handle things specified as `inputdata`, 
so we had to use `File` when defining the arguments.

For LHCb applications, you will almost always define the `inputdata` list 
using either `LocalFile` or `DiracFile` objects. Which one you will use just 
depends on where the input files are.
{% endcallout %}

[batch]: http://information-technology.web.cern.ch/services/batch
[da-archive]: https://groups.cern.ch/group/lhcb-distributed-analysis/default.aspx
[ganga-issues]: https://github.com/ganga-devs/ganga
[ipython-shell]: https://ipython.org/ipython-doc/3/interactive/tutorial.html
[reverse-script]: code/01-managing-files-with-ganga/reverse.py
