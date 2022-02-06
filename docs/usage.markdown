---
layout: default 
---
# Usage

---

### Running executable 

After the successful [building]() of the project, one may run EpIC executable in the following way:
```shell
./bin/epic --seed=SEED --scenario=SCENARIO_PATH
```
where `SEED` is the random seed (unsigned integer) to be used in the initialisation of modules that deal with random numbers, and `SCENARIO_PATH` is the relative or absolute path to the scenario containing all options used in the generation. 

### Structure of XML scenario

EpIC scenarios are written in XML markup language. The general structure of single scenario is the following:
```xml
<!-- XML header -->
<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>

<!-- Definition of scenario -->
<!-- For bookkeeping it includes date and description -->
<scenario date="2022-01-01" description="My first scenario">

<!-- Selection of service and its method-->
<task service="DVCSGeneratorService" method="generate">

  <!-- General configuration -->
  <general_configuration>
  ...
  </general_configuration>

  <!-- Selection of kinematic ranges -->
  <kinematic_range>
  ...
  </kinematic_range>

  <!-- Indication of experimental conditions -->
  <experimental_conditions>
  ...
  </experimental_conditions>
 
  <!-- Configuration of ProcessModule -->
  <computation_configuration>
  ...
  </computation_configuration>

  <!-- Configuration of GeneratorModule -->
  <generator_configuration>
  ...
  </generator_configuration>

  <!-- Configuration of KinematicModule -->
  <kinematic_configuration>
   ...
  </kinematic_configuration>

  <!-- Configuration of RCModule -->
  <rc_configuration>
  ...
   </rc_configuration>

  <!-- Configuration of WriterModule -->
  <writer_configuration>
  ...
  </writer_configuration>

</task>
</scenario>
```

Here, only blocks containing specific information are shown. Each block is described in the following, for DVCS case. More example can be found in [`data/examples`](https://github.com/pawelsznajder/epic/tree/main/data/examples) directory of the project.

### General configuration of EpIC

```xml
<general_configuration>

  <!-- Number of events to be generated -->
  <param name="number_of_events" value="10" />
    
  <!-- Subprocess. Possible values: "DVCS" (pure DVCS), "BH" (pure Bethe-Heitler), "DVCS|BH" (pure DVCS and BH), "DVCS|BH|INT" (pure DVCS, BH and interference between both) -->
  <param name="subprocess_type" value="DVCS" />
  
</general_configuration>
```

### Selection of kinematic ranges

```xml
<kinematic_range>

  <!-- Range of $\xBj$ (Bjorken variable)-->
  <param name="range_xB" value="0.|1." />
    
  <!-- Range of $t$ --> 
  <param name="range_t" value="-1.|0." />
    
  <!-- Range of $Q^{2}$ -->
  <param name="range_Q2" value="1.|10." />
    
  <!-- Range of $\phi$ -->
  <param name="range_phi" value="0.|2*pi" />
    
  <!-- Range of $\phi_{S}$ -->
  <param name="range_phiS" value="0.|2*pi" />
    
  <!-- Range of $y$ -->
  <param name="range_y" value="0.01|0.95" />
  
</kinematic_range>
```

### Experimental conditions

```xml
<experimental_conditions>

  <!-- Energy of lepton beam -->
  <param name="lepton_energy" value="5." />

  <!-- Type of lepton beam, here electron -->
  <param name="lepton_type" value="e-" />
  
  <!-- Polarisation of lepton beam -->
  <param name="lepton_helicity" value="1" />
  
  <!-- Energy of hadron beam -->
  <!-- For target in rest frame use: value="fixed\_target" -->
  <param name="hadron_energy" value="10." />
  
  <!-- Type of hadron beam, here proton -->
  <param name="hadron_type" value="p" />
  
  <!-- Polarisation of hadron beam -->
  <!-- Possible values:  "0|0|0" for unpolarised target, "0|0|$\pm 1$" for long. polarised target, "$\pm 1$|0|0" or "0|$\pm 1$|0" for trans. polarised target -->
  <param name="hadron_polarisation" value="0.|0.|0." />
  
</experimental_conditions>
```

### Configuration of PARTONS `ProcessModule` for the evaluation of Born cross-section

```xml
<computation_configuration>

  <!-- Selection of module -->
  <!-- DVCSProcessBMJ12 module encodes BH, DVCS and interference Born cross-sections according to Refs. \cite{Belitsky:2001ns} and \cite{Belitsky:2012ch} -->
  <module type="DVCSProcessModule" name="DVCSProcessBMJ12">

    <!-- Selection of module used for evaluation of factorisation and renormalisation scales from DVCS kinematics -->
    <!-- DVCSScalesQ2Multiplier module identifies both scales as $Q^2$ variable -->
    <module type="DVCSScalesModule" name="DVCSScalesQ2Multiplier">
    </module>

    <!-- Selection of module used for evaluation of GPD skewness variable, $\xi$, from DVCS kinematics -->
    <!-- DVCSXiConverterXBToXi module use $\xi = \xBj / (2 - \xBj)$ -->
    <module type="DVCSXiConverterModule" name="DVCSXiConverterXBToXi">
    </module>

    <!-- Selection of module used for evaluation DVCS Compton form factors (CFFs) -->
    <!-- DVCSCFFCMILOU3DTables module does not evaluate CFFs from a GPD model during EpIC run-time,  instead it uses look-up tables of CFFs evaluated beforehand with a given GPD module. A number of such look-up tables are issued with EpIC -->
    <module type="DVCSConvolCoeffFunctionModule" name="DVCSCFFCMILOU3DTables">

      <!-- pQCD order of evaluation -->
      <param name="qcd_order_type" value="LO" />
  
      <!-- Path to look-up table, here the one based on GK GPD model -->
      <param name="cff_set_file" value="PATH/epic/data/ DVCSCFFCMILOU3DTables/tables_GK.root" />
</module>
  
  </module>
</computation_configuration>
```

### Configuration of `EventGeneratorModule` for generation of kinematic configurations 

```xml
<generator_configuration>

  <!-- Selection of module -->
  <!-- EventGeneratorFOAM module uses mini-FOAM library \cite{Jadach:2005ex} that is issued with ROOT \cite{Brun:1997pa} -->
  <module type="EventGeneratorModule" name="EventGeneratorFOAM">
  
    <!-- Parameters of FOAM algorithm -->

    <!-- Maximum number of cells -->
    <param name="nCells" value="10000" />
    
    <!--Number of MC events when exploring a cell -->
    <param name="nSamples" value="2000" />
    
    <!-- Number of bins in edge histogram for a cell -->
    <param name="nBins" value="2000" />
    
    <!-- State of FOAM after the initialisation will be saved in PATH/state.root. To use this file in other EpIC run, skipping the initialisation in that run, use read\_state\_file\_path option -->
    <param name="save_state_file_path" value="PATH/state.root" />
  </module>
</generator_configuration>
```

### Configuration of `KinematicModule` module for evaluation of four-momenta from kinematic configurations

```xml
<kinematic_configuration>

  <!-- Selection of module -->
  <!-- DVCSKinematicDefault module provides default evaluation of four-momenta from DVCS kinematics -->
  <module type="DVCSKinematicModule" name="DVCSKinematicDefault">
  </module>
</kinematic_configuration>
```

### Configuration of `RCModule` module for estimation of radiative corrections 

```xml
<rc_configuration>

  <!-- Selection of module -->
  <!-- DVCSRCCollinear module provides evaluation of initial and final state radiative corrections from lepton lines using collinear approximation, see Sect. \ref{sec:rc} for more details -->
  <module type="DVCSRCModule" name="DVCSRCCollinear">
  </module>
</rc_configuration>
```

### Configuration of `WriterModule` module for creation and writing event records to output file 

```xml
<writer_configuration>

  <!-- Selection of module -->
  <!-- WriterHepMC3 module is used to save event records in HepMC3 format \cite{Buckley:2019xhk} -->
  <module type="WriterModule" name="WriterHepMC3">
  
    <!-- Path to output file containing event records -->
    <param name="output_file_path" value="test.txt" />
    
    <!-- Format of output file containing event records: "ascii" for text format, or "root" for binary ROOT format-->
    <param name="HepMC3_writer_type" value="ascii" />
  </module>
</writer_configuration>
```
