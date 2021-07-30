# Scripting Ganga

We have already started using Ganga, such as when [submitting jobs to the 
Grid and using datasets from the bookkeeping](../first-analysis-steps/ganga.md), for
creating jobs; but there's a lot more you can do with it.

Part of Ganga's power comes from it being written in Python. When you run
`ganga`, you're given an IPython prompt where you input Python code that's executed when you hit `<enter>`.
The idea of running Python code extends outside of Ganga, where we can write
scripts that Ganga will execute when starting up. This lesson will focus on
writing job definition scripts, and exploring how we can define utility
functions that will be available across all of our Ganga sessions.


### Defining jobs with scripts

The `ganga` executable is similar to the `python` and `ipython` executables in 
a couple of ways.
If you just run `ganga`, you are dropped into a prompt, but you can also supply
the path to a Python script that will be executed.
Let's start with a small script, saving it in a file called `create_job.py`:

```python
greeting = 'Hello!'
print greeting
```

Run it:

```shell
$ ganga create_job.py

*** Welcome to Ganga ***
Version: X.Y.Z
…
Hello!
…
```

Sensible enough. Just like `python` and `ipython`, we can pass the `-i` flag 
before the file path to tell Ganga to give us a prompt after it's finished 
executing the script:

```shell
$ ganga -i create_job.py

*** Welcome to Ganga ***
Version: X.Y.Z
…
Hello!

Ganga In [1]: greeting
Ganga Out [1]: 'Hello!'

Ganga In [2]:
```

Notice that the variable we defined in the script, `greeting`, is available in 
the interactive session.
The idea of doing some work in a script and then manipulating the result 
interactively can be quite powerful.

One workflow that you might find useful is to create a script that defines a 
job, because this can often take a few lines to do, and typing them out every 
time is boring.
Let's modify `create_job.py` to do that.

```python
# Note: Ganga makes objects like `Job` available in your script automagically
j = Job()
j.name = 'My job'
```

This example is quite boring, but it captures the idea. You'll want to
extend this, changing the `application` property to a `GaudiExec` instance,
for example, as covered in [a previous
lesson](../first-analysis-steps/ganga.md).

Now we can run this and interact with the job as the `j` variable:

```shell
$ ganga -i create_job.py

*** Welcome to Ganga ***
Version: X.Y.Z
…

Ganga In [1]: j
Ganga Out [1]:
Job (
  comment = ,
  parallel_submit = False,
  …
)

Ganga In [2]:
```

We often want to construct a set of very similar jobs that differ only by their 
input data, for example running the same DaVinci application over 2015 and 2016 
data and for magnet up and magnet down configurations.
We need to then _parameterise_ our script, and one way of doing this is passing 
arguments to it by the command line.
You can inspect arguments from a Python script by using the `argv` 
property on the `sys` module:

```python
import sys
print sys.argv
```

Add that to your `create_job.py` script, and run `ganga` again, this time 
passing some arguments:

```shell
$ ganga -i create_job.py -v 123 --hello=world

*** Welcome to Ganga ***
Version: X.Y.Z
…

['create_job.py', '-v', '123', '--hello=world']

Ganga In [1]: j
```

Our script sees `sys.argv` as the list of the arguments that come after `ganga 
-i`.
To parameterise our script for year and magnet polarity, we could check this 
list to find one of `2015` or `2016` and one of `Up` or `Down`, for example, 
but instead we'll opt to use the excellent [`argparse` module][argparse], which 
comes with Python, to parse the command-line arguments for us.

```python
import argparse

parser = argparse.ArgumentParser(description="Make my DaVinci job.")
parser.add_argument('year', type=int, choices=[2015, 2016],
                    help='Year of data-taking to run over')
parser.add_argument('polarity', choices=['Up', 'Down'],
                    help='Polarity of data-taking to run over')
parser.add_argument('--test', action='store_true',
                    help='Run over one file locally')
args = parser.parse_args()

year = args.year
polarity = args.polarity
test = args.test
```

Nicely, `argparse` gives us a useful `--help` argument for free:

```shell
$ ganga -i create_job.py --help

*** Welcome to Ganga ***
Version: X.Y.Z
…
usage: create_job.py [-h] [--test] {2015,2016} {Up,Down}

Make my DaVinci job.

positional arguments:
  {2015,2016}  Year of data-taking to run over
  {Up,Down}    Polarity of data-taking to run over

optional arguments:
  -h, --help   show this help message and exit
  --test       Run over one file locally

Ganga In [1]:
```

This help will also be printed if we don't supply all of the required arguments 
(the year and the magnet polarity), along with a message telling us what's 
missing.

{% callout "Getting to grips with `argparse`" %}

The `argparse` module can do a lot, being able to parse complex sets of 
arguments with much difficultly. It's a useful tool to know in general, so we 
recommend that you check out the [documentation][argparse] to learn more.

[argparse]: https://docs.python.org/2/library/argparse.html

{% endcallout %}

When we do supply all the necessary arguments, the values are then available in
the `year`, `polarity`, and `test` variables:

```shell
$ ganga -i create_job.py 2015 Down

*** Welcome to Ganga ***
Version: X.Y.Z
…

Ganga In [1]: print year, polarity, test
2015 Down False
```

Once you've reached this level, a whole world of possibilities opens up! Here 
are a few things you might proceed to do with these parameters in your script:

* Fetch the corresponding dataset using a `BKQuery`;
* Give your `Job` object a specific name, e.g. `j.name = 
  'Ntuples_{0}_{1}.format(year, polarity)'`;
* Give data-specific options files to the `application` object, e.g. if you 
  have one options file per year defining `DaVinci().DataType`.

Of course, you can add as many arguments as you think might be useful.
Above we added the `--test` flag as an example: if this is `True`, you could 
run the application over only a single data file, and run the job locally 
rather than on the Grid (setting `j.backend` appropriately).

### Adding helpers functions

We've seen above how giving a script to `ganga` makes the variables defined in 
those scripts available interactively.
But what if you have, or would like to have, some set of your own custom helper 
methods defined in _every_ session? It would be annoying to have to run `ganga 
my_helpers.py` every time! Luckily, the `ganga.py` file comes to the rescue.

When Ganga starts, it looks for a file in your home directory (`echo $HOME`) 
called `.ganga.py` (note the starting period in the filename). If it finds such 
a file, it executes it in the context of the Ganga session, meaning the code in 
the file has access to Ganga objects like `Job`, `jobs`, and so on.
To demonstrate the behaviour, we can put a `print` statement on our 
`~/.ganga.py` file:

```python
print 'Yo!'
```

Then run `ganga` (no arguments needed):

```shell
$ ganga

*** Welcome to Ganga ***
Version: X.Y.Z
…
Yo!
…
```

Neat. The general idea for this file is two-fold:

1. Add commands that you always want executed when Ganga starts, e.g. `print 
   jobs.select(status='running')`; and
2. Define functions for commonly-performed tasks.

The latter is particularly interesting. Do you often find yourself creating a 
file that contains all the output LFNs of your job? Write a helper!

```python
def write_lfns(j, filename):
    """Write LFNs of all DiracFiles of all completed subjobs to fname."""
    # Treat a job with subjobs the same as a job with no subjobs
    sjobs = j.subjobs
    if len(sjobs) == 0:
        sjobs = [j]

    lfns = []
    for sj in sjobs:
        if sj.status != 'completed':
            print 'Skipping #{0}'.format(sj.id)
            continue
        for df in sj.outputfiles.get(DiracFile):
            lfns.append(df.lfn)

    with open(filename, 'w') as f:
        f.writelfns('\n'.join(lfns))
```

How about downloading and merging the ROOT output of a job's subjobs? Write a helper!

```python
def merge_root_output(j, input_tree_name, merged_filepath):
    # Treat a job with subjobs the same as a job with no subjobs
    sjobs = j.subjobs
    if len(sjobs) == 0:
        sjobs = [j]

    access_urls = []
    for sj in sjobs:
        if sj.status != 'completed':
            print 'Skipping #{0}'.format(sj.id)
            continue
        for df in sj.outputfiles.get(DiracFile):
            access_urls.append(df.accessURL())

    tchain = ROOT.TChain(input_tree_name)
    for url in access_urls:
        tchain.Add(url)
    tchain.Merge(merged_filepath)
```

Because of the way a [ROOT `TChain`][tchain] works, the subjobs output won't be 
downloaded, so you only need enough disk space for the merged file.

{% callout "Using ROOT in Ganga" %}

By default, ROOT is not available in a Ganga session:

```python
Ganga In [1]: import ROOT
ERROR    No module named ROOT
```

To remedy this, you can start Ganga inside an environment where ROOT _is_ 
available:

```shell
$ lb-run ROOT ganga
```

{% endcallout %}

Once you have your helpers defined, use them in Ganga as you would any other Python function.

```python
Ganga In [1]: j = jobs(123)

Ganga In [2]: write_lfns(j, '{0}.lfns'.format(j.name))
```

Here are some other common operations that you might want helpers for:

* Deleting all LFNs created by a job;
* Resetting the `backend` of all subjobs which are marked as `failed`;
* Replicating all LFNs to a specific Grid site.

What other tasks can you think of?

[tchain]: https://root.cern.ch/doc/master/classTChain.html
[argparse]: https://docs.python.org/2/library/argparse.html
