#include "catch.hpp"

#include "CRADLE/ReactionMode.h"
#include "CRADLE/DecayStructures.h"

#include "PDS/Factory/ParticleFactory.h"

#include <memory>

TEST_CASE("Initialisation") {
  CRADLE::ReactionMode mode = CRADLE::ReactionModeFactory::DefaultBetaMinus();

  SECTION("Cross check") {
    CRADLE::Process p = mode.GetProcesses()[0];

    REQUIRE(p.staticBranchingRatio == 1);
    REQUIRE(p.bc == nullptr);
    REQUIRE(p.a == &CRADLE::decay::SimpleBetaMinus);

    std::map<std::string, std::shared_ptr<CRADLE::SpectrumGenerator> > m = p.generators;

    REQUIRE(m["electron_energy"] != nullptr);
  }

  SECTION("Activation") {
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

    std::vector<PDS::core::DynamicParticle> v = mode.Activate(dynPart, Q, E);

    REQUIRE(v.size() == 3);
  }
}
