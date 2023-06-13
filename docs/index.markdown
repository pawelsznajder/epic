---
layout: default 
---

# About 

---

### What is EpIC?

EpIC is a modern and versatile Monte Carlo generator used in studying exclusive processes. These processes are sensitive to generalised parton distributions (GPDs), which describe the 3D partonic structure of hadrons in the language of quantum chromodynamics (QCD). EpIC has been developed to support the current and future experimental programmes, like [electron-ion collider (EIC)](https://www.bnl.gov/eic) to be constructed in Brookhaven National Laboratory.

EpIC is based on [PARTONS framework](http://partons.cea.fr), which provides the basic elements of its architecture. PARTONS is also used to evaluate Born cross-sections, which together with radiative corrections implemented in EpIC, are used to generate unweighted Monte Carlo events. 

Exclusive processes currently available in EpIC are:
* deeply virtual Compton scattering (DVCS) 
* time-like Compton scattering (TCS)
* deeply virtual meson production (DVMP), only &#960;<sup>0</sup> case
* double deeply virtual Compton scattering (DDVCS) 
* diphoton photoproduction (work in progress)

We refer to [PARTONS documentation of modules](http://partons.cea.fr/partons/doc/html/files.html) for information on theory developments (like GPD models) available for the evaluation of Born cross-sections. 

EpIC is written in C++. It utilities paradigms of the modular programming. Each step of the generation of Monte Carlo events is well distinguished from the others, allowing for the encapsulation of specific pieces of the code into easy-to-develop modules. The flow of data between the modules is supervised by services. 

This page provides user's guide, allowing physicists to use EpIC in real life analyses. More technical information, which is important for a better understanding of the architecture of EpIC, *e.g.* for the purpose of further developments, can be found in [this article](https://inspirehep.net/literature/2077191). The article also provides the official reference points to be used in publications. 

The code of the project is stored in [GitHub](https://github.com/pawelsznajder/epic). Doxygen documentation can be found [here](assets/doxygen/html/annotated.html).
