---
layout: default 
---

# Installation 

---

All files related to EpIC are stored in the repository hosted by [GitHub](https://github.com/pawelsznajder/epic), using [git](https://git-scm.com) version control system. To get a local copy of these files open your terminal and execute: 
```shell
git clone https://github.com/pawelsznajder/epic.git
```

EpIC is written in C++ following C++11 standard. It requires the following external libraries:
* [PARTONS](http://partons.cea.fr)
* [Qt](https://www.qt.io) (version 4 or 5)
* [GSL](https://www.gnu.org/software/gsl)
* [ROOT](https://root.cern)
* [HepMC3](https://gitlab.cern.ch/hepmc/HepMC3)

Other libraries may be required by those listed above.


Both the compilation of source code and the linking against external libraries is managed by [CMake](https://cmake.org). To build the project:
* go to `build` located in the root directory of the project:
```shell
cd build
```
* configure:
```shell
cmake ..
```
* build:
```shell
make
```

The successful building creates `epic` executable in `bin` folder located at the same level as `build`. 

Note that local installation of external libraries may be pointed out to [CMake](https://cmake.org) with `-D` option. For instance:
```shell
cmake -DHepMC3_DIR=ABSOLUTE_PATH ..
```
will tell [CMake](https://cmake.org) to look for [HepMC3](https://gitlab.cern.ch/hepmc/HepMC3) library in `ABSOLUTE_PATH` folder. This folder should, in this particular case, include `HepMC3Config.cmake` file, which is issued with the installation of [HepMC3](https://gitlab.cern.ch/hepmc/HepMC3).