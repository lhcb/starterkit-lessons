# Analysis automation with snakemake

“The Snakemake workflow management system is a tool to create reproducible and scalable data analyses”

## Motivation

- A workflow management system allows you to:

  * Keep a memory of how your scripts are used
  * Keep a memory of where your inputs are stored
  * Run multiple steps in sequence
  * Automatically detect if something changed

- A workflow management forces you to

  * Keep your code and your locations in order
  * Structure your code so that it is user-independent
  * Standardise your scripts
  * Bonus: Standardised scripts can sometimes be used across analyses

## Snakemake: Documentation and installation

You can find full documentation on Snakemake [at this link](http://snakemake.readthedocs.io/en/stable/index.html).

You can find lines to install snakemake on linux in the file [at this link](https://github.com/lhcb/starterkit-lessons/blob/snakemake/second-analysis-steps/code/snakemake/install_snake.sh).
To download `wget https://raw.githubusercontent.com/lhcb/starterkit-lessons/snakemake/second-analysis-steps/code/snakemake/install_snake.sh`.

On lxplus it should be sufficient to exectue it as `source install_snake.sh`.

## Tutorial

Snakemake allows you to create a set of rules, each one defining a "step" of your analysis.
The rules need to be written in a file called `Snakefile`.
For each step you need to provide:

  * The _inputs_: this can be input files but also code (e.g. an executable)
  * The expected _output_. It's not important to list all possible outputs. Just those that you want to keep monitored or that can be then used by a successive step.
  * _A command_ to go from inpput to output. (More details later)

So the basic rule is:

```
rule myname :
    input : ['myinput1', 'myinput2']
    output: ['myoutput']
    shell : 'Some command to go from in to out'
```

An example. If you want to copy some text from a file called input.txt to output.txt you can do:

```
rule copy :
    input : ['input.txt']
    output: ['output.txt']
    shell : 'cat input.txt > output.txt
```

Note that intput and output can be parameterised! E.g. :

```
rule copy :
    input : ['input.txt']
    output: ['output.txt']
    shell : 'cat {input} > {output}'
```

N.B.: Notice that:

  * The inputs and outputs can be of any type
  * You can provide python code after the tags. e.g. `input: glob("*.root")`
  * If a single file is input or output you are allowed to omit the brakets.

### Usage and basic behaviour 

And now that your `Snakefile` is done it's time to run! Just type

```snakemake rulename```

This will :

  1. Check that the inputs exist
  2. Run the command you defined in rule "rulename"
  3. Check that the output was actually produced.

If the inputs do not exist or have changed snakemake will check if there is an other rule that produces them and, if yes, it will run all the needed rules in order.

Comments, which rules are run: 
    
  * If you build a chain of rules and want to run it only up to a certain point just put the name of the rule up to which you want to run on the snakemake command.
  * If you want a rule to be "standalone" just do not give its input/outputs as outputs/inputs of any other rule
  * It is normal practice to put as a first rule a dummy rule that only takes as inputs all the "final" outputs you want to be created by any other rule. In this way when you run just `snakemake` with no label it will run all rules (in the correct order). 

**Exercise:** Try to make a snakefile with at least 3 rules conected to each other and run them all in one go.
To try out download:

`wget https://github.com/lhcb/starterkit-lessons/raw/snakemake/second-analysis-steps/code/snakemake/tutorial.tar`

You will find two files containing names, addresses, and phone numbers.
You can make one rule that, given a name, "grep" the address and phone out of the phones.txt and addresses.txt files and then one other rule to merge them into your final output file. 

_But it does not have to be this, any other task is fine, be creative!_

Comments, partial running:

    * If part of the input is already present the corresponding rule will not run only the rules after the modified files
    N.B.: Note that if you put your code into the inputs snakemake will detect when your code changes and automatically rerun the corresponging rule!
    * If you want to force running all rules even if part of the output is present use "snakemake -f"

**Exercise:** In the previous example try deleting one of the intermediate files, rerun snakemake and see what happens.


### Run and shell

You have two ways to specify commands. One is `shell` that assumes shell commands as shown before.
The other is `run` that instead directly takes pathon code (N.B.: Careful it's python3!).

For example the copy of the file as in the provious example can be done in the following way. 

```
rule dosomething_py :
     input  : 'myfile.txt'
     output : 'myoutput.txt'
     run    :
         inpt = open(input[0]).read()
         fo = open(output[0],'w')
         fo.write(inpt)
         fo.close()
```

And finally you can mix! Namely you can send shell commands from python code.
This is useful, in particular if you have to launch the same shell command on more inputs.

```
rule dosomething_pysh :
     input  : ['data1.root'.'data2.root'],
     output : ['plot1.pdf','plot2.pdf']    
     run    :
        for f in input :
            shell('./{input.code} %s' % f)
```

**Exercise:** Rewrite your previous file using a python script to run the search and use "run" to run on both phones and addresses in the same rule.


### Sub-labels

Inside the prefedined tags you can add custom subtags as in this example.

```
rule run_some_py_script :
    input : 
        exe   = 'myscript.py',
        data  = 'mydata.root'
        extra = 'some_extra_info.txt'
    output: ['output.txt']
    shell : 'python {input.exe} {input.data} --extra {input.extra} > {output}
```

So this will effectively launch the command:

```python myscript.py mydata.roo --extra some_extra_info.txt > output.txt```

N.B.: The "--extra" is not necessary. It's just to illustrate how python scripts options can be used.

**Exercise:** Add your python script to the inputs than make some modifications to it, rerun snakemake and see what happens.

### Config files

Often you want to run the same rule on different sample or with different options for your scripts.
This can be done in snakemake using config files written in [yaml](https://learn.getgrav.org/advanced/yaml).

For example let's put the datafiles in a cfg.yaml file

```
data : 
    - 'data1.root'
    - 'data2.root'
```

Now in your Snakefile you can load this config file and then its content will be available into the rules as a dictionary called "config". Yes, it seems black magic, but it works! Your Snakefile will look like this

configfile : '/path/to/cfg.yaml'

```
rule dosomething_pysh :
     input  : config['data'],
     output : ['plot1.pdf','plot2.pdf']    
     run    :
        for f in input :
            shell('./{input.code} %s' % f)
```

The config dictionary can be used anywere, also inside the shell command or even outside a rule.

**Exercise:** Put the inputs of your scrupt into a config file.

### Includes

The Snakefile can quickly grow to a monster with tens of rules. For this reason it's possible to split them into more files and then include them into the Snakefile. For example you might have a "fit_rules.snake" and "efficiency_rules.snake"
and then your Snakefile will look like this:

```
include: /path/to/fit_rules.snake
include: /path/to/efficiency_rules.snake
```

The order of the includes is irrelevant.

**Exercise:** Move your rules to other files and include them.

