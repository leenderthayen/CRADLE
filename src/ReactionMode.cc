#include "CRADLE/ReactionMode.h"
#include "CRADLE/DecayStructures.h"

#include "PDS/Util/Random.h"

namespace CRADLE {
  ReactionMode::ReactionMode () {

  }

  ReactionMode::ReactionMode(Process p) {
    processes.push_back(p);
  }

  ReactionMode::~ReactionMode() {
  }

  std::vector<PDS::core::DynamicParticle> ReactionMode::Activate(const PDS::core::DynamicParticle& initState, double Q, double finalExcitationEnergy) const {
    std::vector<double> brs;

    //Loop over processes to calculate BR
    for (auto & p : processes) {
      double br = 0.;
      if (p.staticBranchingRatio > 0) {
        br = p.staticBranchingRatio;
      } else {
        br = p.bc(initState.GetParticle(), Q);
      }
      brs.push_back(br);
    }

    //Random index based on relative branching ratios
    int index = PDS::util::GetWeightedRandomIndex(brs);

    return processes[index].a(initState, Q, finalExcitationEnergy, processes[index].generators);
  }

  bool ReactionMode::RemoveProcessAtIndex (unsigned i) {
    if (i < processes.size()) {
      processes.erase(processes.begin() + i);
      return true;
    }
    return false;
  }

  ReactionMode ReactionModeFactory::DefaultBetaMinus(bool aff, double stepSize) {
    std::shared_ptr<SpectrumGenerator> bbsg(new BasicBetaSpectrumGenerator(aff, stepSize));
    std::map<std::string, std::shared_ptr<SpectrumGenerator> > sg = {{"electron_energy", bbsg}};
    Process p = {1., nullptr, &(decay::SimpleBetaMinus), sg};

    return ReactionMode(p);
  }

  ReactionMode ReactionModeFactory::DefaultBetaPlus(bool aff, double stepSize) {
    std::shared_ptr<SpectrumGenerator> bbsg(new BasicBetaSpectrumGenerator(aff, stepSize));
    std::map<std::string, std::shared_ptr<SpectrumGenerator> > sg = {{"positron_energy", bbsg}};
    Process p = {1., nullptr, &(decay::SimpleBetaPlus), sg};

    return ReactionMode(p);
  }

  ReactionMode ReactionModeFactory::DefaultAlpha() {
    std::map<std::string, std::shared_ptr<SpectrumGenerator> > sg;
    Process p = {1., nullptr, &(decay::Alpha), sg};

    return ReactionMode(p);
  }

  ReactionMode ReactionModeFactory::DefaultGamma() {
    std::map<std::string, std::shared_ptr<SpectrumGenerator> > sg;
    Process p = {1., nullptr, &(decay::Gamma), sg};

    return ReactionMode(p);
  }

  ReactionMode ReactionModeFactory::DefaultProtonSeparation() {
    std::map<std::string, std::shared_ptr<SpectrumGenerator> > sg;
    Process p = {1., nullptr, &(decay::ProtonSeparation), sg};

    return ReactionMode(p);
  }
}//end of CRADLE namespace
