import numpy as np
import matplotlib.pyplot as plt
import json
import subprocess

cut_parameters = ["dcanegtopv", "dcapostopv", "v0cospa", "dcav0dau", "v0radius"]

V0_names = ["K0Data", "LambdaData", "K0MC", "LambdaMC"]

x_labels = [
    "#bf{dcanegtopv cut values / cm}",
    "#bf{dcapostopv cut values / cm}",
    "#bf{v0cospa cut values}",
    "#bf{dcav0dau cut values / #sigma}",
    "#bf{v0radius cut values / cm}",
]

# min and max values for each cut parameter
cut_ranges = {
    "K0Data": {
        "dcanegtopv": [0, 0.25],
        "dcapostopv": [0, 0.25],
        "v0cospa": [0.985, 1],
        "dcav0dau": [0, 1],
        "v0radius": [0, 0.875],
    },
}


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

    # filter using the cut ranges
    data_significance_values = np.array(data_significance_values)
    data_significance_errors = np.array(data_significance_errors)
    data_signal_values = np.array(data_signal_values)
    data_signal_errors = np.array(data_signal_errors)
    MC_significance_values = np.array(MC_significance_values)
    MC_significance_errors = np.array(MC_significance_errors)
    MC_signal_values = np.array(MC_signal_values)
    MC_signal_errors = np.array(MC_signal_errors)
    cuts = np.array(cuts)
    print(cuts)
    # filter using the cut ranges
    data_significance_values = data_significance_values[
        (cuts > cut_ranges[V0_names[V0_index]][cut_parameters[cut_index]][0])
        & (cuts < cut_ranges[V0_names[V0_index]][cut_parameters[cut_index]][1])
    ]
    data_significance_errors = data_significance_errors[
        (cuts > cut_ranges[V0_names[V0_index]][cut_parameters[cut_index]][0])
        & (cuts < cut_ranges[V0_names[V0_index]][cut_parameters[cut_index]][1])
    ]
    data_signal_values = data_signal_values[
        (cuts > cut_ranges[V0_names[V0_index]][cut_parameters[cut_index]][0])
        & (cuts < cut_ranges[V0_names[V0_index]][cut_parameters[cut_index]][1])
    ]
    data_signal_errors = data_signal_errors[
        (cuts > cut_ranges[V0_names[V0_index]][cut_parameters[cut_index]][0])
        & (cuts < cut_ranges[V0_names[V0_index]][cut_parameters[cut_index]][1])
    ]
    MC_significance_values = MC_significance_values[
        (cuts > cut_ranges[V0_names[V0_index]][cut_parameters[cut_index]][0])
        & (cuts < cut_ranges[V0_names[V0_index]][cut_parameters[cut_index]][1])
    ]
    MC_significance_errors = MC_significance_errors[
        (cuts > cut_ranges[V0_names[V0_index]][cut_parameters[cut_index]][0])
        & (cuts < cut_ranges[V0_names[V0_index]][cut_parameters[cut_index]][1])
    ]
    MC_signal_values = MC_signal_values[
        (cuts > cut_ranges[V0_names[V0_index]][cut_parameters[cut_index]][0])
        & (cuts < cut_ranges[V0_names[V0_index]][cut_parameters[cut_index]][1])
    ]
    MC_signal_errors = MC_signal_errors[
        (cuts > cut_ranges[V0_names[V0_index]][cut_parameters[cut_index]][0])
        & (cuts < cut_ranges[V0_names[V0_index]][cut_parameters[cut_index]][1])
    ]
    cuts = cuts[
        (cuts > cut_ranges[V0_names[V0_index]][cut_parameters[cut_index]][0])
        & (cuts < cut_ranges[V0_names[V0_index]][cut_parameters[cut_index]][1])
    ]
    print(cuts)

    # flip the arrays if cut_index is 3
    if cut_index == 3:
        data_significance_values = np.flip(data_significance_values)
        data_significance_errors = np.flip(data_significance_errors)
        data_signal_values = np.flip(data_signal_values)
        data_signal_errors = np.flip(data_signal_errors)
        MC_significance_values = np.flip(MC_significance_values)
        MC_significance_errors = np.flip(MC_significance_errors)
        MC_signal_values = np.flip(MC_signal_values)
        MC_signal_errors = np.flip(MC_signal_errors)
        cuts = np.flip(cuts)

    # convert significance values to string
    data_significance_values = str(list(data_significance_values))
    data_significance_errors = str(list(data_significance_errors))
    data_signal_values = str(list(data_signal_values))
    data_signal_errors = str(list(data_signal_errors))
    MC_significance_values = str(list(MC_significance_values))
    MC_significance_errors = str(list(MC_significance_errors))
    MC_signal_values = str(list(MC_signal_values))
    MC_signal_errors = str(list(MC_signal_errors))
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

    Title = "#bf{{Significance vs {}}}".format(cut_parameters[cut_index])
    xLabel = x_labels[cut_index]
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
