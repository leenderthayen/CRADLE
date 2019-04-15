# CRADLE++

Customisable RAdioactive Decay for Low Energy Particle Physics: A C++ event generator

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

* [BOOST](https://www.boost.org/) - Linear algebra rotuines, and the `program_options` part of the library
* [GSL](https://www.gnu.org/software/gsl/) - Used for the calculation of the calculation of the complex Gamma function in the Fermi function
* [Geant4 Data](http://geant4.web.cern.ch/support/download) - G4PhotonEvaporation and G4RadioactiveDecay files are used internally

### Installing

Installing is done using [CMake](https://cmake.org/) using the supplied `CMakeLists.txt` file

Perform an out of source build, for example:

```
mkdir build
cd build && cmake ..
make
```

## Authors

* **Leendert Hayen** - *KU Leuven*

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
