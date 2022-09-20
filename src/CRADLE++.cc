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
  app.allow_extras(true);

  try {
    app.parse(argc, argv);
  } catch (const CLI::ParseError &e) {
    app.exit(e);
  }

  CRADLE::DecayManager& dm = CRADLE::DecayManager::GetInstance();
  bool success = dm.Initialise(configFilename, argc, argv);

  if (success) {
    dm.MainLoop();
  } else {
    std::cout << "Specify configuration file, isotope name, charge and number of nucleons. Use the --help option for more documentation." << std::endl;
  }

  return 0;
}
