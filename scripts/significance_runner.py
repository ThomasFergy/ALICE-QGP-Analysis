import os
import sys
import json
import subprocess


isMC = True

if isMC:
  output_dir = "output/V0MC"
else:
  output_dir = "output/V0Data"

cut_parameters = ["dcanegtopv","dcapostopv", "v0cospa", "dcav0dau", "v0radius"]
  

def get_cut_values(dir, cut_parameter_index):
  files = os.listdir(dir)
  # get all files with the cut_parameter in the name
  files = [f for f in files if cut_parameters[cut_parameter_index] in f]
  cut_values = []
  # cut values are the last part of the file name after the underscore
  for f in files:
    cut_values.append(float(f.split("_")[-1][:-5]))
  cut_values.sort()
  return cut_values

if __name__ == "__main__":
  # 0 = dcanegtopv
  # 1 = dcapostopv
  # 2 = v0cospa
  # 3 = dcav0dau
  # 4 = v0radius
  cuts = get_cut_values(output_dir, 0)
  print(cuts)
