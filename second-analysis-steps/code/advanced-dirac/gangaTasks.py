# First create the overall Task
t = CoreTask()

# Now create the Transform
trf1 = CoreTransform()
trf1.application = Executable()
trf1.application.exe = File('my_script.sh')
trf1.inputfiles = [LocalFile("distro.py")]
trf1.outputfiles = [LocalFile("*.txt")]
trf1.backend = Local()

# change the environment  object for each Unit/Master Job
trf1.unit_splitter = GenericSplitter()
trf1.unit_splitter.attribute = "application.env"
trf1.unit_splitter.values = [{'MYENV':'slow_gaussian'}, {'MYENV':'poisson'}, {'MYENV':'flat'}]

#change the argument object for each subjob in the Unit
trf1.splitter = GenericSplitter()
trf1.splitter.attribute = 'application.args'
trf1.splitter.values = [*range(100,1000,100)]

# Append the transform
t.appendTransform(trf1)

# Create the second transform
trf2 = CoreTransform()
trf2.application = Executable()
trf2.application.exe = File('my_other_script.sh')
trf2.inputfiles = [LocalFile("compare.py")]
trf2.outputfiles = [LocalFile("*.png")]

#specify transform dependancies
task_chain = TaskChainInput()
task_chain.input_trf_id = trf1.getID()
trf2.addInputData(task_chain)

trf2.splitter = GangaDatasetSplitter()
trf2.splitter.files_per_subjob = 3

t.appendTransform(trf2)
# set the maximum number of active jobs to have running (allows for throttling)
t.float = 100

# run the Task
t.run()
