#include "catch.hpp"

#include "CRADLE/SpectrumGenerator.h"
#include "PDS/Factory/ParticleFactory.h"
#include "PDS/Factory/NuclearDataLoaders.h"

TEST_CASE("Initialization") {
  std::string homeDir = "/Users/leenderthayen";
  std::string Geant4RadDir = homeDir + "/Nuclear_Databases/Geant4/RadioactiveDecay5.4";
  std::string Geant4PhotonDir = homeDir + "/Nuclear_Databases/Geant4/PhotonEvaporation5.5";

  PDS::ParticleFactory::SetGeant4RadDirectory(Geant4RadDir);
  PDS::ParticleFactory::SetGeant4PhotonDirectory(Geant4PhotonDir);

  double Q = 3502.0 * keV;

  PDS::core::Particle p1 = PDS::ParticleFactory::CreateNewParticleFromGeant4(2, 6, 0.);
  PDS::core::Particle p2 = PDS::ParticleFactory::CreateNewParticleFromGeant4(3, 6, 0.);

  SECTION("DeltaSpectrumGenerator") {
    CRADLE::DeltaSpectrumGenerator dsg;

    const std::vector<std::vector<double> >* spectrum = dsg.GetSpectrum(p1, p2, Q);

    REQUIRE(spectrum->size() == 1);
    REQUIRE((*spectrum)[0][0] == Q);
  }

  SECTION("BasicBetaSpectrumGenerator") {
    CRADLE::BasicBetaSpectrumGenerator sbd;

    const std::vector<std::vector<double> >* spectrum = sbd.GetSpectrum(p1, p2, Q);

    REQUIRE((*spectrum)[0][0] != 0);
    REQUIRE((*spectrum)[0][1] != 0);
    REQUIRE((*spectrum)[1][0] != 0);
  }


}
