---
layout: page
title: First Steps in LHCb
subtitle: Split a job into subjobs
minutes: 10
---
> ## Learning Objectives {.objectives}
>
> * Learn how to process many files in parallel on the grid
>   by splitting a job into many subjobs
>

In the [previous lesson](11-davinci-grid.md), you've submitted a job to the LHC grid.
You will notice that the job will take a long time to finish.
This is because it has to process many gigabytes of data.

`ganga` provides several *splitters* that implement strategies for processing data in parallel.
The one we will use now is `SplitByFiles`, which spawns several subjobs, each of which only
processes a certain number of files.

To activate a splitter, assign it to the `.splitter` attribute of your job:
```python
j.splitter = SplitByFiles(filesPerJob=5)
```
Note that the specified number of files per job is only the allowed maximum.
You will often get jobs with fewer files.

Now, when you run `j.submit()`, the job will automatically be split into several subjobs.
These can be displayed by entering

```python
jobs(787).subjobs
```
in `ganga`, where 787 is the id of the main job.

You can access individual subjobs as in `jobs(787).subjobs(2)`.
For example, to resubmit a failed subjob, you would run
```python
jobs(787).subjobs(2).resubmit()
```

To access several subjobs at once, you can use the `.select` method:
```python
jobs(787).subjobs.select(status='failed').resubmit()
```
This will resubmit all failed subjobs.

If you want to do something more complex with each subjobs, a regular `for`-loop can be used as well:
```python
for j in jobs(787).subjobs:
    print(j.id)
```

