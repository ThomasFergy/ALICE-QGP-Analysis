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
import json
import subprocess
import numpy as np

########################################
# Which data to use:
isMC = False
########################################
# Set the cut value in the json file
# 0 = dcanegtopv
# 1 = dcapostopv
# 2 = v0cospa
# 3 = dcav0dau
# 4 = v0radius
par_indices = [0, 1]
########################################
# Choose the cut values to use
cut_range = [0.0, 0.4]
step = 0.005
cut_values = np.arange(cut_range[0], cut_range[1] + step, step)
########################################

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
    err_count = 0
    for par_index in par_indices:
        print(
            "---------- Applying cuts to {} ----------".format(
                cut_parameters[par_index]
            )
        )
        print()
        # set default cut values
        for i in range(len(cut_parameters)):
            set_cut_value(json_file, i, default_cut_values[i])

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

            print(
                "----- Running cut {} = {}... -----".format(
                    cut_parameters[par_index], cut_value
                )
            )
            print()

            # Apply the cut (requires being in the alienv environment before running)
            if isMC:
                bash_script = "./run_step0_MC.sh"
            else:
                bash_script = "./run_step0.sh"

            result = subprocess.run([bash_script], cwd=cwd)
            err_count += result.returncode

            # warn if error
            if result.returncode != 0:
                print(
                    "WARN: Error running MACROS/SignificanceFit.C for cut {} = {}".format(
                        cut_parameters[par_index], cut_value
                    )
                )

            # Rename the output file to avoid overwriting and move to output directory
            os.system(
                "mv {}/AnalysisResults.root {}/AnalysisResults_{}_{}.root".format(
                    cwd, output_dir, cut_parameters[par_index], cut_value
                )
            )

    # reset default cut values
    for i in range(len(cut_parameters)):
        set_cut_value(json_file, i, default_cut_values[i])

    if err_count != 0:
        print("Script finished with {} error(s)".format(err_count))
    else:
        print("Script finished successfully")
