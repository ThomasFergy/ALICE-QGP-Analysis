#!/bin/bash

export IGNORE_VALIDITYCHECK_OF_CCDB_LOCALCACHE=1
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    export ALICEO2_CCDB_LOCALCACHE=/home/rl/alice/StrangenessHands-on/locCCDB
elif [[ "$OSTYPE" == "darwin"* ]]; then
    export ALICEO2_CCDB_LOCALCACHE=/Users/tom/code/ALICE-QGP-Analysis/data/V0MC/MCData/dataMCLHC23j4/locCCDB
fi

STEP="step3"

# directory of this script
DIR_THIS="$(dirname "$(realpath "$0")")"

OPTION="-b --configuration json:/$DIR_THIS/jsonConfigs/$STEP.json --resources-monitoring 2 --aod-memory-rate-limit 10000000000 --shm-segment-size 60000000000"

o2-analysis-timestamp ${OPTION} \
| o2-analysis-event-selection ${OPTION} \
| o2-analysis-bc-converter ${OPTION} \
| o2-analysis-tracks-extra-converter  ${OPTION} \
| o2-analysis-track-propagation ${OPTION} \
| o2-analysis-pid-tpc ${OPTION} \
| o2-analysis-pid-tpc-base ${OPTION} \
| o2-analysis-pid-tpc-full ${OPTION} \
| o2-analysis-lf-lambdakzerobuilder ${OPTION} \
| o2-analysis-lf-lambdakzerolabelbuilder ${OPTION} \
| o2-analysistutorial-lf-strangeness-$STEP ${OPTION} 