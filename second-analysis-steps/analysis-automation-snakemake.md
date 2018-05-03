# Analysis automation with snakemake

{% objectives "Learning Objectives" %}
* Learning about analysis automation and analysis preservation
* Learning snakemake syntax
{% endobjectives %}

## Motivation

> “The Snakemake workflow management system is a tool to create reproducible and scalable data analyses”

* A workflow management system allows you to:
  * Keep a memory of how your scripts are used
  * Keep a memory of where your inputs are stored
  * Run multiple steps in sequence
  * Automatically detect if something changed
* A workflow management forces you to:
  * Keep your code and your locations in order
  * Structure your code so that it is user-independent
  * Standardise your scripts
  * Bonus: Standardised scripts can sometimes be used across analyses!

## Snakemake: Documentation and installation

You can find full documentation on Snakemake [at this link](http://snakemake.readthedocs.io/en/stable/index.html).

You can find lines to install snakemake on linux in the file [at this link](https://github.com/lhcb/starterkit-lessons/blob/snakemake/second-analysis-steps/code/snakemake/install_snake.sh).
To download 

`wget https://raw.githubusercontent.com/lhcb/starterkit-lessons/snakemake/second-analysis-steps/code/snakemake/install_snake.sh`.

On lxplus it should be sufficient to execute it as `source install_snake.sh`.

## Tutorial

Snakemake allows you to create a set of rules, each one defining a "step" of your analysis.
The rules need to be written in a file called `Snakefile`.
For each step you need to provide:

  * The _inputs_: input files but also code (e.g. an executable)
  * The expected _output_. It's not important to list all possible outputs. 
  Just those that you want to keep monitored or that are used by a subsequent step as inputs.
  * _A command_ to go from input to output. (More details later)

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

Note that input and output can be parametrised! E.g.:

```python
rule copy:
    input: 'input.txt'
    output: 'output.txt'
    shell: 'cat {input} > {output}'
```

N.B.: Notice that:

  * Inputs and outputs can be of any type
  * You can provide python code after the tags. e.g. `input: glob("*.root")`
  * If a single file is input or output you are allowed to omit the brackets.

{% challenge "Try to make a snakefile with at one single rule" %}
{% endchallenge %}

To try out download:

`wget https://github.com/lhcb/starterkit-lessons/raw/snakemake/second-analysis-steps/code/snakemake/tutorial.tar`

You will find one containing names and phone numbers. You can make one rule that, given a name extracts the line with the phone of that person.

To do this in a shell you can use `grep`, which is a command that lists all lines in a file containing a certain text. 

```
$ grep ciao test.txt
ciao a tutti
```


### Usage and basic behaviour

And now that your `Snakefile` is done it's time to run! Just type

```bash
snakemake rulename
```

This will:
  1. Check that the inputs exist
      * If inputs exists 2)
      * If inputs do not exist or have changed snakemake will check if there is an other rule that produces them go back to 1)
  2. Run the command you defined in rule "rulename"
  3. Check that the output was actually produced.

Comments, which rules are run:

  * If want to run a chain of rules only up to a certain point just put the name of the rule up to which you want to run on the snakemake command.
  * If you want a rule to be "standalone" just do not give its input/outputs as outputs/inputs of any other rule
  * It is normal practice to put as a first rule a dummy rule that only takes as inputs all the "final" outputs you want to be created by any other rule. In this way when you run just `snakemake` with no label it will run all rules (in the correct order).

{% challenge "Try to make a snakefile with at least 3 rules connected to each other and run them all in one go" %}
{% endchallenge %}

In the tutorial folder you find two files containing addresses, and phone numbers.
You can make rules that, given a name, `grep` the address and phone and then one other rule to merge them into your final output file.

_But it does not have to be this, any other task is fine, be creative!_

Comments, partial running:

* If part of the input is already present and not modified present the corresponding rule will not run
N.B.: Note that if you put your code into the inputs snakemake will detect when your code changes and automatically rerun the corresponding rule!
* If you want to force running all rules even if part of the output is present use `snakemake --forceall`

{% challenge "In the previous example try deleting one of the intermediate files, rerun snakemake and see what happens" %}
{% endchallenge %}

### Run and shell

You have two ways to specify commands. One is `shell` that assumes shell commands as shown before.
The other is `run` that instead directly takes python code (N.B.: Careful it's python3!).

For example the copy of the file as in the previous example can be done in the following way.

```python
rule dosomething_py:
    input: 'myfile.txt'
    output: 'myoutput.txt'
    run:
        with open(input, 'rt') as fp:
            file_contents = fp.read()
        with open(output, 'wt') as fp:
            fp.write(file_contents)
```

And finally you can mix! Namely you can send shell commands from python code.
This is useful, in particular if you have to launch the same shell command on more inputs.

```python
rule dosomething_pysh:
    input:
        script = 'data1.root',
        data = 'data2.root']
    output: ['plot1.pdf', 'plot2.pdf']
    run:
        for f in input:
            shell('./{input.code} %s' % f)
```

{% challenge "Rewrite your previous file using a python script to run the search and use `run` to run on both phones and addresses in the same rule" %}
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

N.B.: The `--extra` is not necessary. It's just to illustrate how python scripts options can be used.

{% challenge "Making changes" %}
Add your python script to the inputs than make some modifications to it, rerun snakemake and see what happens.
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
    input: config['data'],
    output: ['plot1.pdf', 'plot2.pdf']
    run:
        for f in input:
            shell('./{input.code} %s' % f)
```

The config dictionary can be used anywhere, also inside the shell command or even outside a rule.

{% challenge "Put the inputs of your script into a config file" %}
{% endchallenge %}

### Includes

The Snakefile can quickly grow to a monster with tens of rules. For this reason it's possible to split them into more files and then include them into the Snakefile. For example you might have a `fit_rules.snake` and `efficiency_rules.snake` and then your Snakefile will look like this:

```python
include: /path/to/fit_rules.snake
include: /path/to/efficiency_rules.snake
```

The order of the includes is irrelevant.

{% challenge "Move your rules to other files and include them" %}
{% endchallenge %}