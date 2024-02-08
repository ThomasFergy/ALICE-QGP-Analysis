#!/bin/bash 

export IGNORE_VALIDITYCHECK_OF_CCDB_LOCALCACHE=1
export ALICEO2_CCDB_LOCALCACHE=locCCDB

export OPTIONS="-b --configuration json://${PWD}/json_strangenesstutorialData3.json --resources-monitoring 2 --aod-memory-rate-limit 1000000000 --shm-segment-size 6000000000"


o2-analysis-timestamp ${OPTIONS} \
| o2-analysis-event-selection ${OPTIONS} \
| o2-analysis-bc-converter ${OPTIONS} \
| o2-analysis-tracks-extra-converter ${OPTIONS} \
| o2-analysis-lf-lambdakzerobuilder ${OPTIONS} \
| o2-analysis-lf-lambdakzerolabelbuilder ${OPTIONS} \
| o2-analysis-pid-tpc-base ${OPTIONS} \
| o2-analysis-pid-tpc ${OPTIONS} \
| o2-analysis-track-propagation ${OPTIONS} \
| o2-analysistutorial-lf-strangeness-step3 ${OPTIONS} 