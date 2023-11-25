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

cwd = "data/V0MC"
output_dir = "output/V0MC"
json_file = "{}/jsonConfigs/step3.json".format(cwd)

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


def combine_root_files(aodmcs_files):
    """
    Function to combine the root files
    """
    print("---------- Combining root files ----------")
    err_count = 0
    # find what files exist in all the aodmcs directories
    file_names = []
    for aodmcs_file in aodmcs_files:
        file_names.append(
            os.listdir(cwd + "/results/step3/{}/".format(aodmcs_file[:-4]))
        )
    file_names = list(set.intersection(*map(set, file_names)))
    print("Combining {} cut files...".format(len(file_names)))
    for file_name in file_names:
        files_to_combine = []
        for aodmcs_file in aodmcs_files:
            files_to_combine.append(
                "results/step3/{}/{}".format(aodmcs_file[:-4], file_name)
            )

        # if the file already exists in the output directory, skip it
        if os.path.isfile("{}/{}".format(output_dir, file_name)):
            print("Output file {} already exists".format(file_name))
            print("Skipping...")
            print()
            continue

        result = subprocess.run(
            ["hadd", "-f", "results/step3/{}".format(file_name), *files_to_combine],
            cwd=cwd,
        )
        err_count += result.returncode

        # check if output directory exists
        if not os.path.isdir(output_dir):
            os.mkdir(output_dir)
        # add .gitkeep file to output directory if it is empty
        if not os.listdir(output_dir):
            os.system("touch {}/.gitkeep".format(output_dir))

        if result.returncode == 0:
            # move the file to the output directory
            os.system("mv {}/results/step3/{} {}/".format(cwd, file_name, output_dir))

    if err_count != 0:
        print("root file combining ended with {} error(s)".format(err_count))
    else:
        print("root file combining ended successfully")


if __name__ == "__main__":
    err_count = 0
    aodmcs_files = set_aodmcs_files()

    for aodmcs_file in aodmcs_files:
        set_aodcms_file_json(aodmcs_file)
        print("---------- Applying cuts to {} ----------".format(aodmcs_file))
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
                aodmcs_output_dir = cwd + "/results/step3/{}".format(aodmcs_file[:-4])

                # check if output file has already been produced
                if os.path.isfile(
                    "{}/AnalysisResults_{}_{}.root".format(
                        aodmcs_output_dir, cut_parameters[par_index], cut_value
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
                bash_script = "./runStep3.sh"

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

                if not os.path.isdir(aodmcs_output_dir):
                    os.mkdir(aodmcs_output_dir)
                os.system(
                    "mv {}/results/step3/AnalysisResults.root {}/AnalysisResults_{}_{}.root".format(
                        cwd, aodmcs_output_dir, cut_parameters[par_index], cut_value
                    )
                )

    # reset default cut values
    for i in range(len(cut_parameters)):
        set_cut_value(json_file, i, default_cut_values[i])

    # combine the root files
    combine_root_files(aodmcs_files)

    if err_count != 0:
        print("Script finished with {} error(s)".format(err_count))
    else:
        print("Script finished successfully")
