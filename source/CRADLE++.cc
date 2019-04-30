#include "DecayManager.hh"
#include "OptionContainer.hh"
#include <iostream>
#include <string>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include "CRADLEConfig.h"

namespace po = boost::program_options;

using std::cout;
using std::endl;
using std::cerr;

void ShowIntro() {
  std::string author = "L. Hayen (leendert.hayen@kuleuven.be)";
  cout << "-----------------------------------------------" << endl;
  cout << "-  CRADLE++ version " << std::string(CRADLE_VERSION) << "      -" << endl;
  cout << "-  Last update: " << std::string(CRADLE_LAST_UPDATE) << endl;
  cout << "-  Author: " << author << endl;
  cout << "-----------------------------------------------\n" << endl;
}

int main (int argc, char* argv[]) {
  ShowIntro();
  po::options_description cmdOptions("Commandline options");

  int nrLoops = 1;
  std::string name;
  std::string filename = "output.txt";
  int Z = 0;
  int A = 0;
  double excitationEnergy = 0.;
  std::string configName = "config.txt";
  int verbosity = 0.;
  int threads = 8;

  cmdOptions.add_options()
    ("help,h", "Product help message")
    ("name,n", po::value<std::string>(&name), "Name of initial particle")
    ("charge,z", po::value<int>(&Z)->default_value(0), "Charge as multiple of proton charge")
    ("nucleons,a", po::value<int>(&A)->default_value(0), "Number of nucleons")
    ("energy,e", po::value<double>(&excitationEnergy)->default_value(0), "Excitation energy of initial state")
    ("loop,l", po::value<int>(&nrLoops)->default_value(1), "Number of events to generate")
    ("threads,t", po::value<int>(&threads)->default_value(8), "Number of threads (2 x #CPU)")
    ("file,f", po::value<std::string>(&filename)->default_value("output.txt"), "Output file")
    ("config,c", po::value<std::string>(&configName)->default_value("config.txt"), "Config file")
    ("bsgconfig,b", po::value<std::string>(), "Config file for the BSG library")
  ;


  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, cmdOptions), vm);
  po::notify(vm);

  OptionContainer::GetInstance(configName);

  if(vm.count("help")) {
    cout << cmdOptions;
    cout << OptionContainer::GetConfigOptions();
    cout << OptionContainer::GetEnvOptions();
  }
  else if (!(vm.count("name") && vm.count("charge") && vm.count("nucleons"))) {
    cout << cmdOptions;
    cout << OptionContainer::GetConfigOptions();
    cout << OptionContainer::GetEnvOptions();
  }
  else {
    DecayManager& dm = DecayManager::GetInstance();
    dm.RegisterBasicParticles();
    dm.RegisterBasicDecayModes();
    dm.RegisterBasicSpectrumGenerators();
    bool success = dm.Initialise(name, Z, A, excitationEnergy, filename, threads);
    if(success) {
      dm.MainLoop(nrLoops);
      if (OptionContainer::GetInstance().GetOption<int>("General.Verbosity") > 0)
        dm.ListRegisteredParticles();
    }
    cout << "Exiting..." << endl;
    return 0;
  }
  return 1;
}
