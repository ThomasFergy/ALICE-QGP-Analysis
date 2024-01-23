import os
import json
import subprocess
import numpy as np


cut_parameters = ["dcanegtopv", "dcapostopv", "v0cospa", "dcav0dau", "v0radius"]
with open("scripts/cut_config.json", "r") as f:
    cut_data = json.load(f)

cut_parameters_strangeness_tutorial = [
    "v0setting_dcanegtopv",
    "v0setting_dcapostopv",
    "v0setting_cospa",
    "v0setting_dcav0dau",
    "v0setting_radius",
]

default_cut_values = [
    round(cut_data["default_cuts"][cut], 12) for cut in cut_parameters
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


cwd = "data/V0Data/p_t"
output_dir = "output/V0Data"
json_file = "{}/step3Data.json".format(cwd)

if __name__ == "__main__":
    # set default cut values
    for i in range(len(cut_parameters)):
        set_cut_value(json_file, i, default_cut_values[i])

    bash_script = "./runStep3Data.sh"

    result = subprocess.run([bash_script], cwd=cwd)

    # check if output directory exists
    if not os.path.isdir(output_dir):
        os.mkdir(output_dir)
    # add .gitkeep file to output directory if it is empty
    if not os.listdir(output_dir):
        os.system("touch {}/.gitkeep".format(output_dir))

    # Rename the output file to avoid overwriting and move to output directory
    os.system(
        "mv {}/AnalysisResults.root {}/AnalysisResults_default_cuts_p_t.root".format(
            cwd, output_dir
        )
    )

    ##### CAN'T GET THIS TO WORK #####