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

# read in the input file
try:
	f_in = open(args.inputfile, 'r')
catch:
	# put some error handling here!!!

# create the output file
try:
	f_out = open(args.outputfile, 'w+')
	f_out.write( f_in.read().reverse() )
		# ^- check that a string reverse function exists!!!
		# also check that reversing handles escape characters (newlines) properly
catch:
	# more error handling here!!!
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
transfer_input_files = input/data.txt
+JobDuration = 60 <!-- CHECK THIS -->

output      = output.out
error       = error.err
log         = log.log
queue 4
```

Most of this should look familiar at this point, with the most significant new addition being the final line: `queue 4`. If you create a `data.txt` file in the `input` directory, submit this, and then look at the queue, you will see that HTCondor has indeed scheduled 4 separate jobs. You can see these jobs in the queue individually by running `condor_q -batch`:

<!-- condor_q -batch output -->

Note that all of these jobs have the same cluster ID, but each has a different process ID ranging from 0 to 3. In almost all cases, when you submit multiple jobs using a single queue command, they will share the same cluster ID, and have sequential process IDs starting from 0.

There are a couple of issues with the above script. Firstly, we're running the exact same job (with the exact same input) multiple times, which is rarely what we need. Secondly, once the jobs have all completed, notice that you only have one `output.out` file - each time one completes, it overwrites any existing output file with its own output because each one has the same name. In order to fix these issues, we need to be able to distinguish the jobs created by a single submit file. We can do this using the variables

<!-- $(ClusterId) and $(ProcId) variables -->

<!-- 2 typical ways to manage files for lots of jobs: 1) folders for output, error, logs, input, etc., with different (numbered) filenames 2) different (numbered) folders for each job, with the same filenames (use initialdir = job$(ProcId)) -->




