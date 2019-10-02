#include "CRADLE/ConfigParser.h"

#include "CLI11.hpp"

namespace NME {
  void parse(CLI::App& app) {
    int argc = 1;
    char* argv[argc] = {"coupling"};

    try {
      app.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
      app.exit(e);
    }
  }

  CmdOptions ParseCmdOptions (std::string filename) {
    CmdOptions cmdOptions;

    CLI::App app{"Command-line Options"};
    app.add_option("-n,--name", cmdOptions.Name, "Name of initial particle.");
    app.add_option("-z,--charge", cmdOptions.Charge, "Charge as multiple of proton charge.");
    app.add_option("-n,--nucleons", cmdOptions.Nucleons, "Number of nucleons.");
    app.add_option("-e,--energy", cmdOptions.Energy, "Excitation energy of initial state.");
    app.add_option("-l,--loop", cmdOptions.Loop, "Number of events to generate.");
    app.add_option("-t,--threads", cmdOptions.Threads, "Number of threads (2 x #CPU).");
    app.add_option("-o,--output", cmdOptions.Output, "Name of the output file.");
    app.add_option("-u,--usebsg", cmdOptions.Usebsg, "Choose whether or not to use BSG.");

    parse(app);

    return cmdOptions;
  }

  General ParseGeneralOptions (std::string filename) {
    General general;

    CLI::App app{"General options"};
    app.allow_config_extras(true);
    app.set_config("-c", filename);
    CLI::App* comp = app.add_subcommand("General", "This is the general subcommand");
    comp->add_option("--Verbosity", general.Verbosity, "Verbosity settings");

    parse(app);

    return general;
  }

  CouplingConstants ParseCouplingConstants (std::string filename) {
    CouplingConstants couplingConstants;

    CLI::App app{"Coupling constants"};
    app.allow_config_extras(true);
    app.set_config("-c", filename);
    CLI::App* coupling = app.add_subcommand("Coupling", "This is the coupling subcommand");
    coupling->add_option("--CV", couplingConstants.CV, "Vector coupling constant.");
    coupling->add_option("--CT", couplingConstants.CT, "Tensor coupling constant.");
    coupling->add_option("--CS", couplingConstants.CS, "Scalar coupling constant.");
    coupling->add_option("--CA", couplingConstants.CA, "Axial coupling constant.");

    parse(app);

    return couplingConstants;
  }

  Cuts ParseCuts (std::string filename) {
    Cuts cuts;

    CLI::App app{"Cuts"};
    app.allow_config_extras(true);
    app.set_config("-c", filename);
    CLI::App* comp = app.add_subcommand("Cuts", "This is the cuts subcommand");
    comp->add_option("--Distance", cuts.Distance, "");
    comp->add_option("--Lifetime", cuts.Lifetime, "");
    comp->add_option("--Energy", cuts.Energy, "");

    parse(app);

    return cuts;
  }

  BetaDecay ParseBetaDecayOptions (std::string filename) {
    BetaDecay betaDecay;

    CLI::App app{"BetaDecay"};
    app.allow_config_extras(true);
    app.set_config("-c", filename);
    CLI::App* comp = app.add_subcommand("BetaDecay", "This is the beta decay subcommand");
    comp->add_option("--Default", betaDecay.Default, "");
    comp->add_option("--FermiFunction", betaDecay.FermiFunction, "");
    comp->add_option("--PolarisationX", betaDecay.PolarisationX, "");
    comp->add_option("--PolarisationY", betaDecay.PolarisationY, "");
    comp->add_option("--PolarisationZ", betaDecay.PolarisationZ, "");

    parse(app);

    return betaDecay;
  }

  EnvOptions ParseEnvironmentOptions (std::string filename) {
    EnvOptions envOptions;

    CLI::App app{"EnvOptions"};

    app.add_option("--Gammadata", envOptions.Gammadata, "")->envname("Gammadata");
    app.add_option("--Radiationdata", envOptions.Radiationdata, "")->envname("Radiationdata");
    parse(app);

    return envOptions;
  }

  ConfigOptions ParseConfigFile(std::string filename) {
    ConfigOptions configOptions;

    configOptions.cmdOptions = ParseCmdOptions(filename);
    configOptions.general = ParseGeneralOptions(filename);
    configOptions.couplingConstants = ParseCouplingConstants(filename);
    configOptions.cuts = ParseCuts(filename);
    configOptions.betaDecay = ParseBetaDecayOptions(filename);
    configOptions.envOptions = ParseEnvironmentOptions(filename);

    return configOptions;
  }
}//end of NME namespace
