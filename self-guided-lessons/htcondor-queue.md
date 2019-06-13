# Using the queue command

{% objectives "Learning Objectives" %}
* Be able to submit multiple jobs at once by adding to the `queue` command
* Be able to use variables in a submit file to automatically change the behaviour of each job
* Learn some different options for queuing multiple jobs
{% endobjectives %} 

So far, we have ended all of our submit files by simply writing `queue` to tell HTCondor to schedule a single execution of the job that we defined. However, it wouldn't be batch computing without being able to submit large numbers of jobs at once!

Let's create another simple job using a python script, `my_script.py`. We want both file input and output - for this example, let's have it reverse the input text, and save it to a new file. We also want to specify both files as arguments. Such a script might look something like:

```
import argparse
parser = argparse.ArgumentParser()
parser.add_argument('inputfile', help='Specify an input file', type=str)
parser.add_argument('outputfile', help='Specify an output file', type=str)
args = parser.parse_args()

f_in = open(args.inputfile, 'r')
f_out = open(args.outputfile, 'w+')
f_out.write( ''.join( reversed(f_in.read()) ) )
```

As before, we'll use an executable shell script `exec.sh` as a wrapper:

```
#!/bin/bash
python3 my_script.py "$@"
```

And finally, the all-important submit file:

```
executable  = exec.sh
universe    = vanilla
arguments   = data.txt result.txt
+JobFlavour = "espresso"

transfer_input_files = my_script.py, input/data.txt
transfer_output_files = result.txt
should_transfer_files = IF_NEEDED
when_to_transfer_output = ON_EXIT

output      = output.out
error       = error.err
log         = log.log
queue 4
```

Most of this should look familiar at this point, with the most significant new addition being the final line: `queue 4`. If you create a `data.txt` file in the `input` directory, submit this, and then look at the queue, you will see that HTCondor has indeed scheduled 4 separate jobs. You can see these jobs in the queue individually by running `condor_q -nobatch`:

```
 ID         OWNER            SUBMITTED     RUN_TIME ST PRI SIZE CMD
2908604.0   djwhite         5/22 12:07   0+00:00:00 I  0    0.0 exec.sh data.txt result.txt
2908604.1   djwhite         5/22 12:07   0+00:00:00 I  0    0.0 exec.sh data.txt result.txt
2908604.2   djwhite         5/22 12:07   0+00:00:00 I  0    0.0 exec.sh data.txt result.txt
2908604.3   djwhite         5/22 12:07   0+00:00:00 I  0    0.0 exec.sh data.txt result.txt

4 jobs; 0 completed, 0 removed, 4 idle, 0 running, 0 held, 0 suspended
```

This shows a bit more information about the jobs than the standard `condor_q`. In particular, note that all of these jobs have the same cluster ID, but each has a different process ID ranging from 0 to 3. In almost all cases, when you submit multiple jobs using a single queue command, they will share the same cluster ID, and have sequential process IDs starting from 0.

There are a couple of issues with the above script. Firstly, we're running the exact same job (with the exact same input) multiple times, which is rarely what is needed. Secondly, once the jobs have all completed, notice that you only have one `output.out` file - each time one completes, it overwrites any existing output file with its own output because each one has the same name. In order to fix these issues, we need to be able to distinguish each job created by a single submit file. We can do this using the variables `$(ClusterId)` and `$(ProcId)` (or, equivalently, `$(Cluster)` and `$(Process)` respectively). These variables are automatically-defined for each job individually. Using this, you can modify your submit file to read:

```
executable  = exec.sh
universe    = vanilla
arguments   = data.txt result_$(ProcID).txt
+JobFlavour = "espresso"

transfer_input_files = my_script.py, input/data.txt
transfer_output_files = result_$(ProcID).txt
should_transfer_files = IF_NEEDED
when_to_transfer_output = ON_EXIT

output      = output/output.$(ClusterId).$(ProcId).out
error       = error/error.$(ClusterId).$(ProcId).err
log         = log/log.$(ProcId).log
queue 4
```

If you first create the folders `output`, `error`, and `log`, and then submit this job, HTCondor will automatically create 4 separate output and error files, and will transfer back 4 separate result files.

{% challenge "Multiple input files" %} The result files from the above script all contain the same information, since the same input file is being used and the script is deterministic. How could you modify it to allow for the use of multiple input files?

{% solution "Solution" %}
If you create multiple input files (e.g. `input/data_0.txt`, `input/data_1.txt`, ...), you can modify the `transfer_input_files` option to read:

```
transfer_input_files = my_script.py, input/data_$(ProcId).txt
```
{% endsolution %}

{% endchallenge %}


### More on variables

As you've seen above, the variables that HTCondor automatically defines provide a lot of potential for the automisation of large numbers of jobs. To expand on this, you are also able to define your own variables within your submit file using very similar syntax. Among other things, this is particularly useful for avoiding repetition - in the example above, the input and output file names had to be written in two places. Equivalent functionality can be achieved by using:

```
INPUTFN = data.txt
OUTPUTFN = result_$(ProcID).txt
# ...
arguments = $(INPUTFN) $(OUTPUTFN)
# ...
transfer_input_files = my_script.py, input/$(INPUTFN)
transfer_output_files = $(OUTPUTFN)
```

### Other ways to queue

The above syntax for queuing multiple jobs is very simple and convenient for jobs with sequential enumeration starting from zero, but this isn't always the case. Fortunately, the queue command is extremely flexible and has many alternative choices of syntax. This part of the lesson showcases some of these options and how to use them. For more information and other examples, you can visit [the documentation for the queue command](http://research.cs.wisc.edu/htcondor/manual/v8.5/2_5Submitting_Job.html#SECTION00352000000000000000).

* **Variable defined from a list**: ` queue [<int expr>] [<varname>] in [slice] <list of items>`

This defines a variable name and lets you give it a list of values, and HTCondor will create separate jobs for each value in the list. For example, you could use `queue MagnetPolarity in (up,down)` to automatically create two jobs, each with a variable `$(MagnetPolarity)` defined, which can then be used elsewhere in the submit file to pass either `up` or `down` to other options.

It's worth noting that the optional parameter `<int expr>` represents the number of jobs to create per element in the list. By default, its value is 1, meaning that one job is created for each list entry. If you were to instead run `queue 4 MagnetPolarity in (up,down)`, a total of 8 jobs would be created, i.e. 4 for `up`, and 4 for `down`, and the variable `$(ProcessId)` would run from 0 to 7. You can use another automatically-defined variable, `$(Step)`, to get the index of the jobs for each list element individually. In this example, `$(Step)` would run from 0 to 3 for jobs with a `$(MagnetPolarity)` value of `up`, and again from 0 to 3 for the `down` value.

* **Multiple variables defined from a list**: `queue [<int expr>] [<list of varnames>] from [slice] <file name> | <list of items>`

Expanding on the previous option, you can define multiple variables using the `from` syntax. For example, you could use:

```
queue MagnetPolarity,NTupleId from (
	up,00083875
	down,00083879
)
```

This would have the effect of creating 2 jobs: one with the values `up` and `00083875`, and one with the values `down` and `00083879`.

You can also put your list into a separate file if you prefer. The above example would be equivalent to using the command `queue MagnetPolarity,NTupleId from options.lst`, where you have a file `options.lst` which looks like:

```
up,00083875
down,00083879
```

* **Files or directories from a filesystem**: `queue [<int expr>] [<varname>] matching [files | dirs] [slice] <list of items with file globbing>`

This method of creating a list of input files is particularly convenient because it doesn't require you to manually list entries, or to define exactly how many files there are, as the previous methods have. Here, you can use wildcards to get HTCondor to automatically construct the list for you. For example, if you had a set of input `.dat` files and wanted to create a separate job for each one, you could use the command `queue INPUTFN matching files *.dat`.

### Mixing options and variables

It may have become apparent by now that options and variables behave in a very similar way within a submit file - you provide parameters to an option (e.g. `transfer_input_files = input.dat`) using the same syntax as you use to provide a value to a variable (e.g. `MyCustomVar = 25`). You can think of options as being specific variables that HTCondor looks at to determine how to create and schedule your jobs.

This observation allows for some additional flexibility when using the queue command - when specifying a variable name, you can choose to use the name of an already-defined submit file option for convenience. For example, you could use `queue arguments in (-a 200,-a 300,-a 400)` to automatically create 3 jobs with different arguments.



