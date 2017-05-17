#!/usr/bin/env bash

echo "Executing:"
echo " lb-project-init"
echo " make $@"
echo " ln -s build*/run run"
echo

if [[ ! -f Makefile ]]; then
lb-project-init
fi

make $@  && rm -f run && ln -s `ls -Art */run | tail -n1` run


