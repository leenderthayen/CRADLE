#include "DecayManager.hh"
#include "SpectrumGenerator.hh"
#include <iostream>
#include <string>

#include "CRADLEConfig.h"


using std::cout;
using std::endl;
using std::cerr;

void ShowInfo() {
  std::string author = "L. Hayen (leendert.hayen@kuleuven.be)";
  cout << "-----------------------------------------------" << endl;
  cout << "-  CRADLE++ version " << std::string(CRADLE_VERSION) << "      -" << endl;
  cout << "-  Last update: " << std::string(CRADLE_LAST_UPDATE) << endl;
  cout << "-  Author: " << author << endl;
  cout << "-----------------------------------------------\n" << endl;
}

int main (int argc, char* argv[]) {
  ShowInfo();


//  if (!(OptExists("config") && OptExists("name") && OptExists("charge") && OptExists("nucleons"))) {
//    cout << "Specify configuration file, isotope name, charge and number of nucleons. Use the --help option for more documentation." << endl;
//  } else {
//    DecayManager& dm = DecayManager::GetInstance();
//    dm.RegisterBasicParticles();
//    dm.RegisterBasicDecayModes();
//    dm.RegisterBasicSpectrumGenerators();
//#ifdef USE_BSG
//    if (OptExists("usebsg")) {
//      dm.RegisterSpectrumGenerator("BetaMinus", BSG::GetInstance());
//      dm.RegisterSpectrumGenerator("BetaPlus", BSG::GetInstance());
//    }
//#endif
//    bool success = dm.Initialise(GetOpt(std::string, "name"), GetOpt(int, "charge"),
//    GetOpt(int, "nucleons"), GetOpt(double, "energy"), GetOpt(std::string, "file"),
//    GetOpt(int, "threads"));
//    if(success) {
//      dm.MainLoop(GetOpt(int, "loop"));
//      if (GetOpt(int, "General.Verbosity") > 0)
//        dm.ListRegisteredParticles();
//    }
//  }
  cout << "Exiting..." << endl;
  return 0;
}
