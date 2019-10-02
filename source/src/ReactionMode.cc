#include "ReactionMode.hh"
#include "DecayManager.hh"
#include "PDS/core/DynamicParticle.hh"
#include "Utilities.hh"
#include "SpectrumGenerator.hh"
#include <string>
#include <sstream>

static std::vector<PDS::core::DynamicParticle*> BetaMinus::activate(PDS::core::DynamicParticle* initState, double Q, double daughterExEn) {
  std::vector<PDS::core::DynamicParticle*> finalStates;

  // std::cout << "In BetaMinus Decay " << std::endl;
  // std::cout << "Address: " << initState << std::endl;
  std::ostringstream oss;
  oss << initState->GetParticleDefinition()->GetA() << PDS::util::GetAtomName(initState->GetParticleDefinition()->GetZ());
  //std::cout << oss.str() << std::endl;
  //TODO: design general approach for the creation of a dynamic particle with charge Z+1 from dynamic particle with charge Z
  PDS::core::DynamicParticle* recoil = PDS::ParticleFactory::CreateNewDynamicParticle(PDS::util::GetAtomName(initState->GetParticleDefinition()->GetZ()+1));
  recoil->SetExcitationEnergy(daughterExEn);
  PDS::core::DynamicParticle* e = PDS::ParticleFactory::CreateNewDynamicParticle("e-");
  PDS::core::DynamicParticle* enu = PDS::ParticleFactory::CreateNewDynamicParticle("enubar");

  // std::cout << "Recoil " << recoil->GetCharge() << " " << recoil->GetNeutrons() << " " << recoil << std::endl;

  oss.str("");
  oss.clear();
  oss << "BetaMinus:Z" << recoil->GetParticleDefinition()->GetZ() << "A" << recoil->GetParticleDefinition()->GetA() << "Q" << Q;
  //Work in the COM frame
  ublas::vector<double> elFourMomentum (4);

  std::vector<std::vector<double> >* dist;
  // std::cout << "Try getting distribution" << std::endl;
  try {
    dist = DecayManager::GetInstance().GetDistribution(oss.str());
  } catch (const std::invalid_argument& e) {
    // bool advancedFermi = false;
    // if (OptionContainer::GetInstance().GetOption<std::string>("BetaDecay.FermiFunction") == "Advanced") {
    //   advancedFermi = true;
    // }
    // std::cout << "Distribution not found" << std::endl;
    // std::cout << oss.str() << " " << spectrumGen << std::endl;
    DecayManager::GetInstance().RegisterDistribution(oss.str(), spectrumGen->GenerateSpectrum(initState, recoil, Q));
    dist = DecayManager::GetInstance().GetDistribution(oss.str());
  }

  // std::cout << "Found distribution" << std::endl;

  double mf = 0.;
  double mgt = 0.;
  if (GetOpt(std::string, "BetaDecay.Default") == "Fermi") {
    mf = 1.;
  }
  else {
    mgt = 1.;
  }

  double a = utilities::CalculateBetaNeutrinoAsymmetry(GetOpt(double, "Couplings.CS"),
  GetOpt(double, "Couplings.CT"), GetOpt(double, "Couplings.CV"),
  GetOpt(double, "Couplings.CA"), mf, mgt);
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

  e->SetMomentum(elFourMomentum);

  ublas::vector<double> velocity = -initState->GetVelocity();
  ThreeBodyDecay(velocity, e, enu, recoil, enuDir, Q);

  finalStates.push_back(recoil);
  finalStates.push_back(e);
  finalStates.push_back(enu);

  return finalStates;
}

static std::vector<PDS::core::DynamicParticle*> BetaPlus::activate(PDS::core::DynamicParticle* initState, double Q, double daughterExEn) {
  std::vector<PDS::core::DynamicParticle*> finalStates;

  double E0 = Q-2*utilities::EMASSC2;

  //std::cout << "In BetaMinus Decay " << std::endl;
  //std::cout << "Address: " << initState << std::endl;
  std::ostringstream oss;
  oss << initState->GetCharge()+initState->GetNeutrons() << utilities::atoms[initState->GetCharge()-2];
  PDS::core::DynamicParticle* recoil = DecayManager::GetInstance().GetNewParticle(oss.str(), initState->GetCharge()-1, initState->GetCharge()+initState->GetNeutrons());
  recoil->SetExcitationEnergy(daughterExEn);
  PDS::core::DynamicParticle* pos = DecayManager::GetInstance().GetNewParticle("e+");
  PDS::core::DynamicParticle* enubar = DecayManager::GetInstance().GetNewParticle("enu");

  oss.str("");
  oss.clear();
  oss << "BetaPlus:Z" << recoil->GetCharge() << "A" << recoil->GetCharge() + recoil->GetNeutrons() << "Q" << Q;
  //Work in the COM frame
  ublas::vector<double> enubarDir = utilities::RandomDirection();

  ublas::vector<double> posFourMomentum (4);
  std::vector<std::vector<double> >* dist;
  try {
    dist = DecayManager::GetInstance().GetDistribution(oss.str());
  } catch (const std::invalid_argument& e) {
    // bool advancedFermi = false;
    // if (OptionContainer::GetInstance().GetOption<std::string>("BetaDecay.FermiFunction") == "Advanced") {
    //   advancedFermi = true;
    // }
    DecayManager::GetInstance().RegisterDistribution(oss.str(), spectrumGen->GenerateSpectrum(initState, recoil, E0));
    dist = DecayManager::GetInstance().GetDistribution(oss.str());
  }

  double posEnergy = utilities::RandomFromDistribution(*dist) + utilities::EMASSC2;
  double posMomentum = std::sqrt(posEnergy*posEnergy-std::pow(utilities::EMASSC2, 2.));

  double mf = 0.;
  double mgt = 0.;
  if (GetOpt(std::string, "BetaDecay.Default") == "Fermi") {
    mf = 1.;
  }
  else {
    mgt = 1.;
  }
  double a = utilities::CalculateBetaNeutrinoAsymmetry(GetOpt(double, "Couplings.CS"),
  GetOpt(double, "Couplings.CT"),
  GetOpt(double, "Couplings.CV"),
  GetOpt(double, "Couplings.CA"), mf, mgt);
  std::vector<double> p;
  p.push_back(1.);
  p.push_back(a*posMomentum/posEnergy);
  ublas::vector<double> posDir = utilities::GetParticleDirection(enubarDir, p);
  posFourMomentum(0) = posEnergy;
  posFourMomentum(1) = posMomentum*posDir[0];
  posFourMomentum(2) = posMomentum*posDir[1];
  posFourMomentum(3) = posMomentum*posDir[2];

  pos->SetMomentum(posFourMomentum);

  ublas::vector<double> velocity = -initState->GetVelocity();
  ThreeBodyDecay(velocity, pos, enubar, recoil, enubarDir, E0);


  finalStates.push_back(recoil);
  finalStates.push_back(pos);
  finalStates.push_back(enubar);

  return finalStates;
}

static std::vector<PDS::core::DynamicParticle*> ConversionElectron::activate(PDS::core::DynamicParticle* initState, double Q, double daughterExEn) {
  std::vector<PDS::core::DynamicParticle*> finalStates;

  ublas::vector<double> velocity = -initState->GetVelocity();
  PDS::core::DynamicParticle* recoil = DecayManager::GetInstance().GetNewParticle(initState->GetRawName());
  PDS::core::DynamicParticle* e = DecayManager::GetInstance().GetNewParticle("e+");
  recoil->SetExcitationEnergy(daughterExEn);

  TwoBodyDecay(velocity, recoil, e, Q);

  finalStates.push_back(recoil);
  finalStates.push_back(e);

  return finalStates;
}

static std::vector<PDS::core::DynamicParticle*> Proton::activate(PDS::core::DynamicParticle* initState, double Q, double daughterExEn) {
  std::vector<PDS::core::DynamicParticle*> finalStates;

  std::ostringstream oss;
  oss << initState->GetCharge()+initState->GetNeutrons() - 1 << utilities::atoms[initState->GetCharge()-2];
  PDS::core::DynamicParticle* recoil = DecayManager::GetInstance().GetNewParticle(oss.str(), initState->GetCharge()-1, initState->GetCharge()+initState->GetNeutrons()-1);
  PDS::core::DynamicParticle* p = DecayManager::GetInstance().GetNewParticle("p");
  recoil->SetExcitationEnergy(daughterExEn);

  ublas::vector<double> velocity = -initState->GetVelocity();
  TwoBodyDecay(velocity, recoil, p, Q);

  finalStates.push_back(recoil);
  finalStates.push_back(p);

  return finalStates;
}

static std::vector<PDS::core::DynamicParticle*> Alpha::activate(PDS::core::DynamicParticle* initState, double Q, double daughterExEn) {
  std::vector<PDS::core::DynamicParticle*> finalStates;

  std::ostringstream oss;
  oss << initState->GetCharge()+initState->GetNeutrons() - 4 << utilities::atoms[initState->GetCharge()-3];
  PDS::core::DynamicParticle* recoil = DecayManager::GetInstance().GetNewParticle(oss.str(), initState->GetCharge()-2, initState->GetCharge()+initState->GetNeutrons()-4);
  PDS::core::DynamicParticle* alpha = DecayManager::GetInstance().GetNewParticle("alpha");
  recoil->SetExcitationEnergy(daughterExEn);

  ublas::vector<double> velocity = -initState->GetVelocity();
  TwoBodyDecay(velocity, recoil, alpha, Q);

  finalStates.push_back(recoil);
  finalStates.push_back(alpha);

  return finalStates;
}

static std::vector<PDS::core::DynamicParticle*> Gamma::activate(PDS::core::DynamicParticle* initState, double Q, double daughterExEn) {
  std::vector<PDS::core::DynamicParticle*> finalStates;

  ublas::vector<double> velocity = -initState->GetVelocity();
  PDS::core::DynamicParticle* recoil = DecayManager::GetInstance().GetNewParticle(initState->GetRawName());
  PDS::core::DynamicParticle* gamma = DecayManager::GetInstance().GetNewParticle("gamma");
  recoil->SetExcitationEnergy(daughterExEn);

  TwoBodyDecay(velocity, recoil, gamma, Q);

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
