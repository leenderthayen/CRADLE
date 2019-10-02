#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <string>

#include "SpectrumGenerator.hh"
#include "PDS/Core/DynamicParticle.hh"
#include "Utilities.hh"

#include "spdlog/spdlog.h"

#ifdef USE_BSG

#include "Generator.h"
#endif

namespace pt = boost::property_tree;

void SpectrumGenerator::RegisterDistribution(const string name,
                                        vector<vector<double> >* dist) {
  registeredDistributions.insert(
      pair<string, vector<vector<double> >*>(name, dist));
  // cout << "Registered distribution " << name << endl;
}

vector<vector<double> >* SpectrumGenerator::GetDistribution(const string name) {
  if (registeredDistributions.count(name) == 0) {
    throw invalid_argument("Distribution not registered.");
  }
  return registeredDistributions.at(name);
}

std::vector<std::vector<double> >* DeltaSpectrumGenerator::GenerateSpectrum(PDS::core::DynamicParticle* initState, PDS::core::DynamicParticle* finalState, double Q) {
  std::vector<std::vector<double> >* deltaDist = new std::vector<std::vector<double> >();
  std::vector<double> pair = {Q, 1.0};
  deltaDist->push_back(pair);

  return deltaDist;
}

std::vector<std::vector<double> >* SimpleBetaDecay::GenerateSpectrum(PDS::core::DynamicParticle* initState, PDS::core::DynamicParticle* finalState, double Q) {
  std::vector<std::vector<double> >* spectrum = utilities::GenerateBetaSpectrum(
  (finalState->GetCharge() - initState->GetCharge())*finalState->GetCharge(),
  finalState->GetCharge()+finalState->GetNeutrons(), Q, true);

  return spectrum;
}

SpectrumGenerator::SpectrumGenerator() { }

SpectrumGenerator::~SpectrumGenerator() { }

DeltaSpectrumGenerator::DeltaSpectrumGenerator() { }

SimpleBetaDecay::SimpleBetaDecay() {}

#ifdef USE_BSG
std::vector<std::vector<double> >* BSG::GenerateSpectrum(PDS::core::DynamicParticle* initState, PDS::core::DynamicParticle* finalState, double Q) {

  bsg::Generator* gen = new bsg::Generator();

  std::vector<std::vector<double> >* spectrum = gen->CalculateSpectrum();

  return spectrum;
}

BSG::BSG() { }

#endif
