#ifndef REACTIONENGINE
#define REACTIONENGINE

#include <vector>

namespace PDS {
  namespace core {
    class DynamicParticle;
  }
}

class ReactionEngine {
  public:
    ReactionEngine(std::vector<DynamicParticle *>);
    ~ReactionEngine();

  private:
    std::vector<DynamicParticle *> dynamicParticles;
    int NRTHREADS;
};
#endif
