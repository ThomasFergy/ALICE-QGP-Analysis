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

cwd = "data/V0Data"
output_dir = "output/V0Data"
original_json_file = "{}/json_strangenesstutorial_multiprocessing.json".format(cwd)

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


for i in range(len(cut_parameters)):
    set_cut_value(original_json_file, i, default_cut_values[i])


def create_tmp_cwd(cwd, name):
    """
    Function to create a temporary working directory
    """
    tmp_cwd = "{}/tmp_dirs/tmp_{}".format(cwd, name)
    if not os.path.isdir(tmp_cwd):
        os.mkdir(tmp_cwd)
    # copy json_strangenesstutorial.json and run_step0.sh to tmp_cwd
    os.system(
        "cp {}/json_strangenesstutorial_multiprocessing.json {}".format(cwd, tmp_cwd)
    )
    os.system("cp {}/run_step0_multiprocessing.sh {}".format(cwd, tmp_cwd))
    # make log file in tmp_cwd
    os.system("touch {}/log.log".format(tmp_cwd))
    return tmp_cwd


def remove_tmp_dir(tmp_dir):
    """
    Function to remove a temporary working directory
    """
    os.system("rm -rf {} >&/dev/null".format(tmp_dir))


def log(logfile, string):
    """
    Function to log a string to the log file
    """
    with open(logfile, "a") as f:
        f.write(string + "\n")


def process_cut(par_index, cut_value):
    # round the cut value to remove floating point errors and to prevent recreating the same file
    cut_value = round(cut_value, 12)

    # check if output file has already been produced
    if os.path.isfile(
        "{}/AnalysisResults_{}_{}.root".format(
            output_dir, cut_parameters[par_index], cut_value
        )
    ):
        return 0

    # create a temporary working directory
    tmp_cwd_name = str(cut_parameters[par_index]) + "_" + str(cut_value)
    tmp_cwd = create_tmp_cwd(cwd, tmp_cwd_name)

    logfile = "{}/log.log".format(tmp_cwd)

    log(
        logfile,
        "[INFO] ----- Running cut {} = {}... -----".format(
            cut_parameters[par_index], cut_value
        ),
    )

    json_file = "{}/json_strangenesstutorial_multiprocessing.json".format(tmp_cwd)
    set_cut_value(json_file, par_index, cut_value)

    # Apply the cut (requires being in the alienv environment before running)
    bash_script = "./run_step0_multiprocessing.sh"

    result = subprocess.run([bash_script], cwd=tmp_cwd)
    err = result.returncode

    # warn if error
    if result.returncode != 0:
        log(logfile, "[ERROR] Issue running ./run_step0_multiprocessing.sh")
        return err

    # Rename the output file to avoid overwriting and move to output directory
    os.system(
        "mv {}/AnalysisResults.root {}/AnalysisResults_{}_{}.root".format(
            tmp_cwd, output_dir, cut_parameters[par_index], cut_value
        )
    )

    log(
        logfile,
        "[INFO] ----- Finished running cut {} = {} -----".format(
            cut_parameters[par_index], cut_value
        ),
    )

    # rename the log file and move to logs directory
    os.system(
        "mv {}/log.log {}/logs/log_{}_{}.log".format(
            tmp_cwd, cwd, cut_parameters[par_index], cut_value
        )
    )

    # remove the temporary working directory
    remove_tmp_dir(tmp_cwd)
    return err


def process_wrapper(args):
    par_index, cut_value = args
    result = process_cut(par_index, cut_value)
    return result


if __name__ == "__main__":
    err_count = 0

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
            "---------- Applying cuts to {} ----------".format(
                cut_parameters[par_index]
            )
        )
        print()

        No_of_simultaneous_processes = -(
            mp.cpu_count() // -3
        )  # Can run out of memory if too high

        cut_values = cut_value_list[par_index]

        # apply cuts in parallel
        print(
            "Applying cuts in parallel using {} processes...".format(
                No_of_simultaneous_processes
            )
        )
        with mp.Pool(processes=No_of_simultaneous_processes) as pool:
            print("Processing: ", end="", flush=True)
            print("0%", end="", flush=True)
            first = True
            for i, result in enumerate(
                pool.imap_unordered(
                    process_wrapper,
                    [(par_index, cut_value) for cut_value in cut_values],
                )
            ):
                err_count += result
                progress = int(
                    (i + 1) / len(cut_values) * 100
                )  # Calculate progress as a percentage
                if first:
                    print("\b" * 2, end="", flush=True)
                    first = False
                print(f"{progress}%", end="", flush=True)
                print(
                    "\b" * (len(str(progress)) + 1), end="", flush=True
                )  # Erase the previous percentage
            print("100%\n")

    # remove tmp_dirs
    if os.path.isdir("{}/tmp_dirs".format(cwd)):
        os.system("rm -rf {}/tmp_dirs >&/dev/null".format(cwd))

    if err_count != 0:
        print("Script finished with {} error(s)".format(err_count))
    else:
        print("Script finished successfully")
