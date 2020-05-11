#include "catch.hpp"

#include "CRADLE/Cradle.h"
#include "CRADLE/Event.h"

#include "PDS/Core/Vertex.h"
#include "PDS/Factory/ParticleFactory.h"
#include "PDS/Units/GlobalSystemOfUnits.h"

#include <memory>
#include <iostream>

TEST_CASE("Initialization") {
  CRADLE::Cradle cradle("test_output");

  std::string homeDir = "/Users/leenderthayen/Work";
  std::string Geant4RadDir = homeDir + "/Nuclear_Databases/Geant4/RadioactiveDecay5.4";
  std::string Geant4PhotonDir = homeDir + "/Nuclear_Databases/Geant4/PhotonEvaporation5.5";

  PDS::ParticleFactory::SetGeant4RadDirectory(Geant4RadDir);
  PDS::ParticleFactory::SetGeant4PhotonDirectory(Geant4PhotonDir);

  SECTION("Construction of initial particle and vertex") {
    cradle.ConstructInitialParticle();
    cradle.ConstructInitialVertex();
  }

  SECTION("Generate event") {
    cradle.EventLoop(30);

    for (auto & e : cradle.GetEvents()) {
      for (auto & v : e.GetVertices()) {
        std::cout << "ID " << e.GetID() << "." << v->GetID() << " POS " << v->GetPosition()(0) / s / c_light << " " << v->GetPosition()(1) / mm << " " << v->GetPosition()(2) / mm << " " << v->GetPosition()(3) / mm << std::endl;
        for(auto & p : v->GetParticlesIn()) {
          std::cout << "ID " << e.GetID() << "." << v->GetID() << " IN " << p->GetName() << "(" << p->GetParticle().GetExcitationEnergy() / keV << ") E " << p->GetKinEnergy() / keV << std::endl;
        }
        for (auto & p : v->GetParticlesOut()) {
          std::cout << "ID " << e.GetID() << "." << v->GetID() << " OUT " << p->GetName() << "(" << p->GetParticle().GetExcitationEnergy() / keV << ") E " << p->GetKinEnergy() / keV << std::endl;
        }
      }
    }
  }

  SECTION("Print event") {
    cradle.EventLoop(2);

    for (auto & e : cradle.GetEvents()) {
      std::cout << e << std::endl;
    }
  }
}
