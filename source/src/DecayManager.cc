#include "DecayManager.hh"
#include "Utilities.hh"
#include "DecayChannel.hh"
#include "Particle.hh"
#include "DecayMode.hh"
#include "OptionContainer.hh"
#include "SpectrumGenerator.hh"
#include <boost/progress.hpp>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <sys/types.h>
#include <sys/stat.h>
#include <future>

using namespace std;

DecayManager::~DecayManager() { }


void DecayManager::RegisterBasicParticles() {
  PDS::ParticleFactory::RegisterBasicParticles();
}


bool DecayManager::Initialise(string name, int Z, int A,
                              double excitationEnergy, string _filename, int threads) {
  cout << "Initialising..." << endl;
  filename = _filename;
  initStateName = name;
  initExcitationEn = excitationEnergy;
  NRTHREADS = threads;

  return PDS::factory::GenerateNucleus(name, Z, A);
  PDS::core::DynamicParticle
}

bool DecayManager::InitializeOptionsFromConfigFile(std::string filename){
  configOptions = ParseConfigFile(filename);
}

void DecayManager::SetReactionEngine(){
  reactionEngine = new ReactionEngine();
}

bool DecayManager::MainLoop(int nrParticles) {
  cout << "Starting Main Loop " << endl;
  ofstream fileStream;
  fileStream.open(filename.c_str());

  std::ios::sync_with_stdio(false);
  boost::progress_display show_progress(nrParticles);
  boost::progress_timer t;
  fileStream << GenerateEvent(0);
  for (int i = 1; i < nrParticles; i+=NRTHREADS) {
    // cout << "LOOP NR " << i+1 << endl;
    int threads = std::min(NRTHREADS, nrParticles-i);
    std::future<std::string> f[threads];
    for (int t = 0; t < threads; t++) {
      f[t] = std::async(std::launch::async, &DecayManager::GenerateEvent, this, i+t);
    }

    for (int t = 0; t < threads; t++) {
      fileStream << f[t].get();
      ++show_progress;
    }
  }
  std::cout << "Done! Time taken: ";
  fileStream.flush();
  fileStream.close();
  return true;
}
