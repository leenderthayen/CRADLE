#include "CRADLE/ReactionEngine.h"
#include "CRADLE/ReactionMode.h"

#include "PDS/Core/ReactionChannel.h"
#include "PDS/Core/Vertex.h"
#include "PDS/Math/LorentzAlgebra.h"
#include "PDS/Units/GlobalPhysicalConstants.h"

#include <stdexcept>
#include <utility>

namespace CRADLE {

  ReactionEngine::ReactionEngine() {
    RegisterDefaultReactionModes();
  }

  std::shared_ptr<PDS::core::Vertex> ReactionEngine::ProcessParticle(
    std::shared_ptr<PDS::core::DynamicParticle> initState,
    const ublas::vector<double>& productionPos) {

    //Make the new vertex
    //TODO ID, now taken care of in Cradle.cc, should be here?
    std::shared_ptr<PDS::core::Vertex> vertex = std::make_shared<PDS::core::Vertex>(1);
    ublas::vector<double> vertexPos(4);

    PDS::core::ReactionModeName modeName;

    std::vector<PDS::core::DynamicParticle> finalStates = ProcessDecay(*initState.get(), vertexPos, modeName);

    vertexPos += productionPos;

    initState->SetDestructionVertex(vertex);
    vertex->AddParticleIn(initState);
    vertex->SetReactionModeName(modeName);
    vertex->SetPosition(vertexPos);

    for (auto & p : finalStates) {
      p.SetProductionVertex(vertex);
      vertex->AddParticleOut(std::make_shared<PDS::core::DynamicParticle>(std::move(p)));
    }
    return vertex;
  }

  std::vector<PDS::core::DynamicParticle> ReactionEngine::ProcessDecay(const PDS::core::DynamicParticle& initState,
    ublas::vector<double>& vertexPos, PDS::core::ReactionModeName& modeName) {

    std::vector<PDS::core::DynamicParticle> finalStates;
    //Decay time in center of mass frame
    double tauDecay = PDS::util::GetExponentialSample(1.0/initState.GetParticle().GetLifetime());

    PDS::core::ReactionChannel rc = initState.GetParticle().GetRandomReactionChannel();
    try {
      const ReactionMode& rm = GetReactionMode(rc.GetReactionModeName());
      finalStates = rm.Activate(initState, rc.GetQValue(), rc.GetFinalExcitationEnergy());
      modeName = rc.GetReactionModeName();
    } catch (int e) {
      //TODO
    }

    // Decay products are already in lab frame, except for a four-vector translation offset
    // The momentum of the initial state is recorded, the decay is performed
    // in the center of mass frame, and the final products are boosted back to the lab frame
    // where the origin of the COM and lab frame coincide

    // While docs say it should be initialized to 0, it doesn't
    ublas::vector<double> decayPosCOM(4);
    // spacetime interval must be done explicitly with c_light for other units to work
    decayPosCOM(0) = tauDecay * c_light;
    decayPosCOM(1) = 0;
    decayPosCOM(2) = 0;
    decayPosCOM(3) = 0;

    ublas::vector<double> velocity = -initState.GetVelocity();
    vertexPos = PDS::util::LorentzBoost(velocity, decayPosCOM);

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
      throw "Cannot register activator method. Invalid mode name.";
    }
  }

}//end of CRADLE namespace
