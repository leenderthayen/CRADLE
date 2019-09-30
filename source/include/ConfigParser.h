#ifndef CRADLE_CONFIG_CONTAINER_H
#define CRADLE_CONFIG_CONTAINER_H

#include "PDS/Units/GlobalSystemOfUnits.h"

#include <string>

namespace CRADLE {

  struct CmdOptions {
    std::string Name = "";
    int Charge = 1;
    int Nucleons = 1;
    double Energy = 0.0;
    int Loop = 1;
    int Threads = 1;
    std::string Output = "output.txt";
    bool Usebsg = true;
  };

  struct General {
    int Verbosity = 0;
  };

  struct CouplingConstants {
    double CS = 0.0;
    double CV = 1.0;
    double CT = 0.0;
    double CA = 1.0;
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
    CmdOptions cmdOptions;
    CouplingConstants couplingConstants;
    Cuts cuts;
    BetaDecay betaDecay;
    EnvOptions envOptions;
  };

  ConfigOptions ParseConfigFile(std::string);

  CmdOptions ParseCmdOptions(std::string);
  General ParseGeneralOptions(std::string);
  CouplingConstants ParseCouplingConstants(std::string);
  Cuts ParseCuts(std::string);
  BetaDecay ParseBetaDecayOptions(std::string);
  EnvOptions ParseEnvironmentOptions(std::string);

}

#endif
