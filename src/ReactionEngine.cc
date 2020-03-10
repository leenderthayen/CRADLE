#include "CRADLE/ReactionEngine.h"
#include "CRADLE/ReactionMode.h"

#include <stdexcept>

namespace CRADLE {

  ReactionEngine::ReactionEngine(){
    RegisterBasicReactionModes();
  }

  Event ReactionEngine::ProcessParticle(PDS::core::DynamicParticle& initState) {
    Event event;
    std::vector<PDS::core::DynamicParticle> finalStates = ProcessDecay(initState);

    return event;
  }

  std::vector<PDS::core::DynamicParticle> ReactionEngine::ProcessDecay(PDS::core::DynamicParticle& initState) {
    //Decay time in center of mass frame
    double tauDecay = 0.;
  }

  void ReactionEngine::RegisterReactionMode(PDS::core::ReactionModeName modeName, ReactionMode& reactionMode){
    try {
      auto it = reactionDictionary.find(modeName);
      if (it != registeredReactionModeMap.end())
        registeredReactionModeMap[modeName] = reactionMode;
      else
        registeredReactionModeMap.insert({modeName, reactionMode});
    } catch (const std::invalid_argument &e) {
          std::cout << "Cannot register activator method. Invalid mode name." << std::endl;
    }
  }

  void ReactionEngine::RegisterDefaultReactionModes() {
    RegisterReactionMode(PDS::core::ReactionModeName::Proton, ReactionModeFactory::DefaultProtonSeparation());
    RegisterReactionMode(PDS::core::ReactionModeName::Alpha, ReactionModeFactory::DefaultAlpha());
    RegisterReactionMode(PDS::core::ReactionModeName::Gamma, ReactionModeFactory::DefaultGamma());
    //RegisterReactionMode(PDS::core::ReactionModeName::InternalConversion, &ConversionElectron::activate);
    RegisterReactionMode(PDS::core::ReactionModeName::BetaPlus, ReactionModeFactory::DefaultBetaPlus());
    RegisterReactionMode(PDS::core::ReactionModeName::BetaMinus, ReactionModeFactory::DefaultBetaMinus());
  }

// std::string ReactionEngine::GenerateEvent(int eventNr, std::string initStateName, double initExcitationEn, ConfigOptions configOptions) {
//   double time = 0.;
//   std::ostringstream eventDataSS;
//   std::vector<PDS::core::DynamicParticle> particleStack;
//   PDS::core::DynamicParticle ini = PDS::ParticleFactory::CreateNewDynamicParticle(initStateName,initExcitationEn);
//   particleStack.push_back(ini);
//   while (!particleStack.empty()) {
//     PDS::core::DynamicParticle dp = particleStack.back();
//     //cout << "Decaying particle " << p->GetName() << endl;
//     std::vector<PDS::core::DynamicParticle> finalStates;
//     //TODO: add similar method in PDS
//     double decayTime = GetDecayTime(dp.GetParticle().GetLifetime());
//
//     if ((time + decayTime) <= configOptions.cuts.Lifetime) {
//       try {
//         finalStates = Decay(dp,configOptions);
//         time += decayTime;
//         // cout << "Decay finished" << endl;
//       } catch (const std::invalid_argument& e) {
//         // cout << "Decay Mode for particle " << p->GetName() << " not found.
//         // Aborting." << endl;
//         return "";
//       }
//     } else {
//       // cout << "Particle " << p->GetName() << " is stable" << endl;
//       eventDataSS << eventNr << "\t" << time << "\t" << GetInfoForFile(dp) << "\n";
//     }
// //    delete particleStack.back();
//     particleStack.pop_back();
//     if (!finalStates.empty()) {
//       particleStack.insert(particleStack.end(), finalStates.begin(),
//                            finalStates.end());
//     }
//   }
//   return eventDataSS.str();
// }
//
// std::vector<PDS::core::DynamicParticle > ReactionEngine::Decay(PDS::core::DynamicParticle dp, ConfigOptions configOptions){
//   double totalIntensity = 0.;
//   double energyThreshold = configOptions.cuts.Energy;
//   std::vector<PDS::core::ReactionChannel> decayChannels = dp.GetParticle().GetParticleDefinition()->GetReactionChannels();
//   double currentExcitationEnergy = dp.GetParticle().GetExcitationEnergy();
//   for(std::vector<PDS::core::ReactionChannel>::size_type i = 0; i != decayChannels.size(); i++) {
//     // Look for decay channels at current excitation level
//     if (std::abs(currentExcitationEnergy - decayChannels[i].GetInitialExcitationEnergy()) < energyThreshold) {
//       totalIntensity+=decayChannels[i].GetIntensity();
//     }
//   }
//   double r = rand()/(double)RAND_MAX*totalIntensity;
//   double intensity = 0.;
//   double index = 0.;
//   // Sample randomly from the different decay channels
//   for(std::vector<PDS::core::ReactionChannel>::size_type i = 0; i != decayChannels.size(); i++) {
//     if (std::abs(dp.GetParticle().GetExcitationEnergy() - decayChannels[i].GetInitialExcitationEnergy()) < energyThreshold) {
//       intensity+=decayChannels[i].GetIntensity();
//       if (r <= intensity) {
//         break;
//       }
//     }
//     index++;
//   }
//
//   auto it = registeredReactionModeMap.find(decayChannels[index].GetReactionModeName());
//   if (it != registeredReactionModeMap.end()){
//     auto it2 = registeredSpectrumGeneratorMap.find(decayChannels[index].GetReactionModeName());
//     if(it2 != registeredSpectrumGeneratorMap.end()){
//       return (*(it->second))(dp,currentExcitationEnergy,decayChannels[index].GetQValue(),it2->second,configOptions.couplingConstants,configOptions.betaDecay);
//     }
//     else{
//       SpectrumGenerator* sg = nullptr;
//       return (*(it->second))(dp,currentExcitationEnergy,decayChannels[index].GetQValue(),*sg,configOptions.couplingConstants,configOptions.betaDecay);
//     }
//   }
//   else{
//     throw "No such decay mode was registered!!";
//   }
// }
//
// inline double ReactionEngine::GetDecayTime(double lifetime) const{
//   std::exponential_distribution<double> distribution(1./lifetime);
//   double decayTime = distribution(randomGen);
//   return decayTime;
// }
//
// inline std::string ReactionEngine::GetInfoForFile(PDS::core::DynamicParticle dp) const {
//   std::string n;
//   std::ostringstream oss(n);
//   ublas::vector<double> fourMomentum = dp.GetFourMomentum();
//   oss << dp.GetName() << "\t" << dp.GetParticle().GetExcitationEnergy() << "\t" << dp.GetKinEnergy() << "\t" << fourMomentum(0) << "\t" << fourMomentum(1) << "\t" << fourMomentum(2) << "\t" << fourMomentum(3);
//   return oss.str();
// }

}//end of CRADLE namespace
