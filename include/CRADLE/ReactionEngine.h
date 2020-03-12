#ifndef CRADLE_REACTION_ENGINE_H
#define CRADLE_REACTION_ENGINE_H

#include "CRADLE/Event.h"
#include "CRADLE/ReactionMode.h"

#include "PDS/Core/DynamicParticle.h"
#include "PDS/Core/ReactionChannel.h"

#include <map>
#include <vector>
#include <random>

namespace CRADLE {

  class ReactionEngine {
  public:
    ReactionEngine();
    ~ReactionEngine() {};

    void RegisterDefaultReactionModes();
    void RegisterReactionMode(PDS::core::ReactionModeName, ReactionMode);

    Event ProcessParticle(PDS::core::DynamicParticle&);

  private:
    static inline std::default_random_engine randomGen;
    std::map<PDS::core::ReactionModeName, ReactionMode> reactionDictionary;

    std::vector<PDS::core::DynamicParticle> ProcessDecay(PDS::core::DynamicParticle&);

    const ReactionMode& GetReactionMode(PDS::core::ReactionModeName);
  };

}//end of CRADLE namespace
#endif
