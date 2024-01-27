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
import multiprocessing as mp

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
cut_value_list = []
for cut in cut_parameters:
    cut_value_list.append(
        np.arange(
            cut_data[cut]["LowerLimit"],
            cut_data[cut]["UpperLimit"] + cut_data[cut]["step"],
            cut_data[cut]["step"],
        )
    )

cwd = "data/V0MC"
output_dir = "output/V0MC"
original_json_file = "{}/jsonConfigs/step3_multiprocessing.json".format(cwd)

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


# set default cut values
for i in range(len(cut_parameters)):
    set_cut_value(original_json_file, i, default_cut_values[i])


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


def set_aodcms_file_json(aodmcs_file, json_file):
    """
    Function to set the aodmcs file in the json file
    """
    with open(json_file, "r") as f:
        data = json.load(f)
    data["internal-dpl-aod-reader"]["aod-file"] = "@../../" + aodmcs_file
    with open(json_file, "w") as f:
        json.dump(data, f, indent=2)


def combine_root_files(cwd, par_index, cut_value):
    """
    Function to combine the root files
    """
    # find all root files in cwd
    root_files = []
    for root, dirs, files in os.walk(cwd):
        for file in files:
            if file.endswith(".root"):
                root_files.append(file)

    file_name = "AnalysisResults_{}_{}.root".format(
        cut_parameters[par_index], cut_value
    )

    result = subprocess.run(
        ["hadd", "-f", "{}".format(file_name), *root_files],
        cwd=cwd,
        capture_output=True,
    )
    err = result.returncode
    if err != 0:
        print(
            "Error combining root files for cut {} = {}".format(
                cut_parameters[par_index], cut_value
            )
        )
    return err


def create_tmp_cwd(cwd, name):
    """
    Function to create a temporary working directory
    """
    tmp_cwd = "{}/tmp_dirs/tmp_{}".format(cwd, name)
    if not os.path.isdir(tmp_cwd):
        os.mkdir(tmp_cwd)
    # copy json_strangenesstutorial.json and run_step0.sh to tmp_cwd
    os.system("cp {}/jsonConfigs/step3_multiprocessing.json {}".format(cwd, tmp_cwd))
    os.system("cp {}/runStep3_multiprocessing.sh {}".format(cwd, tmp_cwd))
    return tmp_cwd


def remove_tmp_dir(tmp_dir):
    """
    Function to remove a temporary working directory
    """
    os.system("rm -rf {}".format(tmp_dir))


def process_cut(par_index, aodmcs_files, cut_value):
    error_count = 0
    # round the cut value to remove floating point errors and to prevent recreating the same file
    cut_value = round(cut_value, 12)

    # check if output file has already been produced in output
    if os.path.isfile(
        "{}/AnalysisResults_{}_{}.root".format(
            output_dir, cut_parameters[par_index], cut_value
        )
    ):
        print(
            "Output file already exists for cut {} = {}\nSkipping...\n".format(
                cut_parameters[par_index], cut_value
            )
        )
        return 0

    print(
        "----- Running cut {} = {}... -----\n".format(
            cut_parameters[par_index], cut_value
        )
    )

    # create a temporary working directory
    tmp_cwd_name = str(cut_parameters[par_index]) + "_" + str(cut_value)
    tmp_cwd = create_tmp_cwd(cwd, tmp_cwd_name)

    json_file = "{}/step3_multiprocessing.json".format(tmp_cwd)
    set_cut_value(json_file, par_index, cut_value)

    bash_script = "./runStep3_multiprocessing.sh"

    for aodmcs_file in aodmcs_files:
        set_aodcms_file_json(aodmcs_file, json_file)

        # Apply the cut (requires being in the alienv environment before running)
        result = subprocess.run([bash_script], cwd=tmp_cwd)
        error_count += result.returncode

        # warn if error
        if result.returncode != 0:
            print(
                "WARN: Error running ./runStep3_multiprocessing.sh for cut {} = {}".format(
                    cut_parameters[par_index], cut_value
                )
            )
            return error_count

        # rename the output file
        os.system(
            "mv {}/AnalysisResults.root {}/AnalysisResults_{}.root".format(
                tmp_cwd, tmp_cwd, aodmcs_file[:-4]
            )
        )
        # rename the log file and move to logs directory
        os.system(
            "mv {}/log.txt {}/logs/log_{}_{}_{}.log".format(
                tmp_cwd, cwd, aodmcs_file[:-4], cut_parameters[par_index], cut_value
            )
        )
    # combine using hadd
    error_count += combine_root_files(tmp_cwd, par_index, cut_value)

    # move the output file to output directory
    os.system(
        "mv {}/AnalysisResults_{}_{}.root {}/AnalysisResults_{}_{}.root".format(
            tmp_cwd,
            cut_parameters[par_index],
            cut_value,
            output_dir,
            cut_parameters[par_index],
            cut_value,
        )
    )

    # remove the temporary working directory
    remove_tmp_dir(tmp_cwd)
    print(
        "----- Finished running cut {} = {} -----\n".format(
            cut_parameters[par_index], cut_value
        )
    )
    return error_count


if __name__ == "__main__":
    err_count = 0
    aodmcs_files = set_aodmcs_files()

    # clear /dev/shm/*
    os.system("rm -f /dev/shm/* >&/dev/null")

    # check if required directories exists
    if not os.path.isdir("{}/tmp_dirs".format(cwd)):
        os.mkdir("{}/tmp_dirs".format(cwd))
    if not os.path.isdir("{}/logs".format(cwd)):
        os.mkdir("{}/logs".format(cwd))
    if not os.path.isdir("output"):
        os.mkdir("output")
    if not os.listdir("output"):
        os.system("touch {}/.gitkeep".format("output"))
    if not os.path.isdir(output_dir):
        os.mkdir(output_dir)
    if not os.listdir(output_dir):
        os.system("touch {}/.gitkeep".format(output_dir))

    for par_index in par_indices:
        print(
            "---------- Applying cuts to {} for all MC runs ----------".format(
                cut_parameters[par_index]
            )
        )
        print()

        cut_values = cut_value_list[par_index]

        # apply cuts in parallel
        print("Applying cuts in parallel using {} processes...".format(mp.cpu_count()))
        with mp.Pool(processes=mp.cpu_count()) as pool:
            for result in pool.starmap(
                process_cut,
                [(par_index, aodmcs_files, cut_value) for cut_value in cut_values],
            ):
                err_count += result

    # remove tmp_dirs
    if os.path.isdir("{}/tmp_dirs".format(cwd)):
        os.system("rm -rf {}/tmp_dirs".format(cwd))

    if err_count != 0:
        print("Script finished with {} error(s)".format(err_count))
    else:
        print("Script finished successfully")
