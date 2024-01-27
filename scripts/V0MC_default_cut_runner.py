import os
import json
import subprocess
import numpy as np

#################### p_t bins ####################
p_t_bins = np.linspace(0.0, 10.0, 11)
##################################################

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
cwd = "data/V0MC"
output_dir = "output/V0MC"
json_file = "{}/jsonConfigs/step3.json".format(cwd)

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


def set_aodmcs_files():
    """
    Function to set the aodmcs files
    """
    # search for all AO2D files in the data directory
    data_dir = "/disk/moose/alice/rl/run3/data/"
    root_files = []
    for root, dirs, files in os.walk(data_dir):
        for file in files:
            if file.endswith(".root"):
                root_files.append(os.path.join(root, file))

    # keep files with this format: AO2D....root
    root_files = [f for f in root_files if "AO2D" in f]
    # keep files with one underscore
    root_files = [f for f in root_files if f.count("_") == 1 or "AO2DMC" in f]

    # count how many unique runs there are
    runs = []
    AO2DMC_exists = False
    for root_file in root_files:
        if "AO2DMC" in root_file:
            AO2DMC_exists = True
            continue
        run_num = int(root_file.split("/")[-1].split("_")[1][:-5])
        runs.append(run_num)

    run_numbers = list(set(runs))

    seperated_files_list = []
    for run_number in run_numbers:
        seperated_files_list.append([f for f in root_files if str(run_number) in f])
    if AO2DMC_exists:
        seperated_files_list.append([f for f in root_files if "AO2DMC" in f])

    # create aodmcs files
    aodmcs_files = []
    for i in range(len(seperated_files_list)):
        with open("{}/aodmcs_{}.txt".format(cwd, i), "w") as f:
            for root_file in seperated_files_list[i]:
                f.write("{}\n".format(root_file))
        aodmcs_files.append("aodmcs_{}.txt".format(i))

    return aodmcs_files


def set_aodcms_file_json(aodmcs_file):
    """
    Function to set the aodmcs file in the json file
    """
    with open(json_file, "r") as f:
        data = json.load(f)
    data["internal-dpl-aod-reader"]["aod-file"] = "@" + aodmcs_file
    with open(json_file, "w") as f:
        json.dump(data, f, indent=2)


def set_p_t_bins(json_file, p_t_bins):
    """
    Function to set the p_t bins in the json file
    """
    with open(json_file, "r") as f:
        data = json.load(f)
    data["lambdakzero-builder"]["axisPtQA"]["values"] = p_t_bins
    with open(json_file, "w") as f:
        json.dump(data, f, indent=2)


if __name__ == "__main__":
    # clear /dev/shm/*
    os.system("rm -f /dev/shm/* >&/dev/null")

    # set p_t bins
    set_p_t_bins(json_file, p_t_bins.tolist())

    # set default cut values
    for i in range(len(cut_parameters)):
        set_cut_value(json_file, i, default_cut_values[i])

    aodmcs_files = set_aodmcs_files()

    # run the default cut values
    print("----- Running cut defaults -----")
    print()

    for aodmcs_file in aodmcs_files:
        set_aodcms_file_json(aodmcs_file)
        aodmcs_output_dir = cwd + "/results/step3/{}".format(aodmcs_file[:-4])
        # Apply the cut (requires being in the alienv environment before running)
        bash_script = "./runStep3.sh"
        result = subprocess.run([bash_script], cwd=cwd)

        if not os.path.isdir(aodmcs_output_dir):
            os.mkdir(aodmcs_output_dir)
        os.system(
            "mv {}/results/step3/AnalysisResults.root {}/AnalysisResults_default_cuts.root".format(
                cwd, aodmcs_output_dir
            )
        )

    # combine the root files
    print("----- Combining root files -----")
    print()

    files_to_combine = []
    for aodmcs_file in aodmcs_files:
        files_to_combine.append(
            "results/step3/{}/AnalysisResults_default_cuts.root".format(
                aodmcs_file[:-4]
            )
        )

    result = subprocess.run(
        [
            "hadd",
            "-f",
            "results/step3/{}".format("AnalysisResults_default_cuts.root"),
            *files_to_combine,
        ],
        cwd=cwd,
    )

    # remove the individual files
    for aodmcs_file in aodmcs_files:
        os.system(
            "rm {}/results/step3/{}/AnalysisResults_default_cuts.root".format(
                cwd, aodmcs_file[:-4]
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
        "mv {}/results/step3/AnalysisResults_default_cuts.root {}/AnalysisResults_default_cuts_MC.root".format(
            cwd, output_dir
        )
    )
