#include <string>
#include <iostream>

#include "CLI11.hpp"
#include "CRADLE/DecayManager.hh"

int main (int argc, const char* argv[]) {
  std::string iniFilename;
  std::string configFilename;
  std::string outputName = "output";

  CLI::App app{"CRADLE++ standalone"};
  app.add_option("-i,--input", iniFilename, "INI input file for transition information");
  app.add_option("-c,--config", configFilename, "INI config file for calculation information");
  app.add_option("-o,--output", outputName, "Name for file output. No extensions.");

  try {
    app.parse(argc, argv);
  } catch (const CLI::ParseError &e) {
    app.exit(e);
  }

  CRADLE::DecayManager& dm = CRADLE::DecayManager::GetInstance();
  dm.RegisterBasicParticles();
  dm.RegisterBasicDecayModes();
  dm.RegisterBasicSpectrumGenerators();
  bool success = dm.Initialise(configFilename, argc, argv);

  if (success) {
    dm.MainLoop();

    dm.ListRegisteredParticles();
  } else {
    std::cout << "Specify configuration file, isotope name, charge and number of nucleons. Use the --help option for more documentation." << std::endl;
  }

  /*CRADLE::Cradle cradle(outputName);

  cradle.Initialise(configFilename, argc, argv);

  OptionContainer::GetInstance(argc, argv);
  if (!(OptExists("config") && OptExists("name") && OptExists("charge") && OptExists("nucleons"))) {
    cout << "Specify configuration file, isotope name, charge and number of nucleons. Use the --help option for more documentation." << endl;
  } else {
    DecayManager& dm = DecayManager::GetInstance();
    dm.RegisterBasicParticles();
    dm.RegisterBasicDecayModes();
    dm.RegisterBasicSpectrumGenerators();
#ifdef USE_BSG
    if (OptExists("usebsg")) {
      dm.RegisterSpectrumGenerator("BetaMinus", BSG::GetInstance());
      dm.RegisterSpectrumGenerator("BetaPlus", BSG::GetInstance());
    }
#endif
    bool success = dm.Initialise(GetOpt(std::string, "name"), GetOpt(int, "charge"),
    GetOpt(int, "nucleons"), GetOpt(double, "energy"), GetOpt(std::string, "file"),
    GetOpt(int, "threads"));
    if(success) {
      dm.MainLoop(GetOpt(int, "loop"));
      if (GetOpt(int, "General.Verbosity") > 0)
        dm.ListRegisteredParticles();
    }
  }*/
  return 0;
}
