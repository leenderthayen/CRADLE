#ifndef DECAYMANAGER
#define DECAYMANAGER

#include <vector>
#include <map>
#include <string>

namespace PDS {
  namespace core {
    class DynamicParticle;
  }
}

class ReactionMode;
class SpectrumGenerator;
class ReactionEngine;
struct ConfigOptions;

class DecayManager {
  public:
    static DecayManager& GetInstance() {
      static DecayManager instance;
      return instance;
    }

    ~DecayManager();

    void Initialise(ConfigOptions);
    bool MainLoop(int,int);

    void SetReactionEngine(ReactionEngine* );

  private:
    DecayManager() {};
    DecayManager(DecayManager const&);
    void operator=(DecayManager const&);

    std::string GenerateEvents(int);
    
    std::string filename;
    std::string initStateName;
    double initExcitationEn;
    ReactionEngine* reactionEngine;
    ConfigOptions* configOptions;
};
#endif
