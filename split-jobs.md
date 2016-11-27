---
layout: page
title: First Steps in LHCb
subtitle: Splitting a job into subjobs
minutes: 10
---
> ## Learning Objectives {.objectives}
>
> * Learn how to process many files in parallel on the grid
>   by splitting a job into many subjobs
>

In the [previous lesson](davinci-grid.html), you've submitted a job to the LHC grid.
You will notice that the job will take a long time to finish.
This is because it has to process many gigabytes of data.

`ganga` provides several *splitters* that implement strategies for processing data in parallel.
The one we will use now is `SplitByFiles`, which spawns several subjobs, each of which only processes a certain number of files.

Apart from processing data faster, this will also allow you to work with datasets that are spread across several sites of the LHC grid.
This is because a job can only access datasets that are available on the site that it runs on.

To activate a splitter, assign it to the `.splitter` attribute of your job:
```python
j.splitter = SplitByFiles(filesPerJob=5)
```
Note that the specified number of files per job is only the allowed maximum.
You will often get jobs with fewer files.

> ## How do I choose the number of files per job? {.callout}
> Choose fewer files per job if possible, as this will allow you to finish sooner and reduces the impact of jobs failing due to grid problems.
> Setting `filesPerJob=5` should work well for real data, while `filesPerJob=1` should be good for signal MC.

Now, when you run `j.submit()`, the job will automatically be split into several subjobs.
These can be displayed by entering

```python
jobs(787).subjobs
```
in `ganga`, where you have to replace 787 with the `id` of your main job.

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

If you want to do something more complex with each subjob, a regular `for`-loop can be used as well:
```python
for j in jobs(787).subjobs:
    print(j.id)
```

It's possible that some of your subjobs will be stuck in a certain state (submitting/completing/...).
If that is the case, try to reset the Dirac backend:
```python
jobs(787).subjobs(42).backend.reset()
```
If that doesn't help, try failing the job and resubmitting:
```python
jobs(787).subjobs(42).force_status('failed')
jobs(787).subjobs(42).resubmit()
```

It can take quite a while to submit all of your subjobs.
If you want to continue working in ganga while submitting jobs, you can use the `queues` feature to do just that.
Simply call `queues.add` with the `submit` function of a job without adding parentheses, like this:
```
queues.add(j.submit)
```
Ganga will then submit this job (and its subjobs) in the background.
Make sure not to close `ganga` before the submission is finished, or you will have to start submitting the rest of the jobs again later on.

> ## Splitting your first job {.challenge}
> Try splitting the `ganga` job from our previous lesson with `splitByFiles=1` ([reference code](code/12-split-jobs/first-job.py)) and submit it with `ganga`.


