#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <string>

#include "CRADLE/SpectrumGenerator.h"
#include "CRADLE/Utilities.h"
//#include "CRADLEConfig.h"

#include "spdlog/spdlog.h"

#ifdef USE_BSG
#include "BSG/Generator.h"
#endif

namespace pt = boost::property_tree;

void SpectrumGenerator::RegisterDistribution(const std::string name,
                                             std::vector<std::vector<double> >* dist) {
  registeredDistributions.insert(
                                 std::pair<std::string, std::vector<std::vector<double> >*>(name, dist));
  // cout << "Registered distribution " << name << endl;
}

std::vector<std::vector<double> >* SpectrumGenerator::GetDistribution(const std::string name) {
  if (registeredDistributions.count(name) == 0) {
      throw std::invalid_argument("Distribution not registered.");
  }
  return registeredDistributions.at(name);
}

std::vector<std::vector<double> >* DeltaSpectrumGenerator::GenerateSpectrum(PDS::core::DynamicParticle& initState, PDS::core::DynamicParticle& finalState, double Q) {
  std::vector<std::vector<double> >* deltaDist = new std::vector<std::vector<double> >();
  std::vector<double> pair = {Q, 1.0};
  deltaDist->push_back(pair);

  return deltaDist;
}

std::vector<std::vector<double> >* SimpleBetaDecay::GenerateSpectrum(PDS::core::DynamicParticle& initState, PDS::core::DynamicParticle& finalState, double Q) {
  PDS::core::Nucleus* initNucleusDef = static_cast<PDS::core::Nucleus*>(initState.GetParticle().GetParticleDefinition());
  PDS::core::Nucleus* finalNucleusDef = static_cast<PDS::core::Nucleus*>(finalState.GetParticle().GetParticleDefinition());
  std::vector<std::vector<double> >* spectrum = utilities::GenerateBetaSpectrum(
  (finalNucleusDef->GetZ() - initNucleusDef->GetZ())*finalNucleusDef->GetZ(), finalNucleusDef->GetZ()+finalNucleusDef->GetA(), Q, true);

  return spectrum;
}

SpectrumGenerator::SpectrumGenerator() { }

SpectrumGenerator::~SpectrumGenerator() { }

DeltaSpectrumGenerator::DeltaSpectrumGenerator() { }

SimpleBetaDecay::SimpleBetaDecay() {}

// #ifdef USE_BSG
// std::vector<std::vector<double> >* BSG::GenerateSpectrum(PDS::core::DynamicParticle* initState, PDS::core::DynamicParticle* finalState, double Q) {
//
//   bsg::Generator* gen = new bsg::Generator();
//
//   std::vector<std::vector<double> >* spectrum = gen->CalculateSpectrum();
//
//   return spectrum;
// }
//
// BSG::BSG() { }

// #endif
