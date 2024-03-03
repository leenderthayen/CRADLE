#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <string>

#include "CRADLE/SpectrumGenerator.hh"
#include "CRADLE/Particle.hh"
#include "CRADLE/Utilities.hh"

#include "spdlog/spdlog.h"

namespace CRADLE {

namespace pt = boost::property_tree;

std::vector<std::vector<double> >* DeltaSpectrumGenerator::GenerateSpectrum(Particle* initState, Particle* finalState, double Q, std::string DecayType) {
  std::vector<std::vector<double> >* deltaDist = new std::vector<std::vector<double> >();
  std::vector<double> pair = {Q, 1.0};
  deltaDist->push_back(pair);

  return deltaDist;
}

std::vector<std::vector<double> >* SimpleBetaDecay::GenerateSpectrum(Particle* initState, Particle* finalState, double Q, std::string DecayType) {
  std::vector<std::vector<double> >* spectrum = utilities::GenerateBetaSpectrum(
  (finalState->GetCharge() - initState->GetCharge())*finalState->GetCharge(),
  finalState->GetCharge()+finalState->GetNeutrons(), Q, false, DecayType);

  return spectrum;
}

std::vector<std::vector<double> >* AdvancedBetaDecay::GenerateSpectrum(Particle* initState, Particle* finalState, double Q, std::string DecayType) {
  std::vector<std::vector<double> >* spectrum = utilities::GenerateBetaSpectrum(
  (finalState->GetCharge() - initState->GetCharge())*finalState->GetCharge(),
  finalState->GetCharge()+finalState->GetNeutrons(), Q, true, DecayType);

  return spectrum;
}

SpectrumGenerator::SpectrumGenerator() { }

SpectrumGenerator::~SpectrumGenerator() { }

DeltaSpectrumGenerator::DeltaSpectrumGenerator() { }

SimpleBetaDecay::SimpleBetaDecay() {}

AdvancedBetaDecay::AdvancedBetaDecay() {}

}//End of CRADLE namespace
