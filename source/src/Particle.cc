#include "Particle.hh"
#include "DecayChannel.hh"
#include "OptionContainer.hh"
#include <stdlib.h>
#include <stdexcept>
#include <cmath>

std::default_random_engine Particle::randomGen;

Particle::Particle(const std::string& _name, double _mass, int _charge, int _neutrons, double _spin, double _excitationEnergy): name(_name), mass(_mass), charge(_charge), neutrons(_neutrons), spin(_spin), currentExcitationEnergy(_excitationEnergy) {
  //std::cout << "Creating new particle " << name << std::endl;
  fourMomentum.resize(4);
  fourMomentum(0) = mass + currentExcitationEnergy;
}

Particle::Particle(const Particle& orig) {
  //std::cout << "Copy constructor for particle " << orig.name << std::endl;
  fourMomentum.resize(4);
  name = orig.name;
  mass = orig.mass;
  charge = orig.charge;
  neutrons = orig.neutrons;
  spin = orig.spin;
  currentExcitationEnergy = orig.currentExcitationEnergy;
  fourMomentum(0) = mass + currentExcitationEnergy;

  //std::cout << charge << " " << neutrons << std::endl;

  decayChannels.insert(decayChannels.end(), orig.decayChannels.begin(), orig.decayChannels.end());
}

double Particle::GetLifetime() const {
  double t = 1.e23;
  double energyThreshold = GetOpt(double, "Cuts.Energy");
  for(int i = 0; i < decayChannels.size(); ++i) {
    // Look for decay channels from current excitation state
    if (std::abs(decayChannels[i]->GetParentExcitationEnergy()-currentExcitationEnergy) < energyThreshold) {
      t = decayChannels[i]->GetLifetime();
       break;
    }
  }
  return t;
}

double Particle::GetDecayTime() {
  double lifetime = GetLifetime();
  // std::cout << name << " Lifetime " << lifetime << std::endl;
  std::exponential_distribution<double> distribution(1./lifetime);
  double decayTime = distribution(randomGen);
  return decayTime;
}

ublas::vector<double> Particle::GetVelocity() const {
  ublas::vector<double> velocity = Get3Momentum()/fourMomentum(0);
  return velocity;
}

std::vector<Particle*> Particle::Decay() {
  double totalIntensity = 0.;
  double energyThreshold = GetOpt(double, "Cuts.Energy");
  for(std::vector<DecayChannel*>::size_type i = 0; i != decayChannels.size(); i++) {
    // Look for decay channels at current excitation level
    if (std::abs(currentExcitationEnergy - decayChannels[i]->GetParentExcitationEnergy()) < energyThreshold) {
      totalIntensity+=decayChannels[i]->GetIntensity();
    }
  }
  double r = rand()/(double)RAND_MAX*totalIntensity;
  double intensity = 0.;
  double index = 0.;
  // Sample randomly from the different decay channels
  for(std::vector<DecayChannel*>::size_type i = 0; i != decayChannels.size(); i++) {
    if (std::abs(currentExcitationEnergy - decayChannels[i]->GetParentExcitationEnergy()) < energyThreshold) {
      intensity+=decayChannels[i]->GetIntensity();
      if (r <= intensity) {
        break;
      }
    }
    index++;
  }
  try {
    return decayChannels[index]->Decay(this);
  }
  catch (const std::invalid_argument& e) {
    std::cout << "DecayMode " << decayChannels[index]->GetModeName() << " not found." << std::endl;
    std::cout << GetName() << std::endl;

    throw e;
  }
}
