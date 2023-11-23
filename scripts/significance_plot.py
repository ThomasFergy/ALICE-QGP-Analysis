import numpy as np
import matplotlib.pyplot as plt
import json
import subprocess

cut_parameters = ["dcanegtopv", "dcapostopv", "v0cospa", "dcav0dau", "v0radius"]

V0_names = ["K0Data", "LambdaData", "K0MC", "LambdaMC"]


def make_plot(V0_index, cut_index):
    if V0_index > 1:
        return
    # get cut values
    with open("output/significance.json", "r") as f:
        results = json.load(f)

    data_significance_values = []
    data_significance_errors = []
    data_signal_values = []
    data_signal_errors = []
    MC_significance_values = []
    MC_significance_errors = []
    MC_signal_values = []
    MC_signal_errors = []
    cuts = []
    for cut in results[V0_names[V0_index]][cut_parameters[cut_index]]:
        cuts.append(float(cut))
        data_significance_values.append(
            results[V0_names[V0_index]][cut_parameters[cut_index]][cut]["significance"]
        )
        data_significance_errors.append(
            results[V0_names[V0_index]][cut_parameters[cut_index]][cut][
                "significance_error"
            ]
        )
        data_signal_values.append(
            results[V0_names[V0_index]][cut_parameters[cut_index]][cut]["signal"]
        )
        data_signal_errors.append(
            results[V0_names[V0_index]][cut_parameters[cut_index]][cut]["signal_error"]
        )

        MC_significance_values.append(
            results[V0_names[V0_index + 2]][cut_parameters[cut_index]][cut][
                "significance"
            ]
        )
        MC_significance_errors.append(
            results[V0_names[V0_index + 2]][cut_parameters[cut_index]][cut][
                "significance_error"
            ]
        )
        MC_signal_values.append(
            results[V0_names[V0_index + 2]][cut_parameters[cut_index]][cut]["signal"]
        )
        MC_signal_errors.append(
            results[V0_names[V0_index + 2]][cut_parameters[cut_index]][cut][
                "signal_error"
            ]
        )

    # convert significance values to string
    data_significance_values = str(data_significance_values)
    data_significance_errors = str(data_significance_errors)
    data_signal_values = str(data_signal_values)
    data_signal_errors = str(data_signal_errors)
    MC_significance_values = str(MC_significance_values)
    MC_significance_errors = str(MC_significance_errors)
    MC_signal_values = str(MC_signal_values)
    MC_signal_errors = str(MC_signal_errors)
    # make square brackets curly
    data_significance_values = data_significance_values.replace("[", "{")
    data_significance_values = data_significance_values.replace("]", "}")
    data_significance_errors = data_significance_errors.replace("[", "{")
    data_significance_errors = data_significance_errors.replace("]", "}")
    data_signal_values = data_signal_values.replace("[", "{")
    data_signal_values = data_signal_values.replace("]", "}")
    data_signal_errors = data_signal_errors.replace("[", "{")
    data_signal_errors = data_signal_errors.replace("]", "}")
    MC_significance_values = MC_significance_values.replace("[", "{")
    MC_significance_values = MC_significance_values.replace("]", "}")
    MC_significance_errors = MC_significance_errors.replace("[", "{")
    MC_significance_errors = MC_significance_errors.replace("]", "}")
    MC_signal_values = MC_signal_values.replace("[", "{")
    MC_signal_values = MC_signal_values.replace("]", "}")
    MC_signal_errors = MC_signal_errors.replace("[", "{")
    MC_signal_errors = MC_signal_errors.replace("]", "}")

    cuts = [float(cut) for cut in cuts]
    cuts = str(cuts)
    # make square brackets curly
    cuts = cuts.replace("[", "{")
    cuts = cuts.replace("]", "}")

    filepath = "output/figures/{}_significance_{}.pdf".format(
        V0_names[V0_index], cut_parameters[cut_index]
    )

    Title = "Significance vs {}".format(cut_parameters[cut_index])
    xLabel = cut_parameters[cut_index]
    yLabel = "Significance"

    args = "{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}".format(
        '"' + filepath + '"',
        '"' + Title + '"',
        '"' + xLabel + '"',
        '"' + yLabel + '"',
        data_significance_values,
        cuts,
        data_significance_errors,
        data_signal_values,
        MC_signal_values,
        data_signal_errors,
        MC_signal_errors,
    )

    result = subprocess.run(
        [
            "root",
            "-l",
            "-b",
            "-q",
            "MACROS/SignificanceWithSignalPlot.C({})".format(args),
        ],
        stdout=subprocess.PIPE,
    )


if __name__ == "__main__":
    V0_index = 0
    cut_index = 0
    make_plot(V0_index, cut_index)