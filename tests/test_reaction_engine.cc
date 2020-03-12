#include "catch.hpp"

#include "CRADLE/ReactionEngine.h"
#include "CRADLE/Event.h"

#include "PDS/Factory/ParticleFactory.h"

TEST_CASE("Initialization") {
  CRADLE::ReactionEngine re;

  PDS::ParticleFactory::RegisterBasicParticles();

  std::string homeDir = "/Users/leenderthayen";
  std::string Geant4RadDir = homeDir + "/Nuclear_Databases/Geant4/RadioactiveDecay5.4";
  std::string Geant4PhotonDir = homeDir + "/Nuclear_Databases/Geant4/PhotonEvaporation5.5";

  PDS::ParticleFactory::SetGeant4RadDirectory(Geant4RadDir);
  PDS::ParticleFactory::SetGeant4PhotonDirectory(Geant4PhotonDir);

  double Q = 3502.0 * keV;
  double E = 0;

  PDS::core::Particle part = PDS::ParticleFactory::CreateNewParticleFromGeant4(2, 6, 0.);

  PDS::core::DynamicParticle dynPart(part);

  CRADLE::Event e = re.ProcessParticle(dynPart);

  std::vector<CRADLE::Vertex> vertices = e.GetVertices();

  std::vector<PDS::core::DynamicParticle> v = vertices[0].particles;

  REQUIRE(v.size() == 3);
}
