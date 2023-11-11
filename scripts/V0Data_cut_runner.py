'''
Script to calculate the significance of the V0s
--- (requires being in the alienv environment before running) ---

Method:
  Step 1: Set the cut values to be used and on which parameter in the relevant json file
  Step 2: Run the run_step0.sh script to produce the output files
  Step 3: Rename the output file to avoid overwriting
  Step 4: go back to step 1 and repeat until you have applied all the cuts you want
'''

import os
import sys
import json

cut_parameters = ["dcanegtopv","dcapostopv", "v0cospa", "dcav0dau", "v0radius"]

default_cut_values = [0.0, 0.0, 0.9, 1, 0.899999976]

def set_cut_value(json_file, cut_name_index, cut_value):
    '''
    Function to set the cut value in the json file
    '''
    with open(json_file, 'r') as f:
        data = json.load(f)
    data['lambdakzero-builder'][cut_parameters[cut_name_index]] = str(cut_value)
    with open(json_file, 'w') as f:
        json.dump(data, f, indent=2)


if __name__ == "__main__":

  # set default cut values
  for i in range(len(cut_parameters)):
    set_cut_value('data/V0Data/json_strangenesstutorial.json', i, default_cut_values[i])

  # Set the cut value in the json file
  # 0 = dcanegtopv
  # 1 = dcapostopv
  # 2 = v0cospa
  # 3 = dcav0dau
  # 4 = v0radius

  par_index = 0
  cut_values = [0, 0.02, 0.04, 0.06, 0.08]

  for cut_value in cut_values:
    set_cut_value('data/V0Data/json_strangenesstutorial.json', par_index, cut_value)
  
    # Apply the cut (requires being in the alienv environment before running)
    os.chdir("data/V0Data")
    os.system("./run_step0.sh")
    os.chdir("../../")

    # Rename the output file to avoid overwriting
    os.system("mv data/V0Data/AnalysisResults.root output/V0Data/AnalysisResults_{}_{}.root".format(cut_parameters[par_index], cut_value))
