#ifndef CRADLE_CONFIG_CONTAINER_H
#define CRADLE_CONFIG_CONTAINER_H

#include "CLI11.hpp"

#include <string>

namespace CRADLE {

struct NuclearOptions {
  std::string Name = "";
  int Charge = 0;
  int Nucleons = 0;
  double Energy = 0.0;
};

struct General {
  int Verbosity = 1;
  int Loop = 0;
  int Threads = 1;
  std::string Output = "output.txt";
};

struct CouplingConstants {
  double CS = 0.0;
  double CSP = 0.0;
  double CV = 1.0;
  double CVP = 0.0;
  double CT = 0.0;
  double CTP = 0.0;
  double CA = 1.2754;
  double CAP = 1.2754;
};

struct Cuts {
  double Distance = 1.E10;
  double Lifetime = 1.E10;
  double Energy = 1.E10;
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

}

#endif
