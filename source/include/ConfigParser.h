#ifndef CRADLE_CONFIG_CONTAINER_H
#define CRADLE_CONFIG_CONTAINER_H

#include "PDS/Units/GlobalSystemOfUnits.h"
#include "CLI11.hpp"

#include <string>

struct NuclearOptions {
  std::string Name = "";
  int Charge = 1;
  int Nucleons = 1;
  double Energy = 0.0;
};

struct General {
  int Verbosity = 0;
  int Loop = 1;
  int Threads = 1;
  std::string Output = "output.txt";
  bool Usebsg = true;
};

struct CouplingConstants {
  double CS = 0.0;
  double CV = 1.0;
  double CT = 0.0;
  double CA = 1.2723;
};

struct Cuts {
  double Distance = 0.0;
  double Lifetime = 0.0;
  double Energy = 0.0;
};

struct BetaDecay {
  std::string Default = "";
  std::string FermiFunction = "";
  double PolarisationX;
  double PolarisationY;
  double PolarisationZ;
};

struct EnvOptions {
  std::string Gammadata;
  std::string Radiationdata;
};

struct ConfigOptions{
  NuclearOptions nuclearOptions;
  General general;
  CouplingConstants couplingConstants;
  Cuts cuts;
  BetaDecay betaDecay;
  EnvOptions envOptions;
};

ConfigOptions ParseOptions(std::string, int argc = 0, const char** argv = nullptr);

void SetCmdOptions(CLI::App&, NuclearOptions&);
void SetGeneralOptions(CLI::App&, General&);
void SetCouplingConstants(CLI::App&, CouplingConstants&);
void SetCuts(CLI::App&, Cuts&);
void SetBetaDecayOptions(CLI::App&, BetaDecay&);
void SetEnvironmentOptions(CLI::App&, EnvOptions&);

#endif
