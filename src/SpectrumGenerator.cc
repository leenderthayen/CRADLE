#include "CRADLE/SpectrumGenerator.h"

#include "PDS/Core/Nucleus.h"
#include "NHL/SpectralFunctions.h"
#include "BSG/Generator.h"

#include "spdlog/spdlog.h"

#include <string>
#include <memory>

namespace CRADLE {

  //-----------------------------------------------------------------------------
  //SpectrumGenerator general methods

  SpectrumGenerator::SpectrumGenerator() {
  }

  SpectrumGenerator::~SpectrumGenerator() {
    for (const auto & x : registeredDistributions) {
      delete x.second;
    }
    registeredDistributions.clear();
  }

  void SpectrumGenerator::RegisterDistribution(const std::string name, std::vector<std::vector<double> >* dist) {
    registeredDistributions.insert(
      std::pair<std::string, std::vector<std::vector<double> >*>(name, dist));
      // cout << "Registered distribution " << name << endl;
  }

  std::vector<std::vector<double> >* SpectrumGenerator::GetDistribution(const std::string name) {
    if (!DistributionExists(name)) {
      throw std::invalid_argument("Distribution not registered.");
    }
    return registeredDistributions.at(name);
  }

  bool SpectrumGenerator::DistributionExists(const std::string name) {
    return registeredDistributions.count(name);
  }

  const std::vector<std::vector<double> >* SpectrumGenerator::GetSpectrum(const PDS::core::Particle& initState, const PDS::core::Particle& finalState, double Q) {
    std::ostringstream oss;
    oss << "ID" << initState.GetID() << "_to_ID" << finalState.GetID() << "_Q" << Q / keV;
    std::string name = oss.str();
    if (!DistributionExists(name)) {
      RegisterDistribution(name, GenerateSpectrum(initState, finalState, Q));
    }
    return GetDistribution(name);
  }

  //-----------------------------------------------------------------------------
  //DeltaSpectrumGenerator

  std::vector<std::vector<double> >* DeltaSpectrumGenerator::GenerateSpectrum(const PDS::core::Particle& initState, const PDS::core::Particle& finalState, double Q) {
    std::vector<std::vector<double> >* deltaDist = new std::vector<std::vector<double> >();
    std::vector<double> pair = {Q, 1.0};
    deltaDist->push_back(pair);

    return deltaDist;
  }

  //-----------------------------------------------------------------------------
  //BasicBetaSpectrumGenerator

  BasicBetaSpectrumGenerator::BasicBetaSpectrumGenerator(bool aff, double ss) {
    advancedFermiFunction = aff;
    stepSize = ss;
  }

  std::vector<std::vector<double> >* BasicBetaSpectrumGenerator::GenerateSpectrum(const PDS::core::Particle& initState, const PDS::core::Particle& finalState, double Q) {
    const PDS::core::Nucleus* initNucleusDef = static_cast<const PDS::core::Nucleus*>(initState.GetParticleDefinition());
    const PDS::core::Nucleus* finalNucleusDef = static_cast<const PDS::core::Nucleus*>(finalState.GetParticleDefinition());
    std::vector<std::vector<double> >* spectrum = NHL::beta::GenerateBetaSpectrum(
      (finalNucleusDef->GetZ() - initNucleusDef->GetZ())*finalNucleusDef->GetZ(),
      finalNucleusDef->GetZ()+finalNucleusDef->GetA(), Q, advancedFermiFunction);

      return spectrum;
  }

  //-----------------------------------------------------------------------------
  //ExternalBSG

  ExternalBSG::ExternalBSG(std::string configFilename) {
    generator = std::make_shared<BSG::Generator>();

    generator->InitialiseOptionsFromConfigFile(configFilename);
  }

  ExternalBSG::~ExternalBSG() {
  }

  const BSG::Generator* ExternalBSG::GetGenerator() const {
    return generator.get();
  }

  std::vector<std::vector<double> >* ExternalBSG::GenerateSpectrum(const PDS::core::Particle& initState, const PDS::core::Particle& finalState, double Q) {
    generator->SetInitialState(initState);
    generator->SetFinalState(finalState);
    generator->SetQValue(Q);

    std::vector<std::vector<double> >* spectrum = generator->CalculateSpectrum();

    return spectrum;
  }
}//end of CRADLE namespace
