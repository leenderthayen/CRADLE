#include "catch.hpp"

#include "CRADLE/SpectrumGenerator.h"
#include "PDS/Factory/ParticleFactory.h"
#include "PDS/Factory/NuclearDataLoaders.h"

#include "BSG/Generator.h"

#include <string>
#include <iostream>
#include <vector>

TEST_CASE("Test External BSG Initialization") {
  CRADLE::ExternalBSG external("");
}

TEST_CASE("Test BSG Spectral generation") {
  CRADLE::ExternalBSG external("");

  std::string homeDir = "/Users/leenderthayen";
  std::string Geant4RadDir = homeDir + "/Nuclear_Databases/Geant4/RadioactiveDecay5.4";
  std::string Geant4PhotonDir = homeDir + "/Nuclear_Databases/Geant4/PhotonEvaporation5.5";

  PDS::ParticleFactory::SetGeant4RadDirectory(Geant4RadDir);
  PDS::ParticleFactory::SetGeant4PhotonDirectory(Geant4PhotonDir);

  double Q = 256.0 * keV;

  PDS::core::Particle p1 = PDS::ParticleFactory::CreateNewParticleFromGeant4(20, 45, 0.);
  PDS::core::Particle p2 = PDS::ParticleFactory::CreateNewParticleFromGeant4(21, 45, 0.);

  REQUIRE(p1.GetName() == "45Ca");

  const std::vector<std::vector<double> >* spectrum = external.GetSpectrum(p1, p2, Q);

  const BSG::Generator* generator = external.GetGenerator();

  BSG::BetaParams bp = generator->GetBetaParams();

  REQUIRE(bp.Zi == 20);
  REQUIRE(bp.A == 45);
  REQUIRE(bp.Zf == 21);
  REQUIRE(bp.betaType == NHL::BETA_MINUS);
  REQUIRE(bp.decayType == NHL::BetaDecayType::GAMOWTELLER);
  REQUIRE(bp.R == Approx(0.014).epsilon(0.2));
  REQUIRE(bp.mixingRatio == 0.);
  REQUIRE(bp.aNeg[0] != 0.);
  REQUIRE(bp.W0 == Approx(1.5).epsilon(0.01));

  REQUIRE((*spectrum)[0][0] != 0);
  REQUIRE((*spectrum)[0][1] != 0);
  REQUIRE((*spectrum)[1][0] != 0);
}

TEST_CASE("Multiple initializations") {
  CRADLE::ExternalBSG external("");

  std::string homeDir = "/Users/leenderthayen";
  std::string Geant4RadDir = homeDir + "/Nuclear_Databases/Geant4/RadioactiveDecay5.4";
  std::string Geant4PhotonDir = homeDir + "/Nuclear_Databases/Geant4/PhotonEvaporation5.5";

  PDS::ParticleFactory::SetGeant4RadDirectory(Geant4RadDir);
  PDS::ParticleFactory::SetGeant4PhotonDirectory(Geant4PhotonDir);

  double Q = 256.0 * keV;

  PDS::core::Particle p1 = PDS::ParticleFactory::CreateNewParticleFromGeant4(20, 45, 0.);
  PDS::core::Particle p2 = PDS::ParticleFactory::CreateNewParticleFromGeant4(21, 45, 0.);

  const std::vector<std::vector<double> >* spectrum = external.GetSpectrum(p1, p2, Q);

  REQUIRE((*spectrum)[0][0] != 0);
  REQUIRE((*spectrum)[0][1] != 0);
  REQUIRE((*spectrum)[1][0] != 0);

  double Q2 = 3502.0 * keV;

  PDS::core::Particle p3 = PDS::ParticleFactory::CreateNewParticleFromGeant4(2, 6, 0.);
  PDS::core::Particle p4 = PDS::ParticleFactory::CreateNewParticleFromGeant4(3, 6, 0.);

  REQUIRE(p3.GetName() == "6He");

  const std::vector<std::vector<double> >* spectrum2 = external.GetSpectrum(p3, p4, Q2);

  REQUIRE((*spectrum2)[0][0] != 0);
  REQUIRE((*spectrum2)[0][1] != 0);
  REQUIRE((*spectrum2)[1][0] != 0);

  REQUIRE((*spectrum2)[1][1] != (*spectrum)[1][1]);
}
