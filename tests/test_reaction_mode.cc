#include "catch.hpp"

#include "CRADLE/ReactionMode.h"
#include "CRADLE/SpectrumGenerator.h"
#include "CRADLE/DecayStructures.h"

#include "PDS/Core/DynamicParticle.h"
#include "PDS/Factory/ParticleFactory.h"
#include "PDS/Units/GlobalSystemOfUnits.h"

#include <vector>
#include <map>
#include <string>
#include <memory>

std::vector<PDS::core::DynamicParticle> TestActivator(const PDS::core::DynamicParticle& i,
  double Q, double E, std::map<std::string, std::shared_ptr<CRADLE::SpectrumGenerator> > sg) {
  std::vector<PDS::core::DynamicParticle> v;

  return v;
}

TEST_CASE("Initialisation") {
  SECTION("Default constructor") {
    CRADLE::ReactionMode mode;
  }

  SECTION("Process constructor") {
    std::map<std::string, std::shared_ptr<CRADLE::SpectrumGenerator> > m;
    CRADLE::Process p = {1, nullptr, &TestActivator, m};
    CRADLE::ReactionMode mode(p);
  }
}

TEST_CASE("Activation") {
  PDS::ParticleFactory::RegisterBasicParticles();

  std::map<std::string, std::shared_ptr<CRADLE::SpectrumGenerator> > m;
  CRADLE::Process p = {1, nullptr, &TestActivator, m};
  CRADLE::ReactionMode mode(p);

  PDS::core::DynamicParticle e = PDS::ParticleFactory::CreateNewDynamicParticle("electron", 0.);
  double Q = 0;
  double E = 0;

  std::vector<PDS::core::DynamicParticle> v = mode.Activate(e, Q, E);

  REQUIRE(v.size() == 0);
}

TEST_CASE("Beta decay") {
  PDS::ParticleFactory::RegisterBasicParticles();

  std::shared_ptr<CRADLE::SpectrumGenerator> sbd = std::make_shared<CRADLE::BasicBetaSpectrumGenerator>(false, 1.0 * keV);
  std::map<std::string, std::shared_ptr<CRADLE::SpectrumGenerator> > m = {{"electron_energy", sbd}};

  SECTION("Mock up activator") {
    PDS::core::DynamicParticle e = PDS::ParticleFactory::CreateNewDynamicParticle("electron", 0.);
    double Q = 0;
    double E = 0;

    CRADLE::Process p = {1, nullptr, &TestActivator, m};
    CRADLE::ReactionMode mode(p);

    mode.Activate(e, Q, E);
  }

  SECTION("Simple beta decay activator") {
    std::string homeDir = "/Users/leenderthayen/Work";
    std::string Geant4RadDir = homeDir + "/Nuclear_Databases/Geant4/RadioactiveDecay5.4";
    std::string Geant4PhotonDir = homeDir + "/Nuclear_Databases/Geant4/PhotonEvaporation5.5";

    PDS::ParticleFactory::SetGeant4RadDirectory(Geant4RadDir);
    PDS::ParticleFactory::SetGeant4PhotonDirectory(Geant4PhotonDir);

    double Q = 3502.0 * keV;
    double E = 0;

    PDS::core::Particle part = PDS::ParticleFactory::CreateNewParticleFromGeant4(2, 6, 0.);

    CRADLE::Process p = {1, nullptr, &CRADLE::decay::SimpleBetaMinus, m};
    CRADLE::ReactionMode mode(p);

    PDS::core::DynamicParticle dynPart(part);

    std::vector<PDS::core::DynamicParticle> v = mode.Activate(dynPart, Q, E);

    REQUIRE(v.size() == 3);

    std::vector<PDS::core::DynamicParticle> v2 = mode.Activate(dynPart, Q, E);

    REQUIRE(v2.size() == 3);
  }


}
