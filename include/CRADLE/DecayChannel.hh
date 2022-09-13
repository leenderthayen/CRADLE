#ifndef DECAYCHANNEL
#define DECAYCHANNEL

#include <vector>
#include <string>

namespace CRADLE {

class Particle;
class DecayMode;

using std::string;

class DecayChannel {
  public:
    DecayChannel(string, DecayMode*, double, double, double, double, double);

    inline double GetIntensity() { return intensity; };
    inline double GetQValue() { return Q; };
    inline double GetLifetime() { return lifetime; };
    inline double GetDaughterExcitationEnergy() { return daughterExcitationEnergy; };
    inline double GetParentExcitationEnergy() { return parentExcitationEnergy; };
    inline string GetModeName() { return modeName; };

    std::vector<Particle*> Decay(Particle*);
  private:
    double daughterExcitationEnergy;
    double parentExcitationEnergy;
    double intensity;
    double Q;
    double lifetime;
    string modeName;
    DecayMode* decayMode;
};

}//End of CRADLE namespace
#endif
