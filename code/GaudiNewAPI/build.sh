#!/usr/bin/env bash

echo "Executing:"
echo ' export CMTPROJECTPATH=/cvmfs/lhcbdev.cern.ch/nightlies/lhcb-future/Today:$CMTPROJECTPATH'
echo " lb-project-init"
echo " make $@"
echo " ln -s build*/run run"
echo

export CMTPROJECTPATH=/cvmfs/lhcbdev.cern.ch/nightlies/lhcb-future/Today:$CMTPROJECTPATH
export CMTPROJECTPATH=$HOME/hackathon

if [[ ! -f Makefile ]]; then
lb-project-init
fi

make $@  && rm -f run && ln -s `ls -Art */run | tail -n1` run


