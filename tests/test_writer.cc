#include "catch.hpp"

#include "PDS/Factory/ParticleFactory.h"

#include "CRADLE/Cradle.h"
#include "CRADLE/Writer.h"

TEST_CASE("Initialization") {
  CRADLE::Cradle cradle("test_output");

  std::string homeDir = "/Users/leenderthayen/Work";
  std::string Geant4RadDir = homeDir + "/Nuclear_Databases/Geant4/RadioactiveDecay5.4";
  std::string Geant4PhotonDir = homeDir + "/Nuclear_Databases/Geant4/PhotonEvaporation5.5";

  PDS::ParticleFactory::SetGeant4RadDirectory(Geant4RadDir);
  PDS::ParticleFactory::SetGeant4PhotonDirectory(Geant4PhotonDir);

  cradle.EventLoop(30);

  std::string fullPath = "/Users/leenderthayen/git/CRADLE++/build_feature/writer_output.txt";
  CRADLE::WriteToFile(fullPath, cradle.GetEvents());
}
