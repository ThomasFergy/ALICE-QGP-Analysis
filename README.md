# Production of multistrange particles at LHC from the ALICE experiment

Multistrange particles, Ξ- and Ω-, are very sensitive probes of the overall level of strangeness in a proton-proton collision [1,2]. It was proposed by J.D. Bjorken that in a proton-proton collision of sufficiently high multiplicity (number of particles produced in a collision) a Quark- Gluon Plasma (QGP) might be formed [3], but, till now, such very high multiplicity collisions have not been accessible. The LHC provides a new regime where much higher multiplicity collisions are seen to occur. Should a QGP be created, a thermal system is produced in which strangeness abundances could be much higher than in normal hadronic reactions [4], as has already been seen in heavy ion interactions. In this project, the relative production rates for multistrange particles will be examined as a function of multiplicity to look for possible changes from the values expected for hadronic interactions.

This project gives students practical experience of some of the methods, techniques and tools used in high-energy physics data analysis, as well as providing insight into state-of-the-art detector systems and the physics of the strong interaction. This project primarily involves analysing existing data and hence is a computing based project. The project will make use of software tools that have been developed by the ALICE collaboration and build on the ROOT C++ analysis framework. You will be provided with a working program for your analysis task but will have to make changes to it. Hence, you need to enjoy programming and thinking of ways to check that your code works the way you think it should.

Further reading:

1. J. Rafelski and B. Mueller, Phys. Rev. Lett. 48 (1982) 1066; Erratum Phys. Rev. Lett. 56, 2334 (1986); J.Rafelski and M. Petráň, Phys. Rev. C82 (2010) 011901
2. H. Caines J. Phys. G 32 (2006) 171
3. J.D. Bjorken, Fermilab Report Fermilab-Pub-82/59-THY (1982)
4. Z.L. Matthews, J. Phys. G. 37 (2010) 094048.


## Instructions

### MACROS
Contains ROOT macros which can be executed using `root MacroName.C` or at a root prompt using `.x MacroName.C`.

### lib
Contains root libraries for use in macros or independently using `.L LibName.cpp` at a root prompt. They need to be compiled before they will work with the MACROS that require them. They can be compiled together using `python3 lib_compile.py`.

### data
Contains all data relevant for running code in `lib` and `MACROS`. The relevant data or MC files are found in `data/V0Data/` and `data/V0MC/`.

### scripts
Contains any scripts used for batch tasks.

### output
Ideally all code that produce usefull data should store it here in a sensible subfolder.


### Useful eprexa commands
Type cc7
Start enviroment of O2 Physics software:
alienv enter O2Physics/latest-master-o2

if on personal server use: alienv enter O2Physics/latest-8621ff625d-o2