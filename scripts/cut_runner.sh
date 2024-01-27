#!/usr/bin/bash

# Show start time
echo Start time : $(date)

# Run the second Python script in the second alienv environment
alienv enter O2Physics/latest-rl-o2 << EOF
python3 scripts/V0MC_cut_runner.py
exit
EOF

# Run the first Python script in the first alienv environment
alienv enter O2Physics/latest-master-o2 << EOF
python3 scripts/V0Data_cut_runner.py
exit
EOF

# Show end time
echo End time : $(date)