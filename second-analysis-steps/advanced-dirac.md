# for the advanced dirac lesson

We have already started using Ganga, such as when [submitting jobs to the 
Grid](../first-analysis-steps/davinci-grid.md) and 
[using datasets from the bookkeeping](../first-analysis-steps/ganga-data.md), for
creating jobs; but there's a lot more you can do with it.

Part of Ganga's power comes from it being written in Python. When you run
`ganga`, you're given an IPython prompt where you input Python code that's executed when you hit `<enter>`.
The idea of running Python code extends outside of Ganga, where we can write
scripts that Ganga will execute when starting up. This lesson will focus on
writing job definition scripts, and exploring how we can define utility
functions that will be available across all of our Ganga sessions.
