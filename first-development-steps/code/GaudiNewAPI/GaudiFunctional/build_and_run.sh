#!/usr/bin/env bash

echo "Running: "
echo '(cd .. && bash build.sh) && ../run gaudirun.py options/gaudi_opts.py'
echo ""

(cd .. && bash build.sh) && ../run gaudirun.py options/gaudi_opts.py
