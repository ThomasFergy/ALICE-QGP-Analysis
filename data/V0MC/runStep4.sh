#!/bin/bash
# log file where the terminal output will be saved
STEP="step4"
LOGFILE="log-${STEP}.txt"

# directory of this script
DIR_THIS="$(dirname "$(realpath "$0")")"

OPTION="-b --configuration json:/$DIR_THIS/jsonConfigs/$STEP.json"

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
| o2-analysis-lf-cascadebuilder ${OPTION} \
| o2-analysis-lf-cascadelabelbuilder ${OPTION} \
| o2-analysistutorial-lf-strangeness-$STEP ${OPTION} \
> "$LOGFILE" 2>&1

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
