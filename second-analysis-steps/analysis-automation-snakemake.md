# Analysis automation with snakemake

{% objectives "Learning Objectives" %}

* Learn what analysis automation is and how it helps with analysis preservation
* Learn how to create a pipeline with Snakemake

{% endobjectives %}

## Motivation

> “The Snakemake workflow management system is a tool to create reproducible and scalable data analyses”

* A workflow management system allows you to:
  * Keep a record of how your scripts are used and what their input dependencies are
  * Run multiple steps in sequence, parallelising where possible
  * Automatically detect if something changes and then reprocess data if needed
* Using a workflow management forces you to:
  * Keep your code and your locations in order
  * Structure your code so that it is user-independent
  * Standardise your scripts
    * Bonus: Standardised scripts can sometimes be used across analyses!

## Documentation and environments

You can find full documentation for Snakemake [at this link](https://snakemake.readthedocs.io/en/stable/index.html), you can also ask any questions you have on the [~reproducible](https://mattermost.web.cern.ch/lhcb/channels/reproducible) channel on mattermost.

Snakemake is best-run at LHCb using the `lb-conda` environment. This environment comes with very recent versions of ROOT, python, cmake, g++, snakemake, etc ready to use. To have access to `lb-conda` you must first have sourced `lb-env`. This is done by default on lxplus, otherwise it is done with `source /cvmfs/lhcb.cern.ch/lib/LbEnv`. In general it is recommended that if you are running non-lhcb software (e.g. code you've written yourself for your analysis) it should be done with `lb-conda`.

{% callout "accessing the `lb-conda` environment" %}

The basic `lb-conda` environment can be entered with the command `lb-conda default`, this will enter you into an interactive bash shell. There are a few other way in which `lb-conda` can be used.

`lb-conda default foo` will run a specific `foo` command (e.g. a python program) within the `lb-conda` environment and then exit. This is similar in effect to the old `lb-run`-style command.

`lb-conda default bash -c 'bash --rcfile ~/.bashrc'` will enter the `lb-conda` environment as before but will also source your `.bashrc` file (be careful to avoid conflicts if doing this).

{% endcallout %}

More infomation on using `lb-conda` can be found [here](https://gitlab.cern.ch/lhcb-core/lbcondawrappers/-/blob/master/README.md).

<!--
```bash
python3 -m pip install --user snakemake
# Depending on your PATH variable you may also need to use:
alias snakemake='python3 -m snakemake'
```

{% callout "Installing on lxplus" %}

Unfortunately most LHCb software only supports Python 2 and doesn't provide a Python 3 installation.
When running on lxplus we recommend using the LCG Python 3 distribution and creating a function in your `.bashrc` to launch Snakemake so that it doesn't affect other LHCb applications.
This can be done using [this script](https://github.com/lhcb/starterkit-lessons/blob/master/second-analysis-steps/code/snakemake/install_snake.sh) by running:

```bash
curl -L https://cern.ch/go/Z8Nk | bash
source ~/.bashrc
```

{% endcallout %}
-->
You can now check if Snakemake is working by using `snakemake --help`.

## Tutorial

Snakemake allows you to create a set of rules, each one defining a "step" of your analysis.
The rules need to be written in a file called `Snakefile`.
For each step you need to provide:

  * The _input_: Data files, scripts, executables or any other files.
  * The expected _output_. It's not required to list all possible outputs.
  Just those that you want to monitor or that are used by a subsequent step as inputs.
  * A _command_ to run to process the input and create the output.

The basic rule is:

```python
rule myname:
    input: ['myinput1', 'myinput2']
    output: ['myoutput']
    shell: 'Some command to go from in to out'
```

An example. If you want to copy some text from a file called `input.txt` to `output.txt` you can do:

```python
rule copy:
    input: 'input.txt'
    output: 'output.txt'
    shell: 'cp input.txt output.txt'
```

You can even avoid typos by substituting variables instead of typing the filenames twice:

```python
rule merge_files:
    input: ['input_1.txt', 'input_2.txt']
    output: 'output.txt'
    shell: 'cat {input[0]} > {output} && cat {input[1]} >> {output}'
```

Input and output can also be parametrised using wildcards:

```python
rule copy_and_echo:
    input: 'input/{filename}.txt'
    output: 'output/{filename}.txt'
    shell: 'echo {wildcards.filename} && cp {input} {output}'
```

If you then make another rule with `output/a_file.txt` and `output/another_file.txt` as inputs they will be automatically created by the `copy_and_echo` rule.

```python
rule all:
     input: ['output/a_file.txt', 'output/another_file.txt']
```

This allows for rules to be reusable, for example to make a rule that can be used to process data with from different years or polarities.

Notice that:

  * Inputs and outputs can be of any type
  * You can provide python code after the tags. e.g. `input: glob("*.root")`
  * If a single file is input or output you are allowed to omit the brackets
  * Wildcards must always be present in the output of a rule (else it wouldn't be possible to know what they should be)

{% challenge "Write a snakefile with a single rule" %}


To try out download:

```bash
$ wget https://github.com/lhcb/starterkit-lessons/raw/master/second-analysis-steps/code/snakemake/tutorial.tar
$ tar -xvf tutorial.tar
```

You will find one containing names and phone numbers. You can make one rule that, given a name extracts the line with the phone of that person.

To do this in a shell you can use `grep`, which is a command that lists all lines in a file containing a certain text.

```bash
$ grep ciao test.txt
ciao a tutti
```

{% endchallenge %}

### Usage and basic behaviour

And now that your `Snakefile` is done it's time to run! Just type

```bash
snakemake rulename_or_filename
```

This will:
  1. Check that the inputs exist
      * If inputs exists &rarr; 2)
      * If inputs do not exist or have changed snakemake will check if there is an other rule that produces them &rarr; Go back to 1)
  2. Run the command you defined in `rulename_or_filename` (or the rule that generates the filename that is given)
  3. Check that the output was actually produced.

Comments, which rules are run:

  * If want to run a chain of rules only up to a certain point just put the name of the rule up to which you want to run on the snakemake command.
  * If you want a rule to be "standalone" just do not give its input/outputs as outputs/inputs of any other rule
  * It is normal practice to put as a first rule a dummy rule that only takes as inputs all the "final" outputs you want to be created by any other rule. In this way when you run just `snakemake` with no label it will run all rules (in the correct order).

{% challenge "Make a snakefile with at least 3 rules connected to each other and run them in one go" %}


In the tutorial folder you find two files containing addresses, and phone numbers.
You can make rules that, given a name, `grep` the address and phone and then one other rule to merge them into your final output file.

If we do this for “Luca”, it can be represented by the following graph:

[![DAG](img/snakemake-dag-single-wide.png)](img/snakemake-dag-single.png)

Which could be achieved using this shell script:

```bash
grep Luca inputs/addresses.txt > output/Luca/myaddress.txt
grep Luca inputs/phones.txt > output/Luca/myphone.txt
cat output/Luca/myaddress.txt > output/Luca/data.txt && cat output/Luca/myphone.txt >> output/Luca/data.txt
```

_But it does not have to be this, any other task is fine, be creative!_


{% endchallenge %}

{% challenge "Use wildcards" %}


Following on from the previous challenge use wildcards to make it so that any name can be used, such as “Fred”

```bash
snakemake output/Fred/data.txt
```

{% solution "Solution" %}


See `Snakefile` in the `simple_solution` folder [here](https://github.com/lhcb/starterkit-lessons/raw/master/second-analysis-steps/code/snakemake/tutorial.tar).

{% endsolution %}

{% endchallenge %}

Comments, partial running:

* If part of the input is already present and not modified present the corresponding rule will not run
Note that if you put your code into the inputs snakemake will detect when your code changes and automatically rerun the corresponding rule!
* If you want to force running all rules even if part of the output is present use `snakemake --forceall`

{% challenge "Explore the snakemake behaviour" %}

In the previous example try deleting one of the intermediate files, rerun snakemake and see what happens

{% endchallenge %}

### Sub-labels

Inside the pre-defined tags you can add custom subtags as in this example.

```python
rule run_some_py_script:
    input:
        exe = 'myscript.py',
        data = 'mydata.root',
        extra = 'some_extra_info.txt',
    output: ['output.txt']
    shell: 'python {input.exe} {input.data} --extra {input.extra} > {output}'
```

So this will effectively launch the command:

```bash
python myscript.py mydata.roo --extra some_extra_info.txt > output.txt
```

The `--extra` is not necessary. It's just to illustrate how python scripts options can be used.

{% challenge "Code as input" %}

Add your python script to the inputs than make some modifications to it, rerun snakemake and see what happens.

{% endchallenge %}



### Run and shell

You have two ways to specify commands. One is `shell` that assumes shell commands as shown before.
The other is `run` that instead directly takes python code (Careful it's python3!).

For example the copy of the file as in the previous example can be done in the following way.

```python
rule dosomething_py:
    input: 'myfile.txt'
    output: 'myoutput.txt'
    run:
        with open(str(input), 'rt') as fi:
            with open(str(output), 'wt') as fo:
                fo.write(fi.read())
```

And finally you can mix! Namely you can send shell commands from python code.
This is useful, in particular if you have to launch the same shell command on more inputs.

```python
rule dosomething_pysh:
    input:
        code = 'mycode.exe',
        data = [ 'data1.root', 'data2.root' ]
    output: ['plot1.pdf', 'plot2.pdf']
    run:
        for f in input.data:
            shell('./{input.code} %s' % f)
```

{% challenge "Use run instead of shell" %}

Rewrite your previous file using a python script to run the search and use `run` to run on both phones and addresses in the same rule

{% solution "Solution" %}

An example solution can be found [here](https://github.com/lhcb/starterkit-lessons/blob/master/second-analysis-steps/code/snakemake/Snakefile). 
Although it's fine if you have done it a different way.

{% endsolution %}

{% endchallenge %}

### Config files

Often you want to run the same rule on different sample or with different options for your scripts.
This can be done in snakemake using config files written in [yaml](https://learn.getgrav.org/advanced/yaml).

For example let's put the datafiles in a cfg.yaml file

```python
data:
    - 'data1.root'
    - 'data2.root'
```

Now in your Snakefile you can load this config file and then its content will be available into the rules as a dictionary called "config". Yes, it seems black magic, but it works! Your Snakefile will look like this

```python
configfile: '/path/to/cfg.yaml'

rule dosomething_pysh:
    input: 
        code = 'mycode.exe',
        data = config['data'],
    output: ['plot1.pdf', 'plot2.pdf']
    run:
        for f in input:
            shell('./{input.code} %s' % f)
```

The config dictionary can be used anywhere, also inside the shell command or even outside a rule.

{% challenge "Make a config file" %}

Put the inputs of your script into a config file

{% endchallenge %}

### Includes

The Snakefile can quickly grow to a monster with tens of rules. For this reason it's possible to split them into more files and then include them into the Snakefile. For example you might have a `fit_rules.snake` and `efficiency_rules.snake` and then your Snakefile will look like this:

```python
include: /path/to/fit_rules.snake
include: /path/to/efficiency_rules.snake
```

The order of the includes is irrelevant.

{% challenge "Use includes" %}

Move your rules to other files and include them

{% solution "Solution" %}


You can find a solution in the `more_complete_solution` folder, which you can find [here](https://github.com/lhcb/starterkit-lessons/raw/master/second-analysis-steps/code/snakemake/tutorial.tar).

{% endsolution %}

{% endchallenge %}

### Reports

As well as executing rules snakemake is also able to produce _reports_. These are html files and can contain information such as a diagramisation of your DAG as well as statistics about the run time of your rules and summaries of your outputs. To include a file in the report simply add the `report` flag to it e.g.

```python
rule myRule:
	input:
 		SomeFile.root
	output:
 		report(Output.root) # this will now be included in the report
	shell:
  		python RuleForExecution.py {input}
```

N.B. the reporting feature does not work with files already marked as `temp`

To produce the report you first run the `snakemake` command as you normally would.
Then run the exact same command again adding the `--report` flag as the first argument to your snakemake command.

Some screenshots of what a report may look like are shown below. Information in the report includes: a graph showing the DAG of the completed jobs, each node of this graph can be clicked to show the rule in more detail; the time taken to run each job; and a summary of all the produced files.

<!-- TODO add screenshots here -->


For more information on using reports as well as more examples, see the snakemake documentation [here](https://snakemake.readthedocs.io/en/stable/snakefiles/reporting.html).

### Workflow preservation

https://lhcb-dpa.web.cern.ch/lhcb-dpa/wp6/workflow-preservation.html
<!-- TODO add this properly -->
