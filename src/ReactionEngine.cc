#include "CRADLE/ReactionEngine.h"
#include "CRADLE/ReactionMode.h"

#include "PDS/Core/ReactionChannel.h"
#include "PDS/Core/Vertex.h"

#include <stdexcept>

namespace CRADLE {

  ReactionEngine::ReactionEngine() {
    RegisterDefaultReactionModes();
  }

  PDS::core::Vertex ReactionEngine::ProcessParticle(PDS::core::DynamicParticle& initState, PDS::core::Vertex& productionVertex) {
    PDS::core::Vertex vertex;
    PDS::core::ReactionModeName modeName;
    std::array<double, 4> vertexPos;

    std::vector<PDS::core::DynamicParticle> finalStates = ProcessDecay(initState, vertexPos, modeName);

    vertexPos += productionVertex;

    vertex.AddParticleIn(initState);
    for (auto & p : finalStates) {
      p.SetProductionVertex(v);
      vertex.AddParticleOut(p);
    }

    vertex.SetPosition(vertexPos);

    return vertex;
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

}//end of CRADLE namespace
