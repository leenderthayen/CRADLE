#include "CRADLE/ReactionMode.h"
#include "CRADLE/DecayStructures.h"

#include "PDS/Util/Miscellaneous.h"

namespace CRADLE {
  ReactionMode::ReactionMode () {

  }

  std::vector<PDS::core::DynamicParticle> ReactionMode::Activate(PDS::core::DynamicParticle& initState, double Q, double finalExcitationEnergy) const {
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

  ReactionMode& ReactionModeFactory::DefaultBetaMinus() {
    //TODO reactionMode is only on stack
    ReactionMode reactionMode;

    std::map<std::string, SpectrumGenerator*> sg = {{"electron_energy", nullptr}};
    Process p = {1., nullptr, &(decay::SimpleBetaMinus), sg};
    reactionMode.AddProcess(p);

    return reactionMode;
  }

  ReactionMode& ReactionModeFactory::DefaultBetaPlus() {
    ReactionMode reactionMode;

    std::map<std::string, SpectrumGenerator*> sg = {{"positron_energy", nullptr}};
    Process p = {1., nullptr, &(decay::SimpleBetaPlus), sg};
    reactionMode.AddProcess(p);

    return reactionMode;
  }

  ReactionMode& ReactionModeFactory::DefaultAlpha() {
    ReactionMode reactionMode;

    //TODO

    return reactionMode;
  }

  ReactionMode& ReactionModeFactory::DefaultGamma() {
    ReactionMode reactionMode;

    //TODO

    return reactionMode;
  }

  ReactionMode& ReactionModeFactory::DefaultProtonSeparation() {
    ReactionMode reactionMode;

    //TODO

    return reactionMode;
  }
}//end of CRADLE namespace
