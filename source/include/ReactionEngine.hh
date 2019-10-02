#ifndef REACTIONENGINE
#define REACTIONENGINE

#include <vector>
#include <map>

namespace PDS {
  namespace core {
    class DynamicParticle;
    enum class ReactionModeNames;
  }
}
struct Cuts;

typedef std::vector<PDS::core::DynamicParticle*> (*activator)(PDS::core::DynamicParticle*, double, double, SpectrumGenerator*);
typedef std::map<ReactionModeNames, activator> reaction_mode_map;

class ReactionEngine {
  public:
    ReactionEngine(Cuts);
    ReactionEngine(const ReactionEngine &);
    ~ReactionEngine();

    void RegisterBasicSpectrumGenerators();
    void RegisterBasicReactionModes();
    void RegisterSpectrumGenerator();
    void RegisterReactionMode();

    std::string GenerateEvent(int);

  private:
    reaction_mode_map registeredReactionModeMap;
    std::map<ReactionModeNames, SpectrumGenerator&> registeredSpectrumGeneratorMap;
    Cuts cuts;

};
#endif
