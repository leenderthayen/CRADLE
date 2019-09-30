#ifndef REACTIONCHANNEL
#define REACTIONCHANNEL

class Particle;
class DecayMode;

#include <vector>
#include <string>

using std::string;

class ReactionChannel {
  public:
    ReactionChannel(string, double, double, double, double, double);

    inline double GetIntensity() { return intensity; };
    inline double GetQValue() { return Q; };
    inline double GetLifetime() { return lifetime; };
    inline double GetDaughterExcitationEnergy() { return daughterExcitationEnergy; };
    inline double GetParentExcitationEnergy() { return parentExcitationEnergy; };
    inline string GetModeName() { return modeName; };

  private:
    double daughterExcitationEnergy;
    double parentExcitationEnergy;
    double intensity;
    double Q;
    double lifetime;
    string modeName;
};

inline ReactionChannel::ReactionChannel(string md, double q, double i, double t, double pExEn, double dExEn):
modeName(md), decayMode(dm), Q(q), intensity(i), lifetime(t), parentExcitationEnergy(pExEn), daughterExcitationEnergy(dExEn) {
}

#endif
