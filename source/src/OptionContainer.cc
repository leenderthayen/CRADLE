#include "OptionContainer.hh"

po::options_description OptionContainer::configOptions("Configuration options");
po::options_description OptionContainer::envOptions("Environment variables");
po::variables_map OptionContainer::vm;

OptionContainer::OptionContainer(std::string configName) {
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

  std::ifstream configStream(configName.c_str());
  if(!configStream.is_open()) {
    std::cerr << "ERROR: Config.txt cannot be found. Aborting.\n\n" << std::endl;
  }

  po::store(po::parse_config_file(configStream, configOptions), vm);
  po::notify(vm);

  envOptions.add_options()
    ("gammadata", po::value<std::string>()->default_value("GammaData/"), "Folder of the gamma radiation data, same as PhotonEvaporation data of Geant4")
    ("radiationdata", po::value<std::string>()->default_value("RadiationData/"), "Folder of the nuclear radiation data, same as Radioactive Decay folder of Geant4")
  ;

  po::store(po::parse_environment(envOptions, "CRADLE_"), vm);
  po::notify(vm);
}
