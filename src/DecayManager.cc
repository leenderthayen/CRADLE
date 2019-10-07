#include "CRADLE/DecayManager.h"
#include "CRADLE/Utilities.h"
#include "CRADLE/SpectrumGenerator.h"
#include "CRADLE/ConfigParser.h"
#include "PDS/Factory/ParticleFactory.h"
#include "CRADLE/ReactionEngine.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <sys/types.h>
#include <sys/stat.h>
#include <future>

using namespace std;

DecayManager::~DecayManager() { }

void DecayManager::Initialise(ConfigOptions _configOptions) {
  cout << "Initialising..." << endl;
  configOptions = &_configOptions;
  initStateName = configOptions->nuclearOptions.Name;
  initExcitationEn = configOptions->nuclearOptions.Energy;
  filename = configOptions->general.Output;

  PDS::ParticleFactory::RegisterBasicParticles();
//  PDS::ParticleFactory::GenerateNucleus(initStateName, configOptions.nuclearOptions.Charge, configOptions.nuclearOptions.Nucleons);
}

void DecayManager::SetReactionEngine(ReactionEngine* _reactionEngine){
  reactionEngine = _reactionEngine;
}

bool DecayManager::MainLoop(int nrParticles, int nThreads) {
  cout << "Starting Main Loop " << endl;
  ofstream fileStream;
  fileStream.open(filename.c_str());

  fileStream << reactionEngine->GenerateEvent(0,initStateName,initExcitationEn,*configOptions);
  int Q = (nrParticles-1)/nThreads;
	int R = (nrParticles-1)%nThreads;

  std::future<std::string> f[nThreads];
  for (int t = 0; t < nThreads; t++) {
    if(t<R) f[t] = std::async(std::launch::async, &DecayManager::GenerateEvents, this, Q+1);
    else f[t] = std::async(std::launch::async, &DecayManager::GenerateEvents, this, Q);

  }

  for (int t = 0; t < nThreads; t++) {
    fileStream << f[t].get();

  }

  std::cout << "Done! Time taken: ";
  fileStream.flush();
  fileStream.close();
  return true;
}

std::string DecayManager::GenerateEvents(int nrEvents){
  std::ios::sync_with_stdio(false);
  // boost::progress_display show_progress(nrEvents);
  // boost::progress_timer t;

  std::ostringstream threadDataSS;
  for(int i=0; i < nrEvents ; i++){
    threadDataSS << reactionEngine->GenerateEvent(i, initStateName, initExcitationEn, *configOptions);
    // ++show_progress;
  }
  return threadDataSS.str();
}
