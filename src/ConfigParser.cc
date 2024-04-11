#include "CRADLE/ConfigParser.hh"

#include "CLI11.hpp"

namespace CRADLE {

  void parse(CLI::App& app, int argc, const char** argv) {
      if (argc == 0) {
        argc = 1;
        const char* _argv[1] = {"coupling"};
        argv = _argv;
      }
      try {
        app.parse(argc, argv);
      } catch (const CLI::ParseError &e) {
        app.exit(e);
      }
  }

  void SetCmdOptions (CLI::App& app, NuclearOptions& cmdOptions) {

    CLI::App* cmd = app.add_subcommand("Nucleus","This is the initial nucleus options command.")->ignore_case()->required();
    cmd->add_option("-n,--name", cmdOptions.Name, "Name of initial particle.")->required();
    cmd->add_option("-Z,--charge", cmdOptions.Charge, "Charge as multiple of proton charge.");
    cmd->add_option("-A,--mass", cmdOptions.Nucleons, "Mass number.");
    cmd->add_option("-e,--energy", cmdOptions.Energy, "Excitation energy of initial state.");

    cmd->callback([&]() {
      std::cout << "Parsing initial nucleus " << cmdOptions.Name << std::endl;
    });

  }

  void SetGeneralOptions (CLI::App& app, General& general) {
    CLI::App* comp = app.add_subcommand("General", "This is the general subcommand")->ignore_case();
    comp->add_option("-v,--Verbosity", general.Verbosity, "Verbosity settings");
    comp->add_option("-w,--Verbosity_file", general.Verbosity_file, "Verbosity File settings");
    comp->add_option("-l,--loop", general.Loop, "Number of events to generate.");
    comp->add_option("-t,--threads", general.Threads, "Number of threads (2 x #CPU).");
    comp->add_option("-o,--output", general.Output, "Name of the output file.");
  }

  void SetCouplingConstants (CLI::App& app, CouplingConstants& couplingConstants) {
    CLI::App* coupling = app.add_subcommand("Coupling", "This is the coupling subcommand")->ignore_case();
    coupling->add_option("--CV", couplingConstants.CV, "Vector coupling constant.");
    coupling->add_option("--CT", couplingConstants.CT, "Tensor coupling constant.");
    coupling->add_option("--CS", couplingConstants.CS, "Scalar coupling constant.");
    coupling->add_option("--CA", couplingConstants.CA, "Axial coupling constant.");
    coupling->add_option("--CVP", couplingConstants.CVP, "Vector prime coupling constant.");
    coupling->add_option("--CTP", couplingConstants.CTP, "Tensor prime coupling constant.");
    coupling->add_option("--CSP", couplingConstants.CSP, "Scalar prime coupling constant.");
    coupling->add_option("--CAP", couplingConstants.CAP, "Axial prime coupling constant.");
    coupling->add_option("--a", couplingConstants.a, "Angular correlation coefficient.");
    coupling->add_option("--b", couplingConstants.b, "Fierz term.");
  }

  void SetCuts (CLI::App& app, Cuts& cuts) {
    CLI::App* comp = app.add_subcommand("Cuts", "This is the cuts subcommand")->ignore_case();
    comp->add_option("--distance", cuts.Distance, "")->ignore_case();
    comp->add_option("--lifetime", cuts.Lifetime, "")->ignore_case();
    comp->add_option("--energy", cuts.Energy, "")->ignore_case();

  }

  void SetBetaDecayOptions (CLI::App& app, BetaDecay& betaDecay) {
    CLI::App* comp = app.add_subcommand("BetaDecay", "This is the beta decay subcommand")->ignore_case();
    comp->add_option("--Default", betaDecay.Default, "");
    comp->add_option("--FermiFunction", betaDecay.FermiFunction, "");
    comp->add_option("--PolarisationX", betaDecay.PolarisationX, "");
    comp->add_option("--PolarisationY", betaDecay.PolarisationY, "");
    comp->add_option("--PolarisationZ", betaDecay.PolarisationZ, "");

  }

  void SetEnvironmentOptions (CLI::App& app, EnvOptions& envOptions) {
    app.add_option("--AMEdata", envOptions.AMEdata, "AME2020 file location")->envname("AMEdata");
    app.add_option("--Gammadata", envOptions.Gammadata, "")->envname("Gammadata");
    app.add_option("--Radiationdata", envOptions.Radiationdata, "")->envname("Radiationdata");
  }

  ConfigOptions ParseOptions(std::string filename, int argc, const char** argv) {
    ConfigOptions configOptions;

    CLI::App app{"CRADLE++ App"};
    app.allow_extras(true);
    app.allow_config_extras(true);
    app.set_config("-c", filename);

    SetCmdOptions(app,configOptions.nuclearOptions);
    SetGeneralOptions(app,configOptions.general);
    SetCouplingConstants(app,configOptions.couplingConstants);
    SetCuts(app,configOptions.cuts);
    SetBetaDecayOptions(app,configOptions.betaDecay);
    SetEnvironmentOptions(app,configOptions.envOptions);

    parse(app, argc, argv);

    return configOptions;
  }

}//end of namespace CRADLE
