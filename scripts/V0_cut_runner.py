"""
Script to batch apply cut values to V0s for further processing.
--- (requires being in the alienv environment before running) ---

Method:
  Step 1: Set the cut values to be used and on which parameter in the relevant json file
  Step 2: Run the run_step0(_MC).sh script to produce the output files
  Step 3: Rename the output file to avoid overwriting and place into relevant output directory
  Step 4: go back to step 1 and repeat until you have applied all the cuts you want
"""

import os
import sys
import json
import subprocess
import numpy as np

isMC = False

if isMC:
    cwd = "data/V0MC"
    output_dir = "output/V0MC"
    json_file = "{}/json_strangenesstutorialMC_sig.json".format(cwd)
else:
    cwd = "data/V0Data"
    output_dir = "output/V0Data"
    json_file = "{}/json_strangenesstutorial.json".format(cwd)


cut_parameters = ["dcanegtopv", "dcapostopv", "v0cospa", "dcav0dau", "v0radius"]

default_cut_values = [0.0, 0.0, 0.9, 1, 0.899999976]


def set_cut_value(json_file, cut_name_index, cut_value):
    """
    Function to set the cut value in the json file
    """
    with open(json_file, "r") as f:
        data = json.load(f)
    data["lambdakzero-builder"][cut_parameters[cut_name_index]] = str(cut_value)
    with open(json_file, "w") as f:
        json.dump(data, f, indent=2)


if __name__ == "__main__":
    # set default cut values
    for i in range(len(cut_parameters)):
        set_cut_value(json_file, i, default_cut_values[i])

    # Set the cut value in the json file
    # 0 = dcanegtopv
    # 1 = dcapostopv
    # 2 = v0cospa
    # 3 = dcav0dau
    # 4 = v0radius

    par_index = 0
    # cut_values = [0, 0.02, 0.04, 0.06, 0.08]
    cut_values = np.linspace(0, 0.2, 41)

    for cut_value in cut_values:
        set_cut_value(json_file, par_index, cut_value)

        # check if output file has already been produced
        if os.path.isfile(
            "{}/AnalysisResults_{}_{}.root".format(
                output_dir, cut_parameters[par_index], cut_value
            )
        ):
            print(
                "Output file already exists for cut {} = {}".format(
                    cut_parameters[par_index], cut_value
                )
            )
            print("Skipping...")
            print()
            continue

        print("Running cut {} = {}".format(cut_parameters[par_index], cut_value))
        print()

        # Apply the cut (requires being in the alienv environment before running)
        if isMC:
            bash_script = "./run_step0_MC.sh"
        else:
            bash_script = "./run_step0.sh"

        # log the command to be run in a temp file in case of error
        result = subprocess.run([bash_script], cwd=cwd)
        if result.returncode != 0:
            print("Error running bash script: {}".format(bash_script))
            print("Exiting...")
            sys.exit(1)

        # Rename the output file to avoid overwriting
        os.system(
            "mv {}/AnalysisResults.root {}/AnalysisResults_{}_{}.root".format(
                cwd, output_dir, cut_parameters[par_index], cut_value
            )
        )

    print("Script finished successfully")
