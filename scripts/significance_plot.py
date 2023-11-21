import numpy as np
import matplotlib.pyplot as plt
import json

cut_parameters = ["dcanegtopv", "dcapostopv", "v0cospa", "dcav0dau", "v0radius"]

V0_names = ["K0Data", "LambdaData", "K0MC", "LambdaMC"]

# Load data from json
with open("output/significance.json") as f:
    data = json.load(f)

V0_index = 0
cut_index = 0

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
        results[V0_names[V0_index + 2]][cut_parameters[cut_index]][cut]["significance"]
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
        results[V0_names[V0_index + 2]][cut_parameters[cut_index]][cut]["signal_error"]
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
