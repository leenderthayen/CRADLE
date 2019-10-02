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

class DecayManager {
  public:
    static DecayManager& GetInstance() {
      static DecayManager instance;
      return instance;
    }

    ~DecayManager();

    bool Initialise(std::string, int, int, double, std::string, int);
    bool MainLoop(int);

  protected:
    virtual void SetReactionEngine(ReactionEngine* );

  private:
    DecayManager() {};
    DecayManager(DecayManager const&);
    void operator=(DecayManager const&);

    bool InitializeOptionsFromConfigFile(std::string);

    //std::vector<Particle*> particleStack;
    std::string filename;
    std::string initStateName;
    double initExcitationEn;
    int NRTHREADS;
    ReactionEngine* reactionEngine;
    ConfigOptions configOptions;
};
#endif
