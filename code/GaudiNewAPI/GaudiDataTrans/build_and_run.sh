#!/usr/bin/env bash

(cd .. && bash build.sh)
../run gaudirun.py options/gaudi_opts.py
