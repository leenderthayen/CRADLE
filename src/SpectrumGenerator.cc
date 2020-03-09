#include "CRADLE/SpectrumGenerator.h"
#include "CRADLE/Utilities.h"
//#include "CRADLEConfig.h"
#include "PDS/Core/Nucleus.h"
#include "spdlog/spdlog.h"
#include <string>

//#ifdef USE_BSG
#include "BSG/Generator.h"
//#endif

namespace CRADLE {

  void SpectrumGenerator::RegisterDistribution(const std::string name, std::vector<std::vector<double> >* dist) {
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

    std::vector<std::vector<double> >* DeltaSpectrumGenerator::GenerateSpectrum(PDS::core::Particle& initState, PDS::core::Particle& finalState, double Q) {
      std::vector<std::vector<double> >* deltaDist = new std::vector<std::vector<double> >();
      std::vector<double> pair = {Q, 1.0};
      deltaDist->push_back(pair);

      return deltaDist;
    }

    std::vector<std::vector<double> >* SimpleBetaDecay::GenerateSpectrum(PDS::core::Particle& initState, PDS::core::Particle& finalState, double Q) {
      PDS::core::Nucleus* initNucleusDef = static_cast<PDS::core::Nucleus*>(initState.GetParticleDefinition());
      PDS::core::Nucleus* finalNucleusDef = static_cast<PDS::core::Nucleus*>(finalState.GetParticleDefinition());
      std::vector<std::vector<double> >* spectrum = utilities::GenerateBetaSpectrum(
        (finalNucleusDef->GetZ() - initNucleusDef->GetZ())*finalNucleusDef->GetZ(), finalNucleusDef->GetZ()+finalNucleusDef->GetA(), Q, true);

        return spectrum;
      }

      SpectrumGenerator::SpectrumGenerator() { }

      SpectrumGenerator::~SpectrumGenerator() { }

      DeltaSpectrumGenerator::DeltaSpectrumGenerator() { }

      SimpleBetaDecay::SimpleBetaDecay() {}

      //#ifdef USE_BSG

      ExternalBSG::ExternalBSG(std::string configFilename) {
        generator = new BSG::Generator();

        generator->InitialiseOptionsFromConfigFile(configFilename);
      }

      const BSG::Generator* ExternalBSG::GetGenerator() const {
        return generator;
      }

      std::vector<std::vector<double> >* ExternalBSG::GenerateSpectrum(PDS::core::Particle& initState, PDS::core::Particle& finalState, double Q) {
        generator->SetInitialState(initState);
        generator->SetFinalState(finalState);
        generator->SetQValue(Q);

        std::vector<std::vector<double> >* spectrum = generator->CalculateSpectrum();

        return spectrum;
      }
      //#endif
      //
      // BSG::BSG() { }

    }//end of CRADLE namespace

    // #endif
