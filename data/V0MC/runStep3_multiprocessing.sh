#!/bin/bash

export IGNORE_VALIDITYCHECK_OF_CCDB_LOCALCACHE=1
export ALICEO2_CCDB_LOCALCACHE=../../locCCDB

STEP="step3"

LOGFILE="log.txt"

DIR_THIS="$(dirname "$(realpath "$0")")"

OPTION="-b --configuration json:/$DIR_THIS/step3_multiprocessing.json --resources-monitoring 2 --aod-memory-rate-limit 1000000000 --shm-segment-size 6000000000"

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
| o2-analysistutorial-lf-strangeness-$STEP ${OPTION} \
> "$LOGFILE" 2>&1
