#!/usr/bin/bash

# Show start time
echo Start time : $(date)

python3 scripts/V0Data_cut_runner.py

python3 scripts/V0MC_cut_runner.py

# Show end time
echo End time : $(date)