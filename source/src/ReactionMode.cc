#include "ReactionMode.hh"
#include "DecayManager.hh"
#include "PDS/core/DynamicParticle.hh"
#include "Utilities.hh"
#include "OptionContainer.hh"
#include "SpectrumGenerator.hh"
#include <string>
#include <sstream>

void ReactionMode::ThreeBodyDecay(ublas::vector<double>& velocity, PDS::core::DynamicParticle* finalState1, PDS::core::DynamicParticle* finalState2, PDS::core::DynamicParticle* finalState3, ublas::vector<double>& dir2, double Q) {
  //Perform decay in CoM frame
  ublas::vector<double> momentum1 = finalState1->GetFourMomentum();
  ublas::vector<double> momentum2 (4);
  ublas::vector<double> momentum3 (4);

  ublas::vector<double> p2 (3);
  double p2Norm = 0.;

  double mass1 = finalState1->GetRestMass();
  double mass2 = finalState2->GetRestMass();
  double mass3 = finalState3->GetRestMass();
  ublas::vector<double> p1 = finalState1->Get3Momentum();

  double a = mass2*mass2;
  double b = mass3*mass3;
  double c = utilities::GetNorm(p1);
  double d = Q + mass1 + mass2 + mass3 - momentum1(0);
  double e = inner_prod(p1, dir2)/c;

  double first = 1./2./(c*c*e*e-d*d);
  double second = a*a*d*d-2*a*b*d*d+4.*a*c*c*d*d*e*e-2.*a*c*c*d*d-2.*a*d*d*d*d+b*b*d*d+2*b*c*c*d*d-2.*b*d*d*d*d+c*c*c*c*d*d-2.*c*c*d*d*d*d+d*d*d*d*d*d;
  double third = a*c*e-b*c*e-c*c*c*e+c*d*d*e;

  p2Norm = first*(-std::sqrt(second)+third);
  p2 = p2Norm*dir2;

  ublas::vector<double> p3 = -(p1+p2);
  double p3Norm = utilities::GetNorm(p3);

  momentum2(0) = std::sqrt(a+p2Norm*p2Norm);
  momentum2(1) = p2(0);
  momentum2(2) = p2(1);
  momentum2(3) = p2(2);

  momentum3(0) = std::sqrt(b+p3Norm*p3Norm);
  momentum3(1) = p3(0);
  momentum3(2) = p3(1);
  momentum3(3) = p3(2);

  //std::cout << "\t" << inner_prod(p1, p2)/p2Norm/c << std::endl;

  // Perform Lorentz boost back to lab frame
  finalState1->SetFourMomentum(utilities::LorentzBoost(velocity, momentum1));
  finalState2->SetFourMomentum(utilities::LorentzBoost(velocity, momentum2));
  finalState3->SetFourMomentum(utilities::LorentzBoost(velocity, momentum3));

}

void ReactionMode::TwoBodyDecay(ublas::vector<double>& velocity, PDS::core::DynamicParticle* finalState1, PDS::core::DynamicParticle* finalState2, double Q) {
  ublas::vector<double> momentum1 (4);
  ublas::vector<double> momentum2 (4);

  ublas::vector<double> dir = utilities::RandomDirection();

  double mass1 = finalState1->GetRestMass();
  double mass2 = finalState2->GetRestMass();

  double M = Q + mass1 + mass2;

  double p = 1./(2.*M)*std::sqrt((M*M-std::pow(mass1-mass2, 2.))*(M*M-std::pow(mass1+mass2, 2.)));

  double energy1 = std::sqrt(mass1*mass1+p*p);
  double energy2 = std::sqrt(mass2*mass2+p*p);

  momentum1(0) = energy1;
  momentum1(1) = -p*dir[0];
  momentum1(2) = -p*dir[1];
  momentum1(3) = -p*dir[2];

  momentum2(0) = energy2;
  momentum2(1) = p*dir[0];
  momentum2(2) = p*dir[1];
  momentum2(3) = p*dir[2];

  // Perform Lorentz boost back to lab frame
  finalState1->SetFourMomentum(utilities::LorentzBoost(velocity, momentum1));
  finalState2->SetFourMomentum(utilities::LorentzBoost(velocity, momentum2));
}

static std::vector<PDS::core::DynamicParticle*> BetaMinus::Decay(PDS::core::DynamicParticle* initState, double Q, double daughterExEn) {
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

static std::vector<PDS::core::DynamicParticle*> BetaPlus::Decay(PDS::core::DynamicParticle* initState, double Q, double daughterExEn) {
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

static std::vector<PDS::core::DynamicParticle*> ConversionElectron::Decay(PDS::core::DynamicParticle* initState, double Q, double daughterExEn) {
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

static std::vector<PDS::core::DynamicParticle*> Proton::Decay(PDS::core::DynamicParticle* initState, double Q, double daughterExEn) {
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

static std::vector<PDS::core::DynamicParticle*> Alpha::Decay(PDS::core::DynamicParticle* initState, double Q, double daughterExEn) {
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

static std::vector<PDS::core::DynamicParticle*> Gamma::Decay(PDS::core::DynamicParticle* initState, double Q, double daughterExEn) {
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

void ReactionMode::SetSpectrumGenerator(SpectrumGenerator* sg) {
  spectrumGen = sg;
}

BetaMinus::BetaMinus() { }

BetaPlus::BetaPlus() { }

ConversionElectron::ConversionElectron() { }

Proton::Proton () { }

Alpha::Alpha () { }

Gamma::Gamma () { }
