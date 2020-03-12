#include "CRADLE/ReactionEngine.h"
#include "CRADLE/ReactionMode.h"

#include "PDS/Core/ReactionChannel.h"

#include <stdexcept>

namespace CRADLE {

  ReactionEngine::ReactionEngine() {
    RegisterDefaultReactionModes();
  }

  Event ReactionEngine::ProcessParticle(PDS::core::DynamicParticle& initState) {
    Event event;
    PDS::core::ReactionModeName modeName;
    std::array<double, 4> vertexPos;

    std::vector<PDS::core::DynamicParticle> finalStates = ProcessDecay(initState, vertexPos, modeName);

    event.AddVertex(finalStates, vertexPos, modeName);

    return event;
  }

  std::vector<PDS::core::DynamicParticle> ReactionEngine::ProcessDecay(PDS::core::DynamicParticle& initState,
    std::array<double, 4>& vertexPos, PDS::core::ReactionModeName& modeName) {

    std::vector<PDS::core::DynamicParticle> finalStates;
    //Decay time in center of mass frame
    double tauDecay = PDS::util::GetExponentialSample(1.0/initState.GetParticle().GetLifetime());

    PDS::core::ReactionChannel rc = initState.GetParticle().GetRandomReactionChannel();
    try {
      const ReactionMode& rm = GetReactionMode(rc.GetReactionModeName());
      finalStates = rm.Activate(initState, rc.GetQValue(), rc.GetFinalExcitationEnergy());
    } catch (int e) {
      //TODO
    }

    //TODO Lorentz transformation into lab frame

    return finalStates;
  }

  void ReactionEngine::RegisterReactionMode(PDS::core::ReactionModeName modeName, ReactionMode reactionMode){
    try {
      auto it = reactionDictionary.find(modeName);
      if (it != reactionDictionary.end())
        reactionDictionary[modeName] = reactionMode;
      else
        reactionDictionary.insert(std::make_pair(modeName, reactionMode));
    } catch (const std::invalid_argument &e) {
          std::cout << "Cannot register activator method. Invalid mode name." << std::endl;
    }
  }

  void ReactionEngine::RegisterDefaultReactionModes() {
    RegisterReactionMode(PDS::core::ReactionModeName::Proton, ReactionModeFactory::DefaultProtonSeparation());
    RegisterReactionMode(PDS::core::ReactionModeName::Alpha, ReactionModeFactory::DefaultAlpha());
    RegisterReactionMode(PDS::core::ReactionModeName::Gamma, ReactionModeFactory::DefaultGamma());
    //RegisterReactionMode(PDS::core::ReactionModeName::InternalConversion, &ConversionElectron::activate);
    RegisterReactionMode(PDS::core::ReactionModeName::BetaPlus, ReactionModeFactory::DefaultBetaPlus());
    RegisterReactionMode(PDS::core::ReactionModeName::BetaMinus, ReactionModeFactory::DefaultBetaMinus());
  }

  const ReactionMode& ReactionEngine::GetReactionMode(PDS::core::ReactionModeName modeName) {
    try {
      auto it = reactionDictionary.find(modeName);
      if (it != reactionDictionary.end()) {
        return reactionDictionary[modeName];
      } else {
        throw;
      }
    } catch (const std::invalid_argument &e) {
      std::cout << "Cannot register activator method. Invalid mode name." << std::endl;
    }
  }

std::string ReactionEngine::GenerateEvent(int eventNr, std::string initStateName, double initExcitationEn, ConfigOptions configOptions) {
  double time = 0.;
  std::ostringstream eventDataSS;
  std::vector<PDS::core::DynamicParticle> particleStack;
  PDS::core::DynamicParticle ini = PDS::ParticleFactory::CreateNewDynamicParticle(initStateName,initExcitationEn);
  particleStack.push_back(ini);
  while (!particleStack.empty()) {
    PDS::core::DynamicParticle dp = particleStack.back();
    //cout << "Decaying particle " << p->GetName() << endl;
    std::vector<PDS::core::DynamicParticle> finalStates;
    //TODO: add similar method in PDS
    double decayTime = GetDecayTime(dp.GetParticle().GetLifetime());

    if ((time + decayTime) <= configOptions.cuts.Lifetime) {
      try {
        finalStates = Decay(dp,configOptions);
        time += decayTime;
        // cout << "Decay finished" << endl;
      } catch (const std::invalid_argument& e) {
        // cout << "Decay Mode for particle " << p->GetName() << " not found.
        // Aborting." << endl;
        return "";
      }
    } else {
      // cout << "Particle " << p->GetName() << " is stable" << endl;
      eventDataSS << eventNr << "\t" << time << "\t" << GetInfoForFile(dp) << "\n";
    }
//    delete particleStack.back();
    particleStack.pop_back();
    if (!finalStates.empty()) {
      particleStack.insert(particleStack.end(), finalStates.begin(),
                           finalStates.end());
    }
  }
  return eventDataSS.str();
}

}//end of CRADLE namespace
