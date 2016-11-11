GaudiNewAPI includes examples of the new API.

Some ways to get this to build on LXPlus:

export CMTPROJECTPATH=/cvmfs/lhcbdev.cern.ch/nightlies/lhcb-future/Today:$CMTPROJECTPATH

You can also get the nightlies using:

lb-run --nightly-cvmfs --nightly lhcb-future gaudi/Future

For the truly lazy, a build.sh has been included to automatically build this package, and make a nice link to the most recent "run".
