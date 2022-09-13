#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <string>

#include "SpectrumGenerator.hh"
#include "Particle.hh"
#include "Utilities.hh"
#include "CRADLEConfig.h"
#include "OptionContainer.hh"

#include "spdlog/spdlog.h"

#ifdef USE_BSG
#include "BSGOptionContainer.h"
#include "NMEOptionContainer.h"
#include "Generator.h"
#endif

namespace pt = boost::property_tree;

std::vector<std::vector<double> >* DeltaSpectrumGenerator::GenerateSpectrum(Particle* initState, Particle* finalState, double Q) {
  std::vector<std::vector<double> >* deltaDist = new std::vector<std::vector<double> >();
  std::vector<double> pair = {Q, 1.0};
  deltaDist->push_back(pair);

  return deltaDist;
}

std::vector<std::vector<double> >* SimpleBetaDecay::GenerateSpectrum(Particle* initState, Particle* finalState, double Q) {
  std::vector<std::vector<double> >* spectrum = utilities::GenerateBetaSpectrum(
  (finalState->GetCharge() - initState->GetCharge())*finalState->GetCharge(),
  finalState->GetCharge()+finalState->GetNeutrons(), Q, true);

  return spectrum;
}

std::vector<std::vector<double> >* AdvancedBetaDecay::GenerateSpectrum(Particle* initState, Particle* finalState, double Q) {
  std::vector<std::vector<double> >* spectrum = utilities::GenerateBetaSpectrum(
  (finalState->GetCharge() - initState->GetCharge())*finalState->GetCharge(),
  finalState->GetCharge()+finalState->GetNeutrons(), Q, true);

  return spectrum;
}

SpectrumGenerator::SpectrumGenerator() { }

SpectrumGenerator::~SpectrumGenerator() { }

DeltaSpectrumGenerator::DeltaSpectrumGenerator() { }

SimpleBetaDecay::SimpleBetaDecay() {}

AdvancedBetaDecay::AdvancedBetaDecay() {}

#ifdef USE_BSG
std::vector<std::vector<double> >* BSG::GenerateSpectrum(Particle* initState, Particle* finalState, double Q) {

  spdlog::warn("In GenerateSpectrum");
  bsg::BSGOptionContainer::GetInstance();
  nme::NMEOptionContainer::GetInstance();
  bsg::BSGOptionContainer::ClearVariablesMap();
  nme::NMEOptionContainer::GetInstance().ClearVariablesMap();
  spdlog::warn("Cleared VMs");
  bsg::BSGOptionContainer::ParseCmdLineOptions(0, NULL);
  nme::NMEOptionContainer::GetInstance().ParseCmdLineOptions(0, NULL);

  spdlog::warn("Parsed Cmd Options");
  spdlog::warn("Test NME: {}", GetNMEOpt(std::string, output));

  std::string filename = "test.ini";
  std::string bsgConfig = GetOpt(std::string, "bsgconfig");

  WriteINIFile(filename, initState, finalState, Q);

  spdlog::warn("Parsing config");
  //nme::NMEOptionContainer::GetInstance().ParseConfigOptions(bsgConfig);
  bsg::BSGOptionContainer::ParseConfigOptions(bsgConfig);

  spdlog::warn("Parsing input");
  bsg::BSGOptionContainer::ParseInputOptions(filename);
  //nme::NMEOptionContainer::GetInstance().ParseInputOptions(filename);

  spdlog::warn("Constructing gen");

  bsg::Generator* gen = new bsg::Generator();

  std::vector<std::vector<double> >* spectrum = gen->CalculateSpectrum();

  delete gen;

  return spectrum;
}

void BSG::WriteINIFile(const std::string filename, Particle* initState, Particle* finalState, double Q) {
  // Create an empty property tree object.
  pt::ptree tree;

  //TODO Change Fermi/GT
  //Mae the most basic transition config file
  tree.put("Transition.Process", (initState->GetCharge() < finalState->GetCharge()) ? "B-" : "B+");
  tree.put("Transition.Type", "Gamow-Teller");
  tree.put("Transition.QValue", Q);

  tree.put("Mother.Z", initState->GetCharge());
  tree.put("Mother.A", initState->GetCharge() + initState->GetNeutrons());
  tree.put("Daughter.Z", finalState->GetCharge());
  tree.put("Daughter.A", finalState->GetCharge() + finalState->GetNeutrons());
  //TODO Put actual spinparities in here
  tree.put("Mother.SpinParity", 2);
  tree.put("Daughter.SpinParity", 0);

  // Write property tree to INI file
  pt::write_ini(filename, tree);
}

BSG::BSG() { }

#endif
