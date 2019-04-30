#include "OptionContainer.hh"

po::options_description OptionContainer::cmdOptions("Commandline options");
po::options_description OptionContainer::configOptions("Configuration options");
po::options_description OptionContainer::envOptions("Environment variables");
po::variables_map OptionContainer::vm;

using std::cout;
using std::endl;

OptionContainer::OptionContainer(int argc, char** argv) {
  std::string configName = "config.txt";
  cmdOptions.add_options()
    ("help,h", "Product help message")
    ("name,n", po::value<std::string>(), "Name of initial particle")
    ("charge,z", po::value<int>(), "Charge as multiple of proton charge")
    ("nucleons,a", po::value<int>(), "Number of nucleons")
    ("energy,e", po::value<double>()->default_value(0.), "Excitation energy of initial state")
    ("loop,l", po::value<int>()->default_value(1), "Number of events to generate")
    ("threads,t", po::value<int>()->default_value(8), "Number of threads (2 x #CPU)")
    ("file,f", po::value<std::string>()->default_value("output.txt"), "Output file")
    ("config,c", po::value<std::string>(&configName)->default_value("config.txt"), "Config file")
  ;
  configOptions.add_options()
    ("General.Verbosity", po::value<int>(), "General verbosity")
    ("Couplings.CS", po::value<double>(), "Scalar coupling constant")
    ("Couplings.CV", po::value<double>(), "Vector coupling constant")
    ("Couplings.CT", po::value<double>(), "Tensor coupling constant")
    ("Couplings.CA", po::value<double>(), "Axial vector coupling constant")
    ("Cuts.Distance", po::value<double>(), "Maximal distance travelled for unstable particle")
    ("Cuts.Lifetime", po::value<double>(), "Maximal lifetime for a particle to be considered unstable")
    ("Cuts.Energy", po::value<double>(), "Maximal deviation between energies in init-Q and final from databases (in keV).")
    ("BetaDecay.Default", po::value<std::string>(), "Set default decay mode to Fermi or Gamov-Teller")
    ("BetaDecay.FermiFunction", po::value<std::string>()->default_value("Advanced"), "Set the calculation of the Fermi function to Simple for the Schrodinger solution, or to Advanced for the traditional Fermi function")
    ("BetaDecay.PolarisationX", po::value<double>(), "Set nuclear polarisation in X of initial state")
    ("BetaDecay.PolarisationY", po::value<double>(), "Set nuclear polarisation in Y of initial state")
    ("BetaDecay.PolarisationZ", po::value<double>(), "Set nuclear polarisation in Z of initial state")
  ;

  po::store(po::parse_command_line(argc, argv, cmdOptions), vm);
  po::notify(vm);

  envOptions.add_options()
    ("gammadata", po::value<std::string>()->default_value("GammaData/"), "Folder of the gamma radiation data, same as PhotonEvaporation data of Geant4")
    ("radiationdata", po::value<std::string>()->default_value("RadiationData/"), "Folder of the nuclear radiation data, same as Radioactive Decay folder of Geant4")
  ;

  po::store(po::parse_environment(envOptions, "CRADLE_"), vm);
  po::notify(vm);

  if(vm.count("help")) {
    cout << cmdOptions << endl;
    cout << configOptions << endl;
    cout << envOptions << endl;
  }
  else if (!(vm.count("name") && vm.count("charge") && vm.count("nucleons"))) {
    cout << cmdOptions << endl;
    cout << configOptions << endl;
    cout << envOptions << endl;
  } else {
    /** Parse configuration file
     * Included: configOptions & spectrumOptions
     */
    std::ifstream configStream(configName.c_str());
    if (!configStream.is_open()) {
      cout << "WARNING: " << configName << " cannot be found.\n\n"
                << endl;
    } else {
      po::store(po::parse_config_file(configStream, configOptions, true), vm);
    }
    po::notify(vm);
  }
}
