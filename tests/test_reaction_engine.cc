#include "catch.hpp"

#include "CRADLE/ReactionEngine.h"
#include "CRADLE/Event.h"

#include "PDS/Factory/ParticleFactory.h"

#include <memory>
#include <iostream>

TEST_CASE("Initialization") {
  CRADLE::ReactionEngine re;
}

TEST_CASE("Processing") {
  CRADLE::ReactionEngine re;

  PDS::ParticleFactory::RegisterBasicParticles();

  std::string homeDir = "/Users/leenderthayen/Work";
  std::string Geant4RadDir = homeDir + "/Nuclear_Databases/Geant4/RadioactiveDecay5.4";
  std::string Geant4PhotonDir = homeDir + "/Nuclear_Databases/Geant4/PhotonEvaporation5.5";

  PDS::ParticleFactory::SetGeant4RadDirectory(Geant4RadDir);
  PDS::ParticleFactory::SetGeant4PhotonDirectory(Geant4PhotonDir);

  std::shared_ptr<PDS::core::DynamicParticle> dynPart =
  std::make_shared<PDS::core::DynamicParticle>(PDS::ParticleFactory::GetNewDynamicParticleFromGeant4(2, 6, 0.));
  ublas::vector<double> initPos;

  REQUIRE(dynPart->GetParticle().GetCharge() == 2);

  std::shared_ptr<PDS::core::Vertex> vertex = re.ProcessParticle(dynPart, initPos);

  REQUIRE(vertex->GetParticlesOut().size() == 3);
  for (auto & p : vertex->GetParticlesOut()) {
    std::cout << p->GetName() << std::endl;
  }
}
