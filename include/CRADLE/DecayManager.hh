#ifndef DECAYMANAGER
#define DECAYMANAGER

#include <vector>
#include <map>
#include <string>

#include "CRADLE/ConfigParser.hh"

namespace CRADLE {

class Particle;
class DecayMode;
class SpectrumGenerator;

class DecayManager {
  public:
    static DecayManager& GetInstance() {
      static DecayManager instance;
      return instance;
    }

    ~DecayManager();

    bool Initialise(std::string, int argc = 0, const char** argv = nullptr);
    bool Initialise(ConfigOptions);
    bool Initialise(std::string, int, int, double, std::string, int);
    bool MainLoop();
    bool GenerateNucleus(std::string, int, int);
    void RegisterBasicParticles();
    void RegisterBasicDecayModes();
    void RegisterDecayMode(const std::string, DecayMode&);
    void RegisterParticle(Particle*);
    void RegisterDistribution(const std::string, std::vector<std::vector<double> >*);
    void RegisterSpectrumGenerator(const std::string, SpectrumGenerator&);
    void RegisterBasicSpectrumGenerators();
    void ListRegisteredParticles();
    std::string GenerateEvent(int);
    Particle* GetNewParticle(const std::string, int Z=0, int A=0);
    DecayMode& GetDecayMode(const std::string);
    std::vector<std::vector<double> >* GetDistribution(const std::string);

    ConfigOptions configOptions;

  private:
    DecayManager() {};
    DecayManager(DecayManager const&);
    void operator=(DecayManager const&);

    std::map<const std::string, DecayMode&> registeredDecayModes;
    std::map<const std::string, std::vector<std::vector<double> >* > registeredDistributions;
    //std::vector<Particle*> particleStack;
    std::map<const std::string, Particle*> registeredParticles;
    std::string outputName;
    std::string initStateName;
    double initExcitationEn;
    int NRTHREADS;
};

}//End of CRADLE namespace
#endif
