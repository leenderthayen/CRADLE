#include "DecayChannel.hh"
#include "DecayMode.hh"
#include "DecayManager.hh"
#include <Particle.hh>
#include <iostream>
#include <stdexcept>

using std::cout;
using std::endl;

DecayChannel::DecayChannel(string md, double q, double i, double t, double pExEn, double dExEn): mode(md), Q(q), intensity(i), lifetime(t), parentExcitationEnergy(pExEn), daughterExcitationEnergy(dExEn) {
}

std::vector<Particle*> DecayChannel::Decay (Particle* initState) {
  try {
    //cout << "Test " <<  initState->GetName() << " " << Q << endl;
    //cout << mode << endl;
    return DecayManager::GetInstance().GetDecayMode(mode).Decay(initState, Q, daughterExcitationEnergy);
  }
  catch (const std::invalid_argument &e) {
    cout << "Forwarding exception up the stack" << endl;
    throw e;
  }
}
