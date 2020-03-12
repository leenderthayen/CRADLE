#ifndef CRADLE_DECAY_STRUCTURES_H
#define CRADLE_DECAY_STRUCTURES_H

#include "CRADLE/SpectrumGenerator.h"
#include "CRADLE/Utilities.h"

#include "PDS/Core/DynamicParticle.h"
#include "PDS/Factory/ParticleFactory.h"
#include "PDS/Util/Atoms.h"
#include "PDS/Units/GlobalPhysicalConstants.h"

#include <boost/numeric/ublas/vector.hpp>
#include <vector>
#include <memory>

namespace ublas = boost::numeric::ublas;

namespace CRADLE {
  namespace decay {
    inline std::vector<PDS::core::DynamicParticle> SimpleBetaMinus(PDS::core::DynamicParticle& initState, double Q, double daughterExEn, std::map<std::string, std::shared_ptr<SpectrumGenerator> > sg) {

      PDS::core::Nucleus* initNucleusDef = static_cast<PDS::core::Nucleus*>(initState.GetParticle().GetParticleDefinition());
      //TODO: design general approach for the creation of a dynamic particle with charge Z+1 from dynamic particle with charge Z
      PDS::core::DynamicParticle recoil = PDS::ParticleFactory::GetNewDynamicParticleFromGeant4(initNucleusDef->GetZ()+1,initNucleusDef->GetA(),daughterExEn);
      PDS::core::DynamicParticle e = PDS::ParticleFactory::CreateNewDynamicParticle("electron", 0);
      PDS::core::DynamicParticle enubar = PDS::ParticleFactory::CreateNewDynamicParticle("electron_antineutrino", 0);

      double elEnergy = utilities::RandomFromDistribution(
        *(sg["electron_energy"]->GetSpectrum(initState.GetParticle(), recoil.GetParticle(), Q)))
        +electron_mass_c2;
      double elMomentum = std::sqrt(elEnergy*elEnergy-std::pow(electron_mass_c2, 2.));
      ublas::vector<double> enuDir = utilities::RandomDirection();
      ublas::vector<double> eDir = utilities::RandomDirection();

      //Work in the COM frame
      ublas::vector<double> elFourMomentum (4);

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

    inline std::vector<PDS::core::DynamicParticle> SimpleBetaPlus(PDS::core::DynamicParticle& initState, double Q, double daughterExEn, std::map<std::string, std::shared_ptr<SpectrumGenerator> > sg) {
      PDS::core::Nucleus* initNucleusDef = static_cast<PDS::core::Nucleus*>(initState.GetParticle().GetParticleDefinition());
      //TODO: design general approach for the creation of a dynamic particle with charge Z+1 from dynamic particle with charge Z
      PDS::core::DynamicParticle recoil = PDS::ParticleFactory::GetNewDynamicParticleFromGeant4(initNucleusDef->GetZ()-1,initNucleusDef->GetA(),daughterExEn);
      PDS::core::DynamicParticle pos = PDS::ParticleFactory::CreateNewDynamicParticle("e+",0);
      PDS::core::DynamicParticle enu = PDS::ParticleFactory::CreateNewDynamicParticle("enu",0);

      double posEnergy = utilities::RandomFromDistribution(
        *(sg["positron_energy"]->GetSpectrum(initState.GetParticle(), recoil.GetParticle(), Q)))
        +electron_mass_c2;
      double posMomentum = std::sqrt(posEnergy*posEnergy-std::pow(electron_mass_c2, 2.));
      ublas::vector<double> enuDir = utilities::RandomDirection();
      ublas::vector<double> posDir = utilities::RandomDirection();

      //Work in the COM frame
      ublas::vector<double> posFourMomentum (4);

      posFourMomentum(0) = posEnergy;
      posFourMomentum(1) = posMomentum*posDir[0];
      posFourMomentum(2) = posMomentum*posDir[1];
      posFourMomentum(3) = posMomentum*posDir[2];

      pos.SetFourMomentum(posFourMomentum);

      ublas::vector<double> velocity = -initState.GetVelocity();

      double E0 = Q - 2 * electron_mass_c2;
      utilities::ThreeBodyDecay(velocity, pos, enu, recoil, enuDir, E0);

      std::vector<PDS::core::DynamicParticle> finalStates;
      finalStates.push_back(recoil);
      finalStates.push_back(pos);
      finalStates.push_back(enu);

      return finalStates;
    }

    inline std::vector<PDS::core::DynamicParticle> ConversionElectron(PDS::core::DynamicParticle& initState, double Q, double daughterExEn, std::map<std::string, std::shared_ptr<SpectrumGenerator> > sg) {
      std::vector<PDS::core::DynamicParticle> finalStates;

      ublas::vector<double> velocity = -initState.GetVelocity();
      PDS::core::DynamicParticle recoil = PDS::ParticleFactory::CreateNewDynamicParticle(initState.GetParticle().GetName(),daughterExEn);
      PDS::core::DynamicParticle e = PDS::ParticleFactory::CreateNewDynamicParticle("e-",0);
      utilities::TwoBodyDecay(velocity, recoil, e, Q);

      finalStates.push_back(recoil);
      finalStates.push_back(e);

      return finalStates;
    }

    inline std::vector<PDS::core::DynamicParticle> ProtonSeparation(PDS::core::DynamicParticle& initState, double Q, double daughterExEn, std::map<std::string, std::shared_ptr<SpectrumGenerator> > sg) {
      PDS::core::Nucleus* initNucleusDef = static_cast<PDS::core::Nucleus*>(initState.GetParticle().GetParticleDefinition());
      //std::ostringstream oss;
      //oss << initState.GetParticle().GetParticleDefinition()->GetName();
      PDS::core::DynamicParticle recoil = PDS::ParticleFactory::GetNewDynamicParticleFromGeant4(initNucleusDef->GetZ()-1,initNucleusDef->GetA(),daughterExEn);
      PDS::core::DynamicParticle p = PDS::ParticleFactory::CreateNewDynamicParticle("p+",0);

      ublas::vector<double> velocity = -initState.GetVelocity();
      utilities::TwoBodyDecay(velocity, recoil, p, Q);

      std::vector<PDS::core::DynamicParticle> finalStates;
      finalStates.push_back(recoil);
      finalStates.push_back(p);

      return finalStates;
    }

    inline std::vector<PDS::core::DynamicParticle> Alpha(PDS::core::DynamicParticle& initState, double Q, double daughterExEn, std::map<std::string, std::shared_ptr<SpectrumGenerator> > sg) {
      PDS::core::Nucleus* initNucleusDef = static_cast<PDS::core::Nucleus*>(initState.GetParticle().GetParticleDefinition());
      //std::ostringstream oss;
      //oss << initState.GetParticle().GetParticleDefinition()->GetName();
      PDS::core::DynamicParticle recoil = PDS::ParticleFactory::GetNewDynamicParticleFromGeant4(initNucleusDef->GetZ()-2,initNucleusDef->GetA()-4,daughterExEn);
      PDS::core::DynamicParticle alpha = PDS::ParticleFactory::CreateNewDynamicParticle("alpha",0);

      ublas::vector<double> velocity = -initState.GetVelocity();
      utilities::TwoBodyDecay(velocity, recoil, alpha, Q);

      std::vector<PDS::core::DynamicParticle> finalStates;
      finalStates.push_back(recoil);
      finalStates.push_back(alpha);

      return finalStates;
    }

    inline std::vector<PDS::core::DynamicParticle> Gamma(PDS::core::DynamicParticle& initState, double Q, double daughterExEn, std::map<std::string, std::shared_ptr<SpectrumGenerator> > sg) {
      std::vector<PDS::core::DynamicParticle> finalStates;

      ublas::vector<double> velocity = -initState.GetVelocity();
      PDS::core::DynamicParticle recoil = PDS::ParticleFactory::CreateNewDynamicParticle(initState.GetParticle().GetParticleDefinition()->GetName(),daughterExEn);
      PDS::core::DynamicParticle gamma = PDS::ParticleFactory::CreateNewDynamicParticle("gamma",0);

      utilities::TwoBodyDecay(velocity, recoil, gamma, Q);

      finalStates.push_back(recoil);
      finalStates.push_back(gamma);

      return finalStates;
    }
  }//end of decay namespace
}//end of CRADLE namespace

#endif
