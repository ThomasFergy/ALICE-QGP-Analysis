# Instructions

## Getting Started:

There is some extra set up beyond cloning this repo required to get everything up and running. First there are some files missing in `data/V0Data/` and `data/V0MC/` which are excluded due to being device specific or being too large to keep in this repo.
  
- `data/V0Data/`: requires `locCCDB/` and `AO2D.root`. These can be symbolic links to the relevant place on your system.

- `data/V0MC/`: requires `locCCDB/` and `aodmcs.txt`. These can be symbolic links to the relevant place on your system.


## Simple Significance Plot Tutorial

In this example we will go from nothing to having a significance plot for dcapostopv on the K0.

1. First we will need to apply the cuts over the range that we want and on the files we want. The scripts `V0Data_cut_runner.py` or `V0MC_cut_runner.py` allow us to do this. To configure the cuts and default cuts you must edit `scripts/cut_config.json`.
   1. We could select: `"dcapostopv": {
    "UpperLimit": 0.2,
    "LowerLimit": 0.0,
    "step": 0.005
  }`.
   2. and also select: `"cuts_to_apply": {
    "dcanegtopv": false,
    "dcapostopv": true,
    "v0cospa": false,
    "dcav0dau": false,
    "v0radius": false
  },`
   
   Make sure you save these changes before step 2.

2. From the root directory of this repo run `python3 scripts/V0Data_cut_runner.py` (IMPORTANT: Must be in alienv O2Physics environment). This will populate `output/V0Data/` with files of the form `AnalysisResults_dcanegtopv_0.05.root`, which contain the parameter and cut value. (This is quite slow)

3. To get to a significance plot for these cuts we need to fit each of the K0 mass plots in the `.root` files produced in step 2. The script `significance_runner.py` allows us to do this. There are two variables to edit at the top of this file.
   1. `isMC`: Boolean to choose Data or MC.
   2. `V0_index`: 0 = K0, 1 = Lambda.
   
   For this example we would choose `isMC = False` and `V0_index = 0`. Make sure you save these changes before step 4.

4. From the root directory of this repo run `python3 scripts/significance_runner.py`. This will populate `figures/batch_mass_plots/` with figures with names like `K0Data_dcapostopv_0.05.pdf` (This is mostly important just to check that the fits are behaving as expected). After this has finished running the results will be saved in `output/significance.json` and the plot `output/figures/K0Data_significance_dcapostopv.pdf` will be saved. You may get some warnings like `WARN: No files found for cut parameter v0cospa`. These are ok to ignore for now as this script tries all parameters but you may not have all the data files yet.


#### Notes:
If a specific cut has already been performed before (meaning the output `.root` file already exists) then this is skipped as there is no need to recreate the file and therefore we can save time.

If a specific significance calculation has already been done before (meaning it already exists in `significance.json`) it will be skipped to save time as well.

If the python scripts are failing to run the MACROS: `MACROS/SignificanceFit.C` and `MACROS/SignificancePlot.C` then you may not have compiled the c++ files in `lib/`. This is done by, from the root directory of this repo, running `python3 lib/lib_compile.py`

Process for running MC cuts is a bit different but should be similar for significance. MC cuts are very very slow. For 80 cuts each for two parameters this takes many hours.


## File Structure:

### `data/`
Contains all data relevant for running code in `lib` and `MACROS`. The relevant data or MC files are found in `data/V0Data/` and `data/V0MC/`.

### `lib/`
Contains root libraries for use in macros or independently using `.L LibName.cpp` at a root prompt. They need to be compiled before they will work with the MACROS that require them. They can be compiled together using `python3 lib_compile.py`.

### `MACROS/`
Contains ROOT macros which can be executed using `root MacroName.C` or at a root prompt using `.x MacroName.C`.

### `output/`
Ideally all code that produce usefull data should store it here in a sensible subfolder.

### `scripts/`
Contains any scripts used for batch tasks.


## Useful server commands
Type cc7 (eprexa)

Start enviroment of O2 Physics software (eprexa):
- alienv enter O2Physics/latest-master-o2
- alienv enter O2Physics/latest-rl-o2

if on epldt003:
- alienv enter O2Physics/latest-d9ac9d44d3-o2
