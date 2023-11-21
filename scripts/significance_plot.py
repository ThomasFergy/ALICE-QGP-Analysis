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
MC_significance_values = []
MC_significance_errors = []
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

    MC_significance_values.append(
        results[V0_names[V0_index + 2]][cut_parameters[cut_index]][cut]["significance"]
    )
    MC_significance_errors.append(
        results[V0_names[V0_index + 2]][cut_parameters[cut_index]][cut][
            "significance_error"
        ]
    )

print(cuts)
