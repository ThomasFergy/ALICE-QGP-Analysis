#!/bin/bash
# log file where the terminal output will be saved
STEP="step3"
LOGFILE="log-${STEP}.txt"

# directory of this script
DIR_THIS="$(dirname "$(realpath "$0")")"

OPTION="-b --configuration json:/$DIR_THIS/jsonConfigs/$STEP.json --resources-monitoring 2 --aod-memory-rate-limit 1000000000 --shm-segment-size 6000000000"

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

# report status
rc=$?
if [ $rc -eq 0 ]; then
  echo "No problems!"
  mkdir -p "$DIR_THIS/results/$STEP"
  mv "AnalysisResults.root" "$DIR_THIS/results/$STEP/AnalysisResults.root"
  mv "dpl-config.json" "$DIR_THIS/jsonConfigs/$STEP.json"
else
  echo "Error: Exit code $rc"
  echo "Check the log file $LOGFILE"
  exit $rc
fi
