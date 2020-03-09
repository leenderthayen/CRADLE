#include "CRADLE/ReactionEngine.h"
#include "CRADLE/ReactionMode.h"
#include "CRADLE/SpectrumGenerator.h"
#include "CRADLE/ConfigParser.h"

#include "PDS/Core/DynamicParticle.h"
#include "PDS/Core/ReactionChannel.h"
#include "PDS/Core/ParticleDefinition.h"
#include "PDS/Factory/ParticleFactory.h"

#include <stdlib.h>
#include <stdexcept>
#include <cmath>
#include <map>

namespace ublas = boost::numeric::ublas;
//namespace modes = PDS::core::ReactionModeName;

namespace CRADLE {

std::default_random_engine ReactionEngine::randomGen;

ReactionEngine::ReactionEngine(){
  RegisterBasicSpectrumGenerators();
  RegisterBasicReactionModes();
}

ReactionEngine::~ReactionEngine(){}

void ReactionEngine::RegisterSpectrumGenerator(PDS::core::ReactionModeName modeName, SpectrumGenerator& sg) {
  try {
    auto it = registeredSpectrumGeneratorMap.find(modeName);
    if (it != registeredSpectrumGeneratorMap.end())
      registeredSpectrumGeneratorMap.erase(it);
    registeredSpectrumGeneratorMap.insert({modeName,sg});
  }
    catch (const std::invalid_argument &e) {
      std::cout << "Cannot register spectrum generator. Invalid mode name." << std::endl;
  }
}

void ReactionEngine::RegisterBasicSpectrumGenerators() {
  RegisterSpectrumGenerator(PDS::core::ReactionModeName::Proton, DeltaSpectrumGenerator::GetInstance());
  RegisterSpectrumGenerator(PDS::core::ReactionModeName::Alpha, DeltaSpectrumGenerator::GetInstance());
  RegisterSpectrumGenerator(PDS::core::ReactionModeName::Gamma, DeltaSpectrumGenerator::GetInstance());
  RegisterSpectrumGenerator(PDS::core::ReactionModeName::InternalConversion, DeltaSpectrumGenerator::GetInstance());
  RegisterSpectrumGenerator(PDS::core::ReactionModeName::BetaPlus, SimpleBetaDecay::GetInstance());
  RegisterSpectrumGenerator(PDS::core::ReactionModeName::BetaMinus, SimpleBetaDecay::GetInstance());
}

void ReactionEngine::RegisterReactionMode(PDS::core::ReactionModeName modeName, activator act){
  try {
    auto it = registeredReactionModeMap.find(modeName);
    if (it != registeredReactionModeMap.end())
      registeredReactionModeMap[modeName] = act;
    else
      registeredReactionModeMap.insert({modeName,act});
  }
    catch (const std::invalid_argument &e) {
        std::cout << "Cannot register activator method. Invalid mode name." << std::endl;
  }
}

void ReactionEngine::RegisterBasicReactionModes(){
  RegisterReactionMode(PDS::core::ReactionModeName::Proton, &Proton::activate);
  RegisterReactionMode(PDS::core::ReactionModeName::Alpha, &Alpha::activate);
  RegisterReactionMode(PDS::core::ReactionModeName::Gamma, &Gamma::activate);
  RegisterReactionMode(PDS::core::ReactionModeName::InternalConversion, &ConversionElectron::activate);
  RegisterReactionMode(PDS::core::ReactionModeName::BetaPlus, &BetaPlus::activate);
  RegisterReactionMode(PDS::core::ReactionModeName::BetaMinus, &BetaMinus::activate);
}

std::string ReactionEngine::GenerateEvent(int eventNr, std::string initStateName, double initExcitationEn, ConfigOptions configOptions) {
  double time = 0.;
  std::ostringstream eventDataSS;
  std::vector<PDS::core::DynamicParticle> particleStack;
  PDS::core::DynamicParticle ini = PDS::ParticleFactory::CreateNewDynamicParticle(initStateName,initExcitationEn);
  particleStack.push_back(ini);
  while (!particleStack.empty()) {
    PDS::core::DynamicParticle dp = particleStack.back();
    //cout << "Decaying particle " << p->GetName() << endl;
    std::vector<PDS::core::DynamicParticle> finalStates;
    //TODO: add similar method in PDS
    double decayTime = GetDecayTime(dp.GetParticle().GetLifetime());

    if ((time + decayTime) <= configOptions.cuts.Lifetime) {
      try {
        finalStates = Decay(dp,configOptions);
        time += decayTime;
        // cout << "Decay finished" << endl;
      } catch (const std::invalid_argument& e) {
        // cout << "Decay Mode for particle " << p->GetName() << " not found.
        // Aborting." << endl;
        return "";
      }
    } else {
      // cout << "Particle " << p->GetName() << " is stable" << endl;
      eventDataSS << eventNr << "\t" << time << "\t" << GetInfoForFile(dp) << "\n";
    }
//    delete particleStack.back();
    particleStack.pop_back();
    if (!finalStates.empty()) {
      particleStack.insert(particleStack.end(), finalStates.begin(),
                           finalStates.end());
    }
  }
  return eventDataSS.str();
}

std::vector<PDS::core::DynamicParticle > ReactionEngine::Decay(PDS::core::DynamicParticle dp, ConfigOptions configOptions){
  double totalIntensity = 0.;
  double energyThreshold = configOptions.cuts.Energy;
  std::vector<PDS::core::ReactionChannel> decayChannels = dp.GetParticle().GetParticleDefinition()->GetReactionChannels();
  double currentExcitationEnergy = dp.GetParticle().GetExcitationEnergy();
  for(std::vector<PDS::core::ReactionChannel>::size_type i = 0; i != decayChannels.size(); i++) {
    // Look for decay channels at current excitation level
    if (std::abs(currentExcitationEnergy - decayChannels[i].GetInitialExcitationEnergy()) < energyThreshold) {
      totalIntensity+=decayChannels[i].GetIntensity();
    }
  }
  double r = rand()/(double)RAND_MAX*totalIntensity;
  double intensity = 0.;
  double index = 0.;
  // Sample randomly from the different decay channels
  for(std::vector<PDS::core::ReactionChannel>::size_type i = 0; i != decayChannels.size(); i++) {
    if (std::abs(dp.GetParticle().GetExcitationEnergy() - decayChannels[i].GetInitialExcitationEnergy()) < energyThreshold) {
      intensity+=decayChannels[i].GetIntensity();
      if (r <= intensity) {
        break;
      }
    }
    index++;
  }

  auto it = registeredReactionModeMap.find(decayChannels[index].GetReactionModeName());
  if (it != registeredReactionModeMap.end()){
    auto it2 = registeredSpectrumGeneratorMap.find(decayChannels[index].GetReactionModeName());
    if(it2 != registeredSpectrumGeneratorMap.end()){
      return (*(it->second))(dp,currentExcitationEnergy,decayChannels[index].GetQValue(),it2->second,configOptions.couplingConstants,configOptions.betaDecay);
    }
    else{
      SpectrumGenerator* sg = nullptr;
      return (*(it->second))(dp,currentExcitationEnergy,decayChannels[index].GetQValue(),*sg,configOptions.couplingConstants,configOptions.betaDecay);
    }
  }
  else{
    throw "No such decay mode was registered!!";
  }

}

inline double ReactionEngine::GetDecayTime(double lifetime) const{
  std::exponential_distribution<double> distribution(1./lifetime);
  double decayTime = distribution(randomGen);
  return decayTime;
}

inline std::string ReactionEngine::GetInfoForFile(PDS::core::DynamicParticle dp) const {
  std::string n;
  std::ostringstream oss(n);
  ublas::vector<double> fourMomentum = dp.GetFourMomentum();
  oss << dp.GetName() << "\t" << dp.GetParticle().GetExcitationEnergy() << "\t" << dp.GetKinEnergy() << "\t" << fourMomentum(0) << "\t" << fourMomentum(1) << "\t" << fourMomentum(2) << "\t" << fourMomentum(3);
  return oss.str();
}

}//end of CRADLE namespace
