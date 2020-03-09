#include "CRADLE/ReactionMode.h"
#include "CRADLE/Cradle.h"
#include "CRADLE/ConfigParser.h"
#include "CRADLE/SpectrumGenerator.h"
#include "PDS/Core/DynamicParticle.h"
#include "PDS/Factory/ParticleFactory.h"
#include "PDS/Util/Atoms.h"
#include "CRADLE/Utilities.h"

#include <string>
#include <sstream>

namespace ublas = boost::numeric::ublas;

namespace CRADLE {

std::vector<PDS::core::DynamicParticle> BetaMinus::activate(PDS::core::DynamicParticle& initState, double Q, double daughterExEn, SpectrumGenerator& sg, CouplingConstants couplingConstants, BetaDecay betaDecay) {

  PDS::core::Nucleus* initNucleusDef = static_cast<PDS::core::Nucleus*>(initState.GetParticle().GetParticleDefinition());
  std::ostringstream oss;
  oss << initNucleusDef->GetName();
  //TODO: design general approach for the creation of a dynamic particle with charge Z+1 from dynamic particle with charge Z
  PDS::core::DynamicParticle recoil = PDS::ParticleFactory::GetNewDynamicParticleFromGeant4(initNucleusDef->GetZ()+1,initNucleusDef->GetA(),daughterExEn);
  PDS::core::DynamicParticle e = PDS::ParticleFactory::CreateNewDynamicParticle("e-",0);
  PDS::core::DynamicParticle enubar = PDS::ParticleFactory::CreateNewDynamicParticle("enubar",0);

  // std::cout << "Recoil " << recoil->GetCharge() << " " << recoil->GetNeutrons() << " " << recoil << std::endl;

  oss.str("");
  oss.clear();
  PDS::core::Nucleus* recoilNucleusDef = static_cast<PDS::core::Nucleus*>(recoil.GetParticle().GetParticleDefinition());
  oss << "BetaMinus:Z" << recoilNucleusDef->GetZ() << "A" << recoilNucleusDef->GetA() << "Q" << Q;
  //Work in the COM frame
  ublas::vector<double> elFourMomentum (4);

  std::vector<std::vector<double> >* dist;
  // std::cout << "Try getting distribution" << std::endl;
  try {
    dist = sg.GetDistribution(oss.str());
  } catch (const std::invalid_argument& e) {
    // bool advancedFermi = false;
    // if (OptionContainer::GetInstance().GetOption<std::string>("BetaDecay.FermiFunction") == "Advanced") {
    //   advancedFermi = true;
    // }
    // std::cout << "Distribution not found" << std::endl;
    // std::cout << oss.str() << " " << spectrumGen << std::endl;
    sg.RegisterDistribution(oss.str(), sg.GenerateSpectrum(initState.GetParticle(), recoil.GetParticle(), Q));
    dist = sg.GetDistribution(oss.str());
  }

  // std::cout << "Found distribution" << std::endl;

  double mf = 0.;
  double mgt = 0.;
  if (betaDecay.Default == "Fermi") {
    mf = 1.;
  }
  else {
    mgt = 1.;
  }

  double a = utilities::CalculateBetaNeutrinoAsymmetry(couplingConstants.CS, couplingConstants.CT, couplingConstants.CV, couplingConstants.CA, mf, mgt);
  std::vector<double> p;

  double elEnergy = utilities::RandomFromDistribution(*dist)+utilities::EMASSC2;
  double elMomentum = std::sqrt(elEnergy*elEnergy-std::pow(utilities::EMASSC2, 2.));
  ublas::vector<double> enuDir = utilities::RandomDirection();

  p.push_back(1.);
  p.push_back(a*elMomentum/elEnergy);
  ublas::vector<double> eDir = utilities::GetParticleDirection(enuDir, p);

  elFourMomentum(0) = elEnergy;
  elFourMomentum(1) = elMomentum*eDir[0];
  elFourMomentum(2) = elMomentum*eDir[1];
  elFourMomentum(3) = elMomentum*eDir[2];

  e.SetFourMomentum(elFourMomentum);

  ublas::vector<double> velocity = -initState.GetVelocity();
  utilities::ThreeBodyDecay(velocity, e, enubar, recoil, enuDir, Q);

  std::vector<PDS::core::DynamicParticle> finalStates;
  finalStates.push_back(recoil);
  finalStates.push_back(e);
  finalStates.push_back(enubar);

  return finalStates;
}

std::vector<PDS::core::DynamicParticle> BetaPlus::activate(PDS::core::DynamicParticle& initState, double Q, double daughterExEn, SpectrumGenerator& sg, CouplingConstants couplingConstants, BetaDecay betaDecay) {
  PDS::core::Nucleus* initNucleusDef = static_cast<PDS::core::Nucleus*>(initState.GetParticle().GetParticleDefinition());
  // std::cout << "In BetaMinus Decay " << std::endl;
  // std::cout << "Address: " << initState << std::endl;
  std::ostringstream oss;
  oss << initNucleusDef->GetName();
  //std::cout << oss.str() << std::endl;
  //TODO: design general approach for the creation of a dynamic particle with charge Z+1 from dynamic particle with charge Z
  PDS::core::DynamicParticle recoil = PDS::ParticleFactory::GetNewDynamicParticleFromGeant4(initNucleusDef->GetZ()-1,initNucleusDef->GetA(),daughterExEn);
  PDS::core::DynamicParticle pos = PDS::ParticleFactory::CreateNewDynamicParticle("e+",0);
  PDS::core::DynamicParticle enu = PDS::ParticleFactory::CreateNewDynamicParticle("enu",0);

  oss.str("");
  oss.clear();
  PDS::core::Nucleus* recoilNucleusDef = static_cast<PDS::core::Nucleus*>(recoil.GetParticle().GetParticleDefinition());
  oss << "BetaMinus:Z" << recoilNucleusDef->GetZ() << "A" << recoilNucleusDef->GetA() << "Q" << Q;
  //Work in the COM frame

  std::vector<std::vector<double> >* dist;
  // std::cout << "Try getting distribution" << std::endl;
  try {
    dist = sg.GetDistribution(oss.str());
  } catch (const std::invalid_argument& e) {
    // bool advancedFermi = false;
    // if (OptionContainer::GetInstance().GetOption<std::string>("BetaDecay.FermiFunction") == "Advanced") {
    //   advancedFermi = true;
    // }
    // std::cout << "Distribution not found" << std::endl;
    // std::cout << oss.str() << " " << spectrumGen << std::endl;
    sg.RegisterDistribution(oss.str(), sg.GenerateSpectrum(initState.GetParticle(), recoil.GetParticle(), Q));
    dist = sg.GetDistribution(oss.str());
  }

  double mf = 0.;
  double mgt = 0.;
  if (betaDecay.Default == "Fermi") {
    mf = 1.;
  }
  else {
    mgt = 1.;
  }

  double a = utilities::CalculateBetaNeutrinoAsymmetry(couplingConstants.CS, couplingConstants.CT, couplingConstants.CV, couplingConstants.CA, mf, mgt);

  double posEnergy = utilities::RandomFromDistribution(*dist) + utilities::EMASSC2;
  double posMomentum = std::sqrt(posEnergy*posEnergy-std::pow(utilities::EMASSC2, 2.));
  ublas::vector<double> enuDir = utilities::RandomDirection();

  std::vector<double> p;
  p.push_back(1.);
  p.push_back(a*posMomentum/posEnergy);
  ublas::vector<double> posDir = utilities::GetParticleDirection(enuDir, p);

  ublas::vector<double> posFourMomentum (4);
  posFourMomentum(0) = posEnergy;
  posFourMomentum(1) = posMomentum*posDir[0];
  posFourMomentum(2) = posMomentum*posDir[1];
  posFourMomentum(3) = posMomentum*posDir[2];

  pos.SetFourMomentum(posFourMomentum);

  ublas::vector<double> velocity = -initState.GetVelocity();

  double E0 = Q-2*utilities::EMASSC2;
  utilities::ThreeBodyDecay(velocity, pos, enu, recoil, enuDir, E0);

  std::vector<PDS::core::DynamicParticle> finalStates;
  finalStates.push_back(recoil);
  finalStates.push_back(pos);
  finalStates.push_back(enu);

  return finalStates;
}

std::vector<PDS::core::DynamicParticle> ConversionElectron::activate(PDS::core::DynamicParticle& initState, double Q, double daughterExEn, SpectrumGenerator&, CouplingConstants, BetaDecay) {
  std::vector<PDS::core::DynamicParticle> finalStates;

  ublas::vector<double> velocity = -initState.GetVelocity();
  PDS::core::DynamicParticle recoil = PDS::ParticleFactory::CreateNewDynamicParticle(initState.GetParticle().GetParticleDefinition()->GetName(),daughterExEn);
  PDS::core::DynamicParticle e = PDS::ParticleFactory::CreateNewDynamicParticle("e-",0);
  utilities::TwoBodyDecay(velocity, recoil, e, Q);

  finalStates.push_back(recoil);
  finalStates.push_back(e);

  return finalStates;
}

std::vector<PDS::core::DynamicParticle> Proton::activate(PDS::core::DynamicParticle& initState, double Q, double daughterExEn, SpectrumGenerator&, CouplingConstants, BetaDecay) {
  PDS::core::Nucleus* initNucleusDef = static_cast<PDS::core::Nucleus*>(initState.GetParticle().GetParticleDefinition());
  std::ostringstream oss;
  oss << initState.GetParticle().GetParticleDefinition()->GetName();
  PDS::core::DynamicParticle recoil = PDS::ParticleFactory::GetNewDynamicParticleFromGeant4(initNucleusDef->GetZ()-1,initNucleusDef->GetA(),daughterExEn);
  PDS::core::DynamicParticle p = PDS::ParticleFactory::CreateNewDynamicParticle("p",0);

  ublas::vector<double> velocity = -initState.GetVelocity();
  utilities::TwoBodyDecay(velocity, recoil, p, Q);

  std::vector<PDS::core::DynamicParticle> finalStates;
  finalStates.push_back(recoil);
  finalStates.push_back(p);

  return finalStates;
}

std::vector<PDS::core::DynamicParticle> Alpha::activate(PDS::core::DynamicParticle& initState, double Q, double daughterExEn, SpectrumGenerator&, CouplingConstants, BetaDecay) {
  PDS::core::Nucleus* initNucleusDef = static_cast<PDS::core::Nucleus*>(initState.GetParticle().GetParticleDefinition());
  std::ostringstream oss;
  oss << initState.GetParticle().GetParticleDefinition()->GetName();
  PDS::core::DynamicParticle recoil = PDS::ParticleFactory::GetNewDynamicParticleFromGeant4(initNucleusDef->GetZ()-2,initNucleusDef->GetA()-4,daughterExEn);
  PDS::core::DynamicParticle alpha = PDS::ParticleFactory::CreateNewDynamicParticle("alpha",0);

  ublas::vector<double> velocity = -initState.GetVelocity();
  utilities::TwoBodyDecay(velocity, recoil, alpha, Q);

  std::vector<PDS::core::DynamicParticle> finalStates;
  finalStates.push_back(recoil);
  finalStates.push_back(alpha);

  return finalStates;
}

std::vector<PDS::core::DynamicParticle> Gamma::activate(PDS::core::DynamicParticle& initState, double Q, double daughterExEn, SpectrumGenerator&, CouplingConstants, BetaDecay) {
  std::vector<PDS::core::DynamicParticle> finalStates;

  ublas::vector<double> velocity = -initState.GetVelocity();
  PDS::core::DynamicParticle recoil = PDS::ParticleFactory::CreateNewDynamicParticle(initState.GetParticle().GetParticleDefinition()->GetName(),daughterExEn);
  PDS::core::DynamicParticle gamma = PDS::ParticleFactory::CreateNewDynamicParticle("gamma",0);

  utilities::TwoBodyDecay(velocity, recoil, gamma, Q);

  finalStates.push_back(recoil);
  finalStates.push_back(gamma);

  return finalStates;
}

ReactionMode::ReactionMode() { }

ReactionMode::~ReactionMode() { }

BetaMinus::BetaMinus() { }

BetaPlus::BetaPlus() { }

ConversionElectron::ConversionElectron() { }

Proton::Proton () { }

Alpha::Alpha () { }

Gamma::Gamma () { }

}//end of CRADLE namespace
