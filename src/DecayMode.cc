#include "CRADLE/DecayMode.hh"
#include "CRADLE/DecayManager.hh"
#include "CRADLE/Particle.hh"
#include "CRADLE/Utilities.hh"
#include "CRADLE/SpectrumGenerator.hh"

#include <string>
#include <sstream>

namespace CRADLE {

void DecayMode::ThreeBodyDecay(ublas::vector<double>& velocity, Particle* finalState1, Particle* finalState2, Particle* finalState3, ublas::vector<double>& dir2, double Q) {
  //Perform decay in CoM frame
  ublas::vector<double> momentum1 = finalState1->GetMomentum();
  ublas::vector<double> momentum2 (4);
  ublas::vector<double> momentum3 (4);

  ublas::vector<double> p2 (3);
  double p2Norm = 0.;

  double mass1 = finalState1->GetMass();
  double mass2 = finalState2->GetMass();
  double mass3 = finalState3->GetMass();
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
  finalState1->SetMomentum(utilities::LorentzBoost(velocity, momentum1));
  finalState2->SetMomentum(utilities::LorentzBoost(velocity, momentum2));
  finalState3->SetMomentum(utilities::LorentzBoost(velocity, momentum3));

}

void DecayMode::TwoBodyDecay(ublas::vector<double>& velocity, Particle* finalState1, Particle* finalState2, double Q) {
  ublas::vector<double> momentum1 (4);
  ublas::vector<double> momentum2 (4);

  ublas::vector<double> dir = utilities::RandomDirection();

  double mass1 = finalState1->GetMass();
  double mass2 = finalState2->GetMass();

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
  finalState1->SetMomentum(utilities::LorentzBoost(velocity, momentum1));
  finalState2->SetMomentum(utilities::LorentzBoost(velocity, momentum2));
}

std::vector<Particle*> BetaMinus::Decay(Particle* initState, double Q, double daughterExEn) {
  std::vector<Particle*> finalStates;

  // std::cout << "In BetaMinus Decay " << std::endl;
  // std::cout << "Address: " << initState << std::endl;
  std::ostringstream oss;
  oss << initState->GetCharge()+initState->GetNeutrons() << utilities::atoms[initState->GetCharge()];
  //std::cout << oss.str() << std::endl;
  Particle* recoil = DecayManager::GetInstance().GetNewParticle(oss.str(), initState->GetCharge()+1, initState->GetCharge()+initState->GetNeutrons());
  recoil->SetExcitationEnergy(daughterExEn);
  Particle* e = DecayManager::GetInstance().GetNewParticle("e-");
  Particle* enu = DecayManager::GetInstance().GetNewParticle("enubar");

  // std::cout << "Recoil " << recoil->GetCharge() << " " << recoil->GetNeutrons() << " " << recoil << std::endl;

  oss.str("");
  oss.clear();
  oss << "BetaMinus:Z" << recoil->GetCharge() << "A" << recoil->GetCharge() + recoil->GetNeutrons() << "Q" << Q;
  //Work in the COM frame
  ublas::vector<double> elFourMomentum (4);

  DecayManager& dm = DecayManager::GetInstance();
  double CS = dm.configOptions.couplingConstants.CS;
  double CSP = dm.configOptions.couplingConstants.CSP;
  double CV = dm.configOptions.couplingConstants.CV;
  double CVP = dm.configOptions.couplingConstants.CVP;
  double CA = dm.configOptions.couplingConstants.CA;
  double CAP = dm.configOptions.couplingConstants.CAP;
  double CT = dm.configOptions.couplingConstants.CT;
  double CTP = dm.configOptions.couplingConstants.CTP;

  double mf = 0.;
  double mgt = 0.;

  if (dm.configOptions.betaDecay.Default == "Fermi") {
    mf = 1.;
  }
  else {
    mgt = 1.;
  }

  double a = utilities::CalculateBetaNeutrinoAsymmetry(CS, CSP, CT, CTP, CV, CVP, CA, CAP, mf, mgt);
  double fierz = utilities::CalculateFierz(CS, CSP, CT, CTP, CV, CVP, CA, CAP, mf, mgt);

  //std::cout << "fierz " << fierz << " a " << a << std::endl;

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
    double gamma = std::sqrt(1-std::pow(utilities::FINESTRUCTURE*recoil->GetCharge(), 2.));
    dist = spectrumGen->GenerateSpectrum(initState, recoil, Q);
    int i = 0;
    for (auto & element : *dist) {
      double E = element[0]+utilities::EMASSC2;
      double SH = element[0];
      ((*dist)[i])[1] = SH*(1+gamma*fierz*utilities::EMASSC2/E);
    }
    DecayManager::GetInstance().RegisterDistribution(oss.str(), dist);
  }

  // std::cout << "Found distribution" << std::endl;

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

std::vector<Particle*> BetaPlus::Decay(Particle* initState, double Q, double daughterExEn) {
  std::vector<Particle*> finalStates;

  DecayManager& dm = DecayManager::GetInstance();

  double E0 = Q-2*utilities::EMASSC2;

  //std::cout << "In BetaPlus Decay " << std::endl;
  //std::cout << "Address: " << initState << std::endl;
  std::ostringstream oss;
  oss << initState->GetCharge()+initState->GetNeutrons() << utilities::atoms[initState->GetCharge()-2];
  Particle* recoil = DecayManager::GetInstance().GetNewParticle(oss.str(), initState->GetCharge()-1, initState->GetCharge()+initState->GetNeutrons());
  recoil->SetExcitationEnergy(daughterExEn);
  Particle* pos = DecayManager::GetInstance().GetNewParticle("e+");
  Particle* enubar = DecayManager::GetInstance().GetNewParticle("enu");

  oss.str("");
  oss.clear();

  oss << "BetaPlus:Z" << recoil->GetCharge() << "A" << recoil->GetCharge() + recoil->GetNeutrons() << "Q" << Q;
  //Work in the COM frame
  ublas::vector<double> enubarDir = utilities::RandomDirection();

  ublas::vector<double> posFourMomentum (4);

  double mf = 0.;
  double mgt = 0.;

  /////////ajout de SL 12/05/2023//////////////

  if (dm.configOptions.betaDecay.Default == "Fermi") {
    mf = 1.;
  }
  else if (dm.configOptions.betaDecay.Default == "Gamow-Teller") {
    mgt = 1.;
  }
  else if (dm.configOptions.betaDecay.Default == "Auto") {
    std::string Type;
    try{
      Type = DecayManager::GetInstance().GetBetaType(oss.str());
    } catch (const std::invalid_argument& e) {
      Type = utilities::FindBetaType(initState, recoil);
      DecayManager::GetInstance().RegisterBetaType(oss.str(), Type);
    }
    if (Type == "Fermi") mf = 1.;
    else mgt = 1.;
  }
  ////////////////////////////////////////////////

  double CS = dm.configOptions.couplingConstants.CS;
  double CSP = dm.configOptions.couplingConstants.CSP;
  double CV = dm.configOptions.couplingConstants.CV;
  double CVP = dm.configOptions.couplingConstants.CVP;
  double CA = dm.configOptions.couplingConstants.CA;
  double CAP = dm.configOptions.couplingConstants.CAP;
  double CT = dm.configOptions.couplingConstants.CT;
  double CTP = dm.configOptions.couplingConstants.CTP;


  double a = utilities::CalculateBetaNeutrinoAsymmetry(CS, CSP, CT, CTP, CV, CVP, CA, CAP, mf, mgt);
  double fierz = utilities::CalculateFierz(CS, CSP, CT, CTP, CV, CVP, CA, CAP, mf, mgt);

  std::vector<std::vector<double> >* dist;
  try {
    dist = DecayManager::GetInstance().GetDistribution(oss.str());
  } catch (const std::invalid_argument& e) {
    bool advancedFermi = false;
    if (dm.configOptions.betaDecay.FermiFunction == "Advanced") { //// réécriture de la condition par SL 10/05/2023
      advancedFermi = true;
    }
    dist = spectrumGen->GenerateSpectrum(initState, recoil, E0); //// changement de Q en E0 par SL 10/05/2023
    double gamma = std::sqrt(1-std::pow(utilities::FINESTRUCTURE*recoil->GetCharge(), 2.));
    int i=0;
    for ( int i = 0; i<dist->size(); i++) {                                  ////// changement de boucle et element par ((*dist)[i]) par SL 10/05/2023
      double E = ((*dist)[i])[0]+utilities::EMASSC2;
      double SH = ((*dist)[i])[1];
      ((*dist)[i])[1] = SH*(1+gamma*fierz*utilities::EMASSC2/E);
    }
    DecayManager::GetInstance().RegisterDistribution(oss.str(), dist);
    }


  // ublas::vector<std::vector<double> >* dist;
  double posEnergy = utilities::RandomFromDistribution(*dist) + utilities::EMASSC2;
  double posMomentum = std::sqrt(posEnergy*posEnergy-std::pow(utilities::EMASSC2, 2.));

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

std::vector<Particle*> ConversionElectron::Decay(Particle* initState, double Q, double daughterExEn) {
  std::vector<Particle*> finalStates;

  ublas::vector<double> velocity = -initState->GetVelocity();
  Particle* recoil = DecayManager::GetInstance().GetNewParticle(initState->GetRawName());
  Particle* e = DecayManager::GetInstance().GetNewParticle("e+");
  recoil->SetExcitationEnergy(daughterExEn);

  TwoBodyDecay(velocity, recoil, e, Q);

  finalStates.push_back(recoil);
  finalStates.push_back(e);

  return finalStates;
}

std::vector<Particle*> Proton::Decay(Particle* initState, double Q, double daughterExEn) {
  std::vector<Particle*> finalStates;

  std::ostringstream oss;
  oss << initState->GetCharge()+initState->GetNeutrons() - 1 << utilities::atoms[initState->GetCharge()-2];
  Particle* recoil = DecayManager::GetInstance().GetNewParticle(oss.str(), initState->GetCharge()-1, initState->GetCharge()+initState->GetNeutrons()-1);
  Particle* p = DecayManager::GetInstance().GetNewParticle("p");
  recoil->SetExcitationEnergy(daughterExEn);

  ublas::vector<double> velocity = -initState->GetVelocity();
  TwoBodyDecay(velocity, recoil, p, Q);

  finalStates.push_back(recoil);
  finalStates.push_back(p);

  return finalStates;
}

std::vector<Particle*> Alpha::Decay(Particle* initState, double Q, double daughterExEn) {
  std::vector<Particle*> finalStates;

  std::ostringstream oss;
  oss << initState->GetCharge()+initState->GetNeutrons() - 4 << utilities::atoms[initState->GetCharge()-3];
  Particle* recoil = DecayManager::GetInstance().GetNewParticle(oss.str(), initState->GetCharge()-2, initState->GetCharge()+initState->GetNeutrons()-4);
  Particle* alpha = DecayManager::GetInstance().GetNewParticle("alpha");
  recoil->SetExcitationEnergy(daughterExEn);

  ublas::vector<double> velocity = -initState->GetVelocity();
  TwoBodyDecay(velocity, recoil, alpha, Q);

  finalStates.push_back(recoil);
  finalStates.push_back(alpha);

  return finalStates;
}

std::vector<Particle*> Gamma::Decay(Particle* initState, double Q, double daughterExEn) {
  std::vector<Particle*> finalStates;

  ublas::vector<double> velocity = -initState->GetVelocity();
  Particle* recoil = DecayManager::GetInstance().GetNewParticle(initState->GetRawName());
  Particle* gamma = DecayManager::GetInstance().GetNewParticle("gamma");
  recoil->SetExcitationEnergy(daughterExEn);

  TwoBodyDecay(velocity, recoil, gamma, Q);

  finalStates.push_back(recoil);
  finalStates.push_back(gamma);

  return finalStates;
}

DecayMode::DecayMode() { }

DecayMode::~DecayMode() { }

void DecayMode::SetSpectrumGenerator(SpectrumGenerator* sg) {
  spectrumGen = sg;
}

BetaMinus::BetaMinus() { }

BetaPlus::BetaPlus() { }

ConversionElectron::ConversionElectron() { }

Proton::Proton () { }

Alpha::Alpha () { }

Gamma::Gamma () { }

}//End of CRADLE namespace
