#include "DecayManager.hh"
#include "Utilities.hh"
#include "DecayChannel.hh"
#include "Particle.hh"
#include "DecayMode.hh"
#include "SpectrumGenerator.hh"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <sys/types.h>
#include <sys/stat.h>
#include <future>

using namespace std;

DecayManager::~DecayManager() { }

void DecayManager::Initialise(ConfigOptions configOptions) {
  cout << "Initialising..." << endl;
  initStateName = configOptions.nuclearOptions.Name;
  initExcitationEn = configOptions.nuclearOptions.Energy;
  filename = configOptions.General.Output;

  PDS::ParticleFactory::RegisterBasicParticles();
  PDS::factory::GenerateNucleus(initStateName, configOptions.nuclearOptions.Z, configOptions.nuclearOptions.A);
}

void DecayManager::SetReactionEngine(ReactionEngine* _reactionEngine){
  reactionEngine = _reactionEngine;
}

bool DecayManager::MainLoop(int nrParticles, int nThreads) {
  cout << "Starting Main Loop " << endl;
  ofstream fileStream;
  fileStream.open(filename.c_str());

  std::ios::sync_with_stdio(false);
  boost::progress_display show_progress(nrParticles);
  boost::progress_timer t;
  fileStream << GenerateEvent(0);
  int Q = nrParticles/nThreads;
	int R = nrParticles%nThreads;

  std::future<std::string> f[nThreads];
  for (int t = 0; t < nThreads; t++) {
    if(t<R) f[t] = std::async(std::launch::async, &DecayManager::GenerateEvent, this, Q+1);
    else f[t] = std::async(std::launch::async, &DecayManager::GenerateEvent, this, Q);

  }

  for (int t = 0; t < nThreads; t++) {
    fileStream << f[t].get();
    ++show_progress;
  }

  std::cout << "Done! Time taken: ";
  fileStream.flush();
  fileStream.close();
  return true;
}
