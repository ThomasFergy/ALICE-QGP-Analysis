#!/bin/bash

export IGNORE_VALIDITYCHECK_OF_CCDB_LOCALCACHE=1
export ALICEO2_CCDB_LOCALCACHE=../../locCCDB

LOGFILE="log.log"

export OPTIONS="-b --configuration json://${PWD}/json_strangenesstutorial_multiprocessing.json --resources-monitoring 2 --aod-memory-rate-limit 1000000000 --shm-segment-size 6000000000"

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        o2-analysis-timestamp ${OPTIONS} \
      | o2-analysis-event-selection ${OPTIONS} \
      | o2-analysis-bc-converter ${OPTIONS} \
      | o2-analysis-tracks-extra-converter ${OPTIONS} \
      | o2-analysis-lf-lambdakzerobuilder ${OPTIONS} \
      | o2-analysistutorial-lf-strangeness-step0 ${OPTIONS} \
>> "$LOGFILE" 2>&1
elif [[ "$OSTYPE" == "darwin"* ]]; then
        o2-analysis-timestamp ${OPTIONS} \
      | o2-analysis-event-selection ${OPTIONS} \
      | o2-analysis-bc-converter ${OPTIONS} \
      | o2-analysis-tracks-extra-converter ${OPTIONS} \
      | o2-analysis-v0converter ${OPTIONS} \
      | o2-analysis-lf-lambdakzerobuilder ${OPTIONS} \
      | o2-analysistutorial-lf-strangeness-step0 ${OPTIONS} \
>> "$LOGFILE" 2>&1
fi
