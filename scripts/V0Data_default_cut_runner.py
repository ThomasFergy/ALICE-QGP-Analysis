import os
import json
import subprocess
import numpy as np

########################################
# Set up using the cut_config.json file
########################################
cut_parameters = ["dcanegtopv", "dcapostopv", "v0cospa", "dcav0dau", "v0radius"]
with open("scripts/cut_config.json", "r") as f:
    cut_data = json.load(f)

# Which cuts to apply
par_indices = []
for cut in cut_parameters:
    if cut_data["cuts_to_apply"][cut]:
        par_indices.append(cut_parameters.index(cut))

# Set default cut values and all cut values to be used
default_cut_values = [
    round(cut_data["default_cuts"][cut], 12) for cut in cut_parameters
]
########################################
cwd = "data/V0Data"
output_dir = "output/V0Data"
json_file = "{}/json_strangenesstutorial.json".format(cwd)

cut_parameters_strangeness_tutorial = [
    "v0setting_dcanegtopv",
    "v0setting_dcapostopv",
    "v0setting_cospa",
    "v0setting_dcav0dau",
    "v0setting_radius",
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
    # set default cut values
    for i in range(len(cut_parameters)):
        set_cut_value(json_file, i, default_cut_values[i])

    # run the default cut values
    print("----- Running cut defaults -----")
    print()

    # Apply the cut (requires being in the alienv environment before running)
    bash_script = "./run_step0.sh"

    result = subprocess.run([bash_script], cwd=cwd)

    # check if output directory exists
    if not os.path.isdir(output_dir):
        os.mkdir(output_dir)
    # add .gitkeep file to output directory if it is empty
    if not os.listdir(output_dir):
        os.system("touch {}/.gitkeep".format(output_dir))

    # Rename the output file to avoid overwriting and move to output directory
    os.system(
        "mv {}/AnalysisResults.root {}/AnalysisResults_default_cuts.root".format(
            cwd, output_dir
        )
    )
