import numpy as np
import matplotlib.pyplot as plt
import json
import subprocess

cut_parameters = ["dcanegtopv", "dcapostopv", "v0cospa", "dcav0dau", "v0radius"]
cut_parameters_labels = [
    "DCANegToPV",
    "DCAPosToPV",
    "V0CosPA",
    "DCAV0Dau",
    "V0Radius",
]

V0_names = ["K0Data", "LambdaData", "K0MC", "LambdaMC"]

x_labels = [
    "DCANegToPV cut value (cm)",
    "DCAPosToPV cut value (cm)",
    "V0CosPA cut value",
    "DCAV0Dau cut value (#sigma)",
    "V0Radius cut value (cm)",
]


cpp_convert = {True: "true", False: "false"}


def delete_values(array, indices):
    """
    Function to delete values from an array
    """
    return np.delete(array, indices)


def make_plot(
    V0_index, cut_index, cut_ranges, significance_ranges, fit_and_plot_line=False
):
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

    min_significance = significance_ranges[V0_names[V0_index]][
        cut_parameters[cut_index]
    ][0]
    max_significance = significance_ranges[V0_names[V0_index]][
        cut_parameters[cut_index]
    ][1]
    # find indicies of values greater than max_significance
    data_significance_indicies = np.where((data_significance_values > max_significance))

    # delete the values
    data_significance_values = delete_values(
        data_significance_values, data_significance_indicies
    )
    data_significance_errors = delete_values(
        data_significance_errors, data_significance_indicies
    )
    data_signal_values = delete_values(data_signal_values, data_significance_indicies)
    data_signal_errors = delete_values(data_signal_errors, data_significance_indicies)
    MC_significance_values = delete_values(
        MC_significance_values, data_significance_indicies
    )
    MC_significance_errors = delete_values(
        MC_significance_errors, data_significance_indicies
    )
    MC_signal_values = delete_values(MC_signal_values, data_significance_indicies)
    MC_signal_errors = delete_values(MC_signal_errors, data_significance_indicies)
    cuts = delete_values(cuts, data_significance_indicies)

    # find indicies of values less than min_significance
    data_significance_indicies = np.where((data_significance_values < min_significance))

    # delete the values
    data_significance_values = delete_values(
        data_significance_values, data_significance_indicies
    )
    data_significance_errors = delete_values(
        data_significance_errors, data_significance_indicies
    )
    data_signal_values = delete_values(data_signal_values, data_significance_indicies)
    data_signal_errors = delete_values(data_signal_errors, data_significance_indicies)
    MC_significance_values = delete_values(
        MC_significance_values, data_significance_indicies
    )
    MC_significance_errors = delete_values(
        MC_significance_errors, data_significance_indicies
    )
    MC_signal_values = delete_values(MC_signal_values, data_significance_indicies)
    MC_signal_errors = delete_values(MC_signal_errors, data_significance_indicies)
    cuts = delete_values(cuts, data_significance_indicies)

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

    Title = "{}".format(cut_parameters_labels[cut_index])
    xLabel = x_labels[cut_index]
    yLabel = "Significance"

    args = "{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}".format(
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
        cut_index,
        cpp_convert[fit_and_plot_line],
    )

    print(args)

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

    print(result.stdout)


if __name__ == "__main__":
    V0_index = 1
    cut_indices = [0, 1, 2, 3, 4]
    fit_and_plot_line = True
    # min and max values for each cut parameter
    cut_ranges = {
        "K0Data": {
            "dcanegtopv": [0, 0.25],
            "dcapostopv": [0, 0.25],
            # "v0cospa": [0.985, 1.0],
            "v0cospa": [0.985, 0.996],
            "dcav0dau": [0.1, 0.4],
            "v0radius": [0, 0.875],
        },
        "LambdaData": {
            "dcanegtopv": [0, 0.5],
            "dcapostopv": [0, 0.055],
            "v0cospa": [0, 0.9685],
            "dcav0dau": [0, 2],
            "v0radius": [0, 1],
        },
    }

    significance_ranges = {
        "K0Data": {
            "dcanegtopv": [0, 1000],
            "dcapostopv": [0, 1000],
            # "v0cospa": [0.985, 1.0],
            "v0cospa": [0, 130],
            "dcav0dau": [0, 1000],
            "v0radius": [0, 1000],
        },
        "LambdaData": {
            "dcanegtopv": [0, 130],
            "dcapostopv": [0, 130],
            "v0cospa": [0, 200],
            "dcav0dau": [0, 136],
            "v0radius": [0, 125],
        },
    }

    for cut_index in cut_indices:
        make_plot(
            V0_index,
            cut_index,
            cut_ranges,
            significance_ranges,
            fit_and_plot_line,
        )
