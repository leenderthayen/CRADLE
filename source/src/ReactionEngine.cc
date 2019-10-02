#include "ReactionEngine.hh"
#include "PDS/core/DynamicParticle.h"
#include "PDS/core/ReactionChannel.h"
#include "ReactionMode.hh"
#include "SpectrumGenerator.hh"

#include <stdlib.h>
#include <stdexcept>
#include <cmath>

std::default_random_engine ReactionEngine::randomGen;

ReactionEngine::ReactionEngine(Cuts _cuts) : cuts(_cuts){
  RegisterBasicSpectrumGenerators();
  RegisterBasicReactionModes();
}

ReactionEngine::~ReactionEngine(){}

void ReactionEngine::RegisterSpectrumGenerator(ReactionModeNames modeName, SpectrumGenerator& sg) {
  try {
    registeredSpectrumGeneratorMap.insert(modeName,sg);
  }
  catch (const invalid_argument &e) {
    cout << "Cannot register" <<  typeid(sg).name() << "spectrum generator. Invalid mode name." << endl;
  }
}

void ReactionEngine::RegisterBasicSpectrumGenerators() {
  RegisterSpectrumGenerator(ReactionModeNames::Proton, DeltaSpectrumGenerator::GetInstance());
  RegisterSpectrumGenerator(ReactionModeNames::Alpha, DeltaSpectrumGenerator::GetInstance());
  RegisterSpectrumGenerator(ReactionModeNames::Gamma, DeltaSpectrumGenerator::GetInstance());
  RegisterSpectrumGenerator(ReactionModeNames::InternalConversion, DeltaSpectrumGenerator::GetInstance());
  RegisterSpectrumGenerator(ReactionModeNames::BetaPlus, SimpleBetaDecay::GetInstance());
  RegisterSpectrumGenerator(ReactionModeNames::BetaMinus, SimpleBetaDecay::GetInstance());
}

void ReactionEngine::RegisterReactionMode(ReactionModeNames modeName, activator act){
  try {
    registeredReactionModeMap.insert(modeName,act);
  }
  catch (const invalid_argument &e) {
    cout << "Cannot register" <<  typeid(sg).name() << "activator method. Invalid mode name." << endl;
  }
}

void ReactionEngine::RegisterBasicReactionModes(){
  RegisterReactionMode(ReactionModeNames::Proton, &Proton::activate);
  RegisterReactionMode(ReactionModeNames::Alpha, &Alpha::activate);
  RegisterReactionMode(ReactionModeNames::Gamma, &Gamma::activate);
  RegisterReactionMode(ReactionModeNames::InternalConversion, &InternalConversion::activate);
  RegisterReactionMode(ReactionModeNames::BetaPlus, &BetaPlus::activate);
  RegisterReactionMode(ReactionModeNames::BetaMinus, &BetaMinus::activate);
}

std::string ReactionEngine::GenerateEvent(int eventNr, std::string initStateName, double initExcitationEn) {
  double time = 0.;
  ostringstream eventDataSS;
  std::vector<PDS::core::DynamicParticle*> particleStack;
  PDS::core::DynamicParticle* ini = PDS::ParticleFactory::CreateNewDynamicParticle(initStateName,initExcitationEn);
  particleStack.push_back(ini);
  while (!particleStack.empty()) {
    PDS::core::DynamicParticle* dp = particleStack.back();
    //cout << "Decaying particle " << p->GetName() << endl;
    vector<PDS::core::DynamicParticle*> finalStates;
    //TODO: add similar method in PDS
    double decayTime = GetDecayTime(dp->getParticle()->GetCurrentState()->GetLifetime());

    if ((time + decayTime) <= cuts.Lifetime) {
      try {
        finalStates = Decay(dp);
        time += decayTime;
        // cout << "Decay finished" << endl;
      } catch (const std::invalid_argument& e) {
        // cout << "Decay Mode for particle " << p->GetName() << " not found.
        // Aborting." << endl;
        return "";
      }
    } else {
      // cout << "Particle " << p->GetName() << " is stable" << endl;
      eventDataSS << eventNr << "\t" << time << "\t" << p->GetInfoForFile() << "\n";
    }
    delete particleStack.back();
    particleStack.pop_back();
    if (!finalStates.empty()) {
      particleStack.insert(particleStack.end(), finalStates.begin(),
                           finalStates.end());
    }
  }
  return eventDataSS.str();
}

std::vector<PDS::core::DynamicParticle *> ReactionEngine::Decay(PDS::core::DynamicParticle* dp){
  double totalIntensity = 0.;
  double energyThreshold = cuts.Energy;
  std::vector<PDS::core::ReactionChannel *> decayChannels = dp->GetParticleDefinition()->GetReactionChannels();
  double currentExcitationEnergy = dp->getParticle().GetExcitationEnergy();
  for(std::vector<PDS::core::ReactionChannel*>::size_type i = 0; i != decayChannels.size(); i++) {
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
    if (std::abs(dp->GetParticle()->GetExcitationEnergy() - decayChannels[i]->GetParentExcitationEnergy()) < energyThreshold) {
      intensity+=decayChannels[i]->GetIntensity();
      if (r <= intensity) {
        break;
      }
    }
    index++;
  }

  auto it = registeredReactionModeMap.find( decayChannels[index]->GetReactionModeName());
  if (it != registeredReactionModeMap.end()){
    auto it2 = registeredSpectrumGeneratorMap.find(decayChannels[index]->GetReactionModeName());
    if(it2 != registeredSpectrumGeneratorMap.end()){
      return (*(it->second))(dp,currentExcitationEnergy,decayChannels[index]->GetQValue(),it2->second());
    }
    else{
      return (*(it->second))(dp,currentExcitationEnergy,decayChannels[index]->GetQValue(),nullptr);
    }
  }
  else{
    throw "No such decay mode was registered!!";
  }

}

double ReactionEngine::GetDecayTime(double lifetime) {
  std::exponential_distribution<double> distribution(1./lifetime);
  double decayTime = distribution(randomGen);
  return decayTime;
}
