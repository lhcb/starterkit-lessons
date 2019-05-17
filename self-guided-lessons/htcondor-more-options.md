# More submit file options

{% objectives "Learning Objectives" %}
* Learn some more options you can specify for your jobs
* Be able to submit jobs with input/output files and arguments
* Be able to debug some common errors
{% endobjectives %} 

The options showcased in submit file we used in the previous section are enough for certain simpler jobs, but for many purposes we will want to do more. Here, we'll cover a selection of some of the more common and/or useful ones, and the settings you'll likely want to use for your own jobs. For more information, a complete list can be found [in the documentation](https://research.cs.wisc.edu/htcondor/manual/v7.6/condor_submit.html) for `condor_submit`.

### Input files

Often, you'll want your job to process data that's stored in another file. Create a file `input.txt` and add some lines of text to it. We want a simple executable that will read this file - here, we'll use the following bash script:

```
#!/bin/bash
echo "# Contents of file 'input.txt': #"
cat input.txt
```

Remember to make the script exectable, and to test it locally first. Then, add the following line to the body of your submit file:

```
transfer_input_files = input.txt
```

Once you've done this, you can submit your job. Behind the scenes, HTCondor has a remote temporary storage area where it runs your executable. The `transfer_input_files` option tells it to copy additional files to this storage area, so that the executable can access them.

{% callout "Transferring files from other directories" %} No matter where HTCondor copies a file from, by default it will place it in the same directory as the executable when it's running it. This means that if your file was stored in the directory `data/input.txt` instead, you would need to modify the path in the submit file to `transfer_input_files = data/input.txt`, but the executable above would not need to change.

You can access the remote area where your job is running using the command `condor_ssh_to_job -auto-retry <ClusterId>.<ProcID>`. Here, the cluster ID is the number it assigned to your job(s) upon submission (in the example from the previous section, this was 2875625), and the process ID is the number of the job within that cluster (for now, there is only one job, so this is just 0 - more on multiple jobs in the next section). From here, you can view the file structure. The executable is here, although it now has the name `condor_exec.exe`. Also, there are files `_condor_stdout` and `_condor_stderr` which contain the output to be transferred back to your output & error files later. The file `input.txt` can be found in this directory as well, regardless of where it was located in relation to your submit file. When you're finished, you can type `logout` to exit the ssh connection.
{% endcallout %}

You can use this to submit scripts in other formats, such as python scripts or C++ code. You will usually want to write a wrapper shell script to be your executable, and then to transfer your code as an input file. You can use the wrapper script to set up the environment as you need. For example, let's create a python script `process_file.py`:

```
f = open('input.txt', 'r')
print('# Contents of file \'input.txt\' using python: #')
print(f.read())
```

We'll also need to add this script to the list of input files, so change the relevant line in your submit file to:

```
transfer_input_files = process_file.py, input.txt
```

Finally, let's modify the executable so that it will run this python script. Change it so that it reads:

```
#!/bin/bash
python3 process_file.py
```

Now you can submit the job, and check that this works as expected.

### Output files

HTCondor uses the name 'output file' to refer to different things in different contexts, so it's important to make it clear which is meant. One of these is the one we've been using so far, which refers to the one specified in the submit file (using the `output` option) which contains the STDOUT (terminal output) of your executable. The other use is for any new files that your executable may create as it runs. In this part, we'll cover the second of these.

Let's modify our python script so that, instead of displaying the file contents in the terminal, it puts all the text into upper-case and saves it to a new file. This new script will look something like:

```
f_in = open('input.txt', 'r')
f_out = open('result.txt', 'w+')
print('Converting input file to upper-case...')
f_out.write( f_in.read().upper() )
```

Now you can submit this job. Once it has finished you will find that HTCondor has successfully produced the file `result.txt`, and has automatically put it in your working directory, without even having to add anything to the submit file! This is the default behaviour of the `transfer_output_files` option - when it isn't specified, it automatically transfers back all files that have been created or modified in the job's temporary working directory.

However, while this behaviour is helpful, it's generally preferable to specify which output files you want HTCondor to transfer back to you. There are a couple of reasons for this, but the most important ones are:

* Files aren't automatically transferred when running using certain universes (notably, this applies to most grid universes).
* If you don't want all files to be transferred back, but only a subset of them.
* If the files created are made inside of directories.

### Arguments

If your executable takes command-line arguments, you need to specify them in the submit file using the `arguments` option. Anything you put here will be passed as an argument to the executable.

To test this out, let's modify our python script to add an argument that will let us choose whether to change the text to upper or lower-case. For convenience, we'll use the `argparse` library:

```
import argparse
parser = argparse.ArgumentParser()
parser.add_argument('case', help='Choose what case to change the text to', type=str, choices=['upper','lower'])
args = parser.parse_args()

f_in = open('input.txt', 'r')
f_out = open('result.txt', 'w+')
print('Converting input file to {c}-case...'.format(c=args.case))
if args.case == 'upper':
	f_out.write( f_in.read().upper() )
elif args.case == 'lower':
	f_out.write( f_in.read().lower() )
```

We can now add our arguments to the submit file, by including the line:

```
arguments   = upper
```

This will pass the argument `upper` to our executable (equivalent to running `./my_script upper`). However, because we're using a wrapper shell script, we have to do one more step here in order to get it to pass these arguments to the python script. To do this, you can just modify your executable to be:

```
#!/bin/bash
python3 process_file.py "$@"
```

This will cause the script to pass all arguments it receives along to the python script.

Submitting the job now should give produce a file `result.txt` with all the text all in upper-case. You can also switch modes by changing the argument in the submit file from `upper` to `lower` - submitting this job should replace `result.txt` with a version in all lower-case.

### Notifications

If you want, you can tell HTCondor to send you emails updating you on the progress of your job. In your submit file, add the line:

```
notify_user = <your_email_address>
```

By default, this will send you an email when your job completes. You can change the conditions for receiving a notification using a second option:

```
notification = < Always | Complete | Error | Never >
```

The default setting is `Complete`. The `Error` setting is also extremely useful, especially for longer jobs that you may want to resubmit sooner rather than later if something goes wrong.

{% challenge "Debug some jobs" %} TODO: [Here]() you can download a `.tar(.GZ??)` file containing the scripts for NNN jobs, each of which features some commonly-experimenced error or mistake. For each one, submit the job, see if you can figure out what went wrong with it (making use of the error files), and then try to fix it. To unpack the file, run `LINE`.

{% solution "Solution 1" %}
{% endsolution %}

{% solution "Solution 2" %}
{% endsolution %}

{% solution "Solution 3" %}
{% endsolution %}

{% endchallenge %}





