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
# Set the cut value in the json file
# 0 = dcanegtopv
# 1 = dcapostopv
# 2 = v0cospa
# 3 = dcav0dau
# 4 = v0radius
par_indices = [0, 1, 2]
########################################
# Choose the cut values to use
cut_value_list = [
    np.arange(0.0, 0.4 + 0.005, 0.005),  # dcanegtopv
    np.arange(0.0, 0.4 + 0.005, 0.005),  # dcapostopv
    np.arange(0.95, 1 - 0.00125, 0.00125),  # v0cospa
    # np.arange(1.0, 1.1 + 0.005, 0.005),  # dcav0dau
    # np.arange(0.9, 0.95 + 0.005, 0.005),  # v0radius
]
########################################
# Default cut values
default_cut_values = [0.0, 0.0, 0.9, 2, 0.75]
########################################

cwd = "data/V0Data"
output_dir = "output/V0Data"
json_file = "{}/json_strangenesstutorial.json".format(cwd)

cut_parameters = ["dcanegtopv", "dcapostopv", "v0cospa", "dcav0dau", "v0radius"]
cut_parameters_strangeness_tutorial = [
    "v0setting_dcanegtopv",
    "v0setting_dcapostopv",
    "v0setting_cospa",
    "v0setting_dcav0dau",
    "v0setting_radius",
    # "cascadesetting_cospa",
    # "cascadesetting_cascradius",
    # "NSigmaTPCPion",
    # "NSigmaTPCProton",
]


def set_cut_value(json_file, cut_name_index, cut_value):
    """
    Function to set the cut value in the json file
    """
    with open(json_file, "r") as f:
        data = json.load(f)
    data["lambdakzero-builder"][cut_parameters[cut_name_index]] = str(cut_value)
    data["strangeness_tutorial"][
        cut_parameters_strangeness_tutorial[cut_name_index]
    ] = str(cut_value)
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

        cut_values = cut_value_list[par_index]
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

            # check if output directory exists
            if not os.path.isdir(output_dir):
                os.mkdir(output_dir)
            # add .gitkeep file to output directory if it is empty
            if not os.listdir(output_dir):
                os.system("touch {}/.gitkeep".format(output_dir))

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
