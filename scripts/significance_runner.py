"""
Script to calculate the significance of the V0s
--- (requires being in the alienv environment before running) ---
--- (may require running the V0_cut_runner.py script first if no data exists yet) ---

Method:
  Step 1: Extract the cut values for the desired parameter from the output files in the output directory
  Step 2: For each parameter .root file in the output directory, run the SignificanceFit.C root script
  Step 3: save the data in a database for later use.
"""


import os
import sys
import json
import subprocess
import matplotlib.pyplot as plt

isMC = False

if isMC:
    cwd = "data/V0MC"
    output_dir = "output/V0MC"
else:
    cwd = "data/V0Data"
    output_dir = "output/V0Data"

cut_parameters = ["dcanegtopv", "dcapostopv", "v0cospa", "dcav0dau", "v0radius"]

K0_results = {
    "dcanegtopv": {},
    "dcapostopv": {},
    "v0cospa": {},
    "dcav0dau": {},
    "v0radius": {},
}
lambda_results = {
    "dcanegtopv": {},
    "dcapostopv": {},
    "v0cospa": {},
    "dcav0dau": {},
    "v0radius": {},
}

cpp_convert = {True: "true", False: "false"}


def get_cut_values(dir, cut_parameter_index):
    files = os.listdir(dir)
    # get all files with the cut_parameter in the name
    files = [f for f in files if cut_parameters[cut_parameter_index] in f]
    cut_values = []
    # cut values are the last part of the file name after the underscore
    for f in files:
        cut_values.append(float(f.split("_")[-1][:-5]))
    files_sorted = [x for _, x in sorted(zip(cut_values, files))]
    cut_values.sort()
    return [cut_values, files_sorted]


if __name__ == "__main__":
    # 0 = dcanegtopv
    # 1 = dcapostopv
    # 2 = v0cospa
    # 3 = dcav0dau
    # 4 = v0radius
    cut_index = 0

    # i = 0  # need loop later

    V0_names = ["K0", "Lambda"]
    V0_index = 0  # will also need to loop over these later

    cuts, files = get_cut_values(output_dir, cut_index)

    # loop through all cuts for the parameter

    significance_values = []
    for i in range(len(cuts)):
        figure_outputname = (
            V0_names[V0_index] + "_" + cut_parameters[0] + "_" + str(cuts[i]) + ".pdf"
        )
        print(figure_outputname)
        # not generalised at all yet
        if V0_index == 0:
            fit_params = "{1.2, 1.2, 1.4, 1.4, 0.49, 0.004, 3000, 1, 1, 1}"
        else:
            pass
        args = "{}, {}, {}, {}, {}, {}".format(
            cpp_convert[isMC],
            str(0.45),
            str(0.54),
            '"' + str("{}/".format(output_dir) + files[i]) + '"',
            '"{}"'.format(figure_outputname),
            fit_params,
        )

        result = subprocess.run(
            ["root", "-l", "-b", "-q", "MACROS/SignificanceFit.C({})".format(args)],
            stdout=subprocess.PIPE,
        )

        # print(result.stdout.decode("utf-8"))

        significance = float(result.stdout.decode("utf-8").split("$$$")[1])
        significance_values.append(significance)

# plot significance vs cut value
plt.plot(cuts, significance_values, "o")
plt.xlabel(cut_parameters[cut_index])
plt.ylabel("Significance")
plt.title(V0_names[V0_index])
plt.savefig("output/figures/significance_vs_{}.pdf".format(cut_parameters[cut_index]))
