"""
Script to calculate the significance of the V0s
--- (may require running the V0_cut_runner.py script first if no data exists yet) ---

Method:
  Step 1: Extract the cut values for the desired parameter from the output files in the output directory
  Step 2: For each parameter .root file in the output directory, run the SignificanceFit.C root script
  Step 3: save the data in a database for later use.
  Step 4: Run the SignificancePlot.C root script to plot the significance vs the cut value
"""
import os
import json
import subprocess

########################################
# Which data to use:
isMC = False
########################################
# which V0 to use:
# 0 = K0
# 1 = Lambda
V0_index = 0
########################################

if isMC:
    cwd = "data/V0MC"
    output_dir = "output/V0MC"
else:
    cwd = "data/V0Data"
    output_dir = "output/V0Data"

cut_parameters = ["dcanegtopv", "dcapostopv", "v0cospa", "dcav0dau", "v0radius"]

V0_names = ["K0Data", "LambdaData", "K0MC", "LambdaMC"]

isK0 = V0_index == 0
if isMC:
    V0_index += 2

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
    err_count = 0

    # 0 = dcanegtopv
    # 1 = dcapostopv
    # 2 = v0cospa
    # 3 = dcav0dau
    # 4 = v0radius
    cut_indicies = [0, 1, 2, 3, 4]

    # make file if it doesn't exist
    if not os.path.exists("output/significance.json"):
        content = {
            "K0Data": {
                "dcanegtopv": {},
                "dcapostopv": {},
                "v0cospa": {},
                "dcav0dau": {},
                "v0radius": {},
            },
            "LambdaData": {
                "dcanegtopv": {},
                "dcapostopv": {},
                "v0cospa": {},
                "dcav0dau": {},
                "v0radius": {},
            },
            "K0MC": {
                "dcanegtopv": {},
                "dcapostopv": {},
                "v0cospa": {},
                "dcav0dau": {},
                "v0radius": {},
            },
            "LambdaMC": {
                "dcanegtopv": {},
                "dcapostopv": {},
                "v0cospa": {},
                "dcav0dau": {},
                "v0radius": {},
            },
        }
        with open("output/significance.json", "w") as f:
            json.dump(content, f, indent=2)

    # load json file
    with open("output/significance.json", "r") as f:
        results = json.load(f)

    # loop through each cut parameter
    for cut_index in cut_indicies:
        cuts, files = get_cut_values(output_dir, cut_index)

        if len(files) == 0:
            err_count += 1
            print(
                "WARN: No files found for cut parameter {}".format(
                    cut_parameters[cut_index]
                )
            )
            continue

        ######### RUN SIGNIFICANCE FIT MACRO #########

        # loop through all cuts for the parameter
        for i in range(len(cuts)):
            print()
            # check if key exists
            if str(cuts[i]) in results[V0_names[V0_index]][cut_parameters[cut_index]]:
                print(
                    "Skipping cut {} = {} as already cached in json".format(
                        cut_parameters[cut_index], cuts[i]
                    )
                )
                continue

            print(
                "----- Running cut {} = {}... -----".format(
                    cut_parameters[cut_index], cuts[i]
                )
            )
            print()
            figure_outputname = (
                V0_names[V0_index]
                + "_"
                + cut_parameters[cut_index]
                + "_"
                + str(cuts[i])
                + ".pdf"
            )

            if V0_index == 0:
                # fit_params = "{1.2, 1.2, 1.4, 1.4, 0.49, 0.004, 3000, 1, 1, 1}"
                # MACRO = "SignificanceFitDSCB.C"
                xlow = 0.45
                xhigh = 0.54
                MACRO = "SignificanceFitGaussPoly.C"
                fit_params = "{1, 0.0049, 3000, 1, 1, 1}"
            elif V0_index == 1:
                xlow = 1.08
                xhigh = 1.15
                MACRO = "SignificanceFitGaussPoly.C"
                fit_params = "{1.115, 0.002, 1200, 1, 1, 1}"
            elif V0_index == 2:
                print("K0 MC not implemented yet")
                exit(1)
            elif V0_index == 3:
                print("Lambda MC not implemented yet")
                exit(1)

            # TODO: Not tested on lamdas or MC yet
            args = "{}, {}, {}, {}, {}, {}, {}".format(
                cpp_convert[isMC],
                cpp_convert[isK0],
                str(xlow),
                str(xhigh),
                '"' + str("{}/".format(output_dir) + files[i]) + '"',
                '"{}"'.format(figure_outputname),
                fit_params,
            )

            result = subprocess.run(
                ["root", "-l", "-b", "-q", "MACROS/{}({})".format(MACRO, args)],
                stdout=subprocess.PIPE,
            )

            print(result.stdout.decode("utf-8"))

            err_count += result.returncode

            # warn if error
            if result.returncode != 0:
                print(
                    "WARN: Error running MACROS/SignificanceFit.C for cut {} = {}".format(
                        cut_parameters[cut_index], cuts[i]
                    )
                )

            significance = float(result.stdout.decode("utf-8").split("$$$")[1])

            significance_error = float(result.stdout.decode("utf-8").split("$$$")[3])

            signal = float(result.stdout.decode("utf-8").split("$$$")[5])

            signal_error = float(result.stdout.decode("utf-8").split("$$$")[7])

            # save to results dict
            fit_results = {
                "significance": significance,
                "significance_error": significance_error,
                "signal": signal,
                "signal_error": signal_error,
            }

            results[V0_names[V0_index]][cut_parameters[cut_index]][
                cuts[i]
            ] = fit_results

        # save to json file
        with open("output/significance.json", "w") as f:
            json.dump(results, f, indent=2)

        ######### RUN SIGNIFICANCE PLOT MACRO #########

        filepath = "output/figures/{}_significance_{}.pdf".format(
            V0_names[V0_index], cut_parameters[cut_index]
        )

        # load all significance values from json file
        with open("output/significance.json", "r") as f:
            results = json.load(f)

        significance_values = []
        significance_errors = []
        cuts = []
        for cut in results[V0_names[V0_index]][cut_parameters[cut_index]]:
            cuts.append(cut)
            significance_values.append(
                results[V0_names[V0_index]][cut_parameters[cut_index]][cut][
                    "significance"
                ]
            )
            significance_errors.append(
                results[V0_names[V0_index]][cut_parameters[cut_index]][cut][
                    "significance_error"
                ]
            )

        # convert significance values to string
        significance_values = str(significance_values)
        significance_errors = str(significance_errors)
        # make square brackets curly
        significance_values = significance_values.replace("[", "{")
        significance_values = significance_values.replace("]", "}")
        significance_errors = significance_errors.replace("[", "{")
        significance_errors = significance_errors.replace("]", "}")

        # convert cuts to string
        cuts = [float(cut) for cut in cuts]
        cuts = str(cuts)
        # make square brackets curly
        cuts = cuts.replace("[", "{")
        cuts = cuts.replace("]", "}")

        Title = "Significance vs {}".format(cut_parameters[cut_index])
        xLabel = cut_parameters[cut_index]
        yLabel = "Significance"

        args = "{}, {}, {}, {}, {}, {}, {}".format(
            '"' + filepath + '"',
            '"' + Title + '"',
            '"' + xLabel + '"',
            '"' + yLabel + '"',
            significance_values,
            cuts,
            significance_errors,
        )

        result = subprocess.run(
            ["root", "-l", "-b", "-q", "MACROS/SignificancePlot.C({})".format(args)],
            stdout=subprocess.PIPE,
        )
        err_count += result.returncode

        # warn if error
        if result.returncode != 0:
            print("WARN: Error running MACROS/SignificancePlot.C")

    if err_count != 0:
        print("Script finished with {} error(s)".format(err_count))
    else:
        print("Script finished successfully")
