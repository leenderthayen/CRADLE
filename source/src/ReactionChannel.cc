#include "DecayChannel.hh"
#include "DecayMode.hh"
#include "Particle.hh"
#include <iostream>
#include <stdexcept>

using std::cout;
using std::endl;

DecayChannel::DecayChannel(string md, double q, double i, double t, double pExEn, double dExEn):
modeName(md), decayMode(dm), Q(q), intensity(i), lifetime(t), parentExcitationEnergy(pExEn), daughterExcitationEnergy(dExEn) {
}
