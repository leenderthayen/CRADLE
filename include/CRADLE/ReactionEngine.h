#ifndef CRADLE_REACTION_ENGINE_H
#define CRADLE_REACTION_ENGINE_H

#include "CRADLE/Event.h"
#include "CRADLE/ReactionMode.h"

#include "PDS/Core/DynamicParticle.h"
#include "PDS/Core/ReactionChannel.h"

#include <map>
#include <vector>
#include <random>
#include <array>
#include <memory>

namespace CRADLE {

  class ReactionEngine {
  public:
    ReactionEngine();
    ~ReactionEngine() {};

    void RegisterDefaultReactionModes();
    void RegisterReactionMode(PDS::core::ReactionModeName, ReactionMode);

    std::shared_ptr<PDS::core::Vertex> ProcessParticle(std::shared_ptr<PDS::core::DynamicParticle>, const std::array<double, 4>&);

  private:
    static inline std::default_random_engine randomGen;
    std::map<PDS::core::ReactionModeName, ReactionMode> reactionDictionary;

    std::vector<PDS::core::DynamicParticle> ProcessDecay(const PDS::core::DynamicParticle&, std::array<double, 4>&, PDS::core::ReactionModeName&);

    const ReactionMode& GetReactionMode(PDS::core::ReactionModeName);
  };

}//end of CRADLE namespace
#endif
