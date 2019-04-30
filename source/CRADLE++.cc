#include "OptionContainer.hh"
#include "DecayManager.hh"
#include <iostream>
#include <string>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include "CRADLEConfig.h"
#include "SpectrumGenerator.hh"

namespace po = boost::program_options;

using std::cout;
using std::endl;
using std::cerr;

void ShowIntro() {
  std::string author = "L. Hayen (leendert.hayen@kuleuven.be)";
  cout << "------------------------------" << endl;
  cout << "-  CRADLE++ version " << std::string(CRADLE_VERSION) << "      -" << endl;
  cout << "-  Last update: " << std::string(CRADLE_LAST_UPDATE) << endl;
  cout << "-  Author: " << author << endl;
  cout << "------------------------------\n" << endl;
}

int main (int argc, char* argv[]) {
  ShowIntro();

  OptionContainer::GetInstance(argc, argv);

  if (OptionContainer::GetInstance().Exists("name") && OptionContainer::GetInstance().Exists("charge")
  && OptionContainer::GetInstance().Exists("nucleons")) {
    DecayManager& dm = DecayManager::GetInstance();
    dm.RegisterBasicParticles();
    dm.RegisterBasicDecayModes();
    dm.RegisterBasicSpectrumGenerators();
    // Comment following line to include the BSG library, placeholder
    dm.RegisterSpectrumGenerator("BetaMinus", BSG::GetInstance());
    dm.RegisterSpectrumGenerator("BetaPlus", BSG::GetInstance());

    bool success = dm.Initialise(OptionContainer::GetInstance().GetOption<std::string>("name"),
    OptionContainer::GetInstance().GetOption<int>("charge"),
    OptionContainer::GetInstance().GetOption<int>("nucleons"),
    OptionContainer::GetInstance().GetOption<double>("energy"),
    OptionContainer::GetInstance().GetOption<std::string>("file"),
    OptionContainer::GetInstance().GetOption<int>("threads"));
    if(success) {
      cout << "Success" << endl;
      dm.MainLoop(OptionContainer::GetInstance().GetOption<int>("loop"));
      if (OptionContainer::GetInstance().GetOption<int>("General.Verbosity") > 0)
        dm.ListRegisteredParticles();
    }
    cout << "Exiting..." << endl;
    return 0;
  } else {
    cerr << "Required input: config, name, charge, nucleons" << endl;
    return 1;
  }
}
