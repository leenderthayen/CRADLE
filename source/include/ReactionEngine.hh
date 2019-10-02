#ifndef REACTIONENGINE
#define REACTIONENGINE

#include <vector>
#include <map>

namespace PDS {
  namespace core {
    class DynamicParticle;
  }
}

enum class ReactionModeNames;

typedef std::vector<PDS::core::DynamicParticle*> (*activator)(PDS::core::DynamicParticle*, double, double, SpectrumGenerator*);
typedef std::map<ReactionModeNames, activator> reaction_mode_map;

class ReactionEngine {
  public:
    ReactionEngine(std::vector<DynamicParticle *>);
    ~ReactionEngine();

    virtual void RegisterSpectrumGenerators();
    virtual void RegisterReactionModes();

    std::string GenerateEvent(int);

  private:
    void RegisterSpectrumGenerator();
    void RegisterReactionMode();
    std::vector<DynamicParticle *> dynamicParticles;
    reaction_mode_map registeredReactionModeMap;
    std::map<ReactionModeNames, SpectrumGenerator&> registeredSpectrumGeneratorMap;
    int NRTHREADS;
};
#endif
