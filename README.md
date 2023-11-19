# Instructions

## MACROS
Contains ROOT macros which can be executed using `root MacroName.C` or at a root prompt using `.x MacroName.C`.

## lib
Contains root libraries for use in macros or independently using `.L LibName.cpp` at a root prompt. They need to be compiled before they will work with the MACROS that require them. They can be compiled together using `python3 lib_compile.py`.

## data
Contains all data relevant for running code in `lib` and `MACROS`. The relevant data or MC files are found in `data/V0Data/` and `data/V0MC/`.

## scripts
Contains any scripts used for batch tasks.

## output
Ideally all code that produce usefull data should store it here in a sensible subfolder.


## Useful server commands
Type cc7
Start enviroment of O2 Physics software (eprexa):
alienv enter O2Physics/latest-master-o2

if on personal server use: alienv enter O2Physics/latest-8621ff625d-o2

if on epldt003: alienv enter O2Physics/latest-d9ac9d44d3-o2
