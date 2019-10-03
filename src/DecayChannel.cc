#include "CRADLE/DecayChannel.h"
#include "CRADLE/DecayMode.h"
#include "CRADLE/Particle.h"
#include <iostream>
#include <stdexcept>

using std::cout;
using std::endl;

DecayChannel::DecayChannel(string md, DecayMode* dm, double q, double i, double t, double pExEn, double dExEn):
modeName(md), decayMode(dm), Q(q), intensity(i), lifetime(t), parentExcitationEnergy(pExEn), daughterExcitationEnergy(dExEn) {
}

std::vector<Particle*> DecayChannel::Decay (Particle* initState) {
  try {
    //cout << "Test " <<  initState->GetName() << " " << Q << endl;
    //cout << mode << endl;
    return decayMode->Decay(initState, Q, daughterExcitationEnergy);
  }
  catch (const std::invalid_argument &e) {
    cout << "Forwarding exception up the stack" << endl;
    throw e;
  }
}
