#include "DecayManager.hh"
#include "Utilities.hh"
#include "DecayChannel.hh"
#include "Particle.hh"
#include "DecayMode.hh"
#include "OptionContainer.hh"
#include <boost/progress.hpp>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <sys/types.h>
#include <sys/stat.h>
#include <future>

using namespace std;


template<typename A, typename B>
std::pair<B,A> flip_pair(const std::pair<A,B> &p)
{
    return std::pair<B,A>(p.second, p.first);
}

template<typename A, typename B>
std::multimap<B,A> flip_map(const std::map<A,B> &src)
{
    std::multimap<B,A> dst;
    std::transform(src.begin(), src.end(), std::inserter(dst, dst.begin()), 
                   flip_pair<A,B>);
    return dst;
}

DecayManager::~DecayManager() {
  // cout << "Destroying decaymanager" << endl;
  /*for (vector<Particle*>::iterator it = particleStack.begin();
       it != particleStack.end(); ++it) {
    delete *it;
  }*/
  for (map<string, Particle*>::iterator it = registeredParticles.begin();
       it != registeredParticles.end(); ++it) {
    for (vector<DecayChannel*>::iterator it2 =
             (it->second)->GetDecayChannels().begin();
         it2 != (it->second)->GetDecayChannels().end(); ++it2) {
      delete *it2;
    }
    delete it->second;
  }
  registeredParticles.clear();

  for (map<string, vector<vector<double> >*>::iterator it =
           registeredDistributions.begin();
       it != registeredDistributions.end(); ++it) {
    delete it->second;
  }
  registeredDistributions.clear();
}

void DecayManager::RegisterDecayMode(const string name, DecayMode& dm) {
  registeredDecayModes.insert(pair<string, DecayMode&>(name, dm));
  if (OptionContainer::GetInstance().GetOption<int>("General.Verbosity") > 0)
    cout << "Registered DecayMode " << name << endl;
}

DecayMode& DecayManager::GetDecayMode(const string name) {
  if (registeredDecayModes.count(name) == 0) {
    throw invalid_argument("DecayMode " + name + " not registered. Aborting.");
  }
  return registeredDecayModes.at(name);
}

void DecayManager::RegisterParticle(Particle* p) {
  registeredParticles.insert(pair<string, Particle*>(p->GetRawName(), p));
  // cout << "Registered particle " << p->GetRawName() << endl;
}

Particle* DecayManager::GetNewParticle(const string name, int Z, int A) {
  if (registeredParticles.count(name) == 0) {
    GenerateNucleus(name, Z, A);
  }
  return new Particle(*(registeredParticles.at(name)));
}

void DecayManager::RegisterDistribution(const string name,
                                        vector<vector<double> >* dist) {
  registeredDistributions.insert(
      pair<string, vector<vector<double> >*>(name, dist));
  // cout << "Registered distribution " << name << endl;
}

vector<vector<double> >* DecayManager::GetDistribution(const string name) {
  if (registeredDistributions.count(name) == 0) {
    throw invalid_argument("Distribution not registered.");
  }
  return registeredDistributions.at(name);
}

void DecayManager::RegisterBasicParticles() {
  RegisterParticle(new Particle("e-", utilities::EMASSC2, -1, 0, 0.5, 0.));
  RegisterParticle(new Particle("e+", utilities::EMASSC2, 1, 0, 0.5, 0.));
  RegisterParticle(new Particle("p", utilities::PMASSC2, 1, 0, 0.5, 0.));
  RegisterParticle(new Particle("n", utilities::NMASSC2, 0, 1, 0.5, 0.));
  RegisterParticle(new Particle("alpha", utilities::ALPHAMASSC2, 2, 2, 0., 0.));
  RegisterParticle(new Particle("enu", 0., 0, 0, 0.5, 0.));
  RegisterParticle(new Particle("enubar", 0., 0, 0, 0.5, 0.));
  RegisterParticle(new Particle("gamma", 0., 0, 0, 0., 0.));
}

void DecayManager::RegisterBasicDecayModes() {
  RegisterDecayMode("BetaMinus", BetaMinus::GetInstance());
  RegisterDecayMode("BetaPlus", BetaPlus::GetInstance());
  // RegisterDecayMode("ConversionElectron", ConversionElectron::GetInstance());
  RegisterDecayMode("Proton", Proton::GetInstance());
  RegisterDecayMode("Alpha", Alpha::GetInstance());
  RegisterDecayMode("Gamma", Gamma::GetInstance());
  RegisterDecayMode("IT", Gamma::GetInstance());
}

void DecayManager::ListRegisteredParticles() {
  cout << "--------------------------------------------------------\n";
  cout << " List of registered particles\n";
  cout << "--------------------------------------------------------\n\n";
  for (map<string, Particle*>::iterator it = registeredParticles.begin();
       it != registeredParticles.end(); ++it) {
    cout << it->second->ListInformation();
    cout << "\n";
  }
  cout << "--------------------------------------------------------\n\n"
       << endl;
}

bool DecayManager::GenerateNucleus(string name, int Z, int A) {
  ostringstream filename;
  filename << OptionContainer::GetInstance().GetOption<string>("radiationdata");
  filename << "z" << Z << ".a" << A;
  ifstream radDataFile((filename.str()).c_str());

  // cout << "Generating nucleus " << name << endl;

  string line;
  double excitationEnergy = 0.;
  double lifetime;
  Particle* p = new Particle(name, utilities::GetApproximateMass(Z, A), Z,
                             (A - Z), 0., 0);

  while (getline(radDataFile, line)) {
    if (!line.compare(0, 1, "#")) {
      continue;
    } else if (!line.compare(0, 1, "P")) {
      istringstream iss(line);
      string p;
      iss >> p >> excitationEnergy >> lifetime;
      continue;
    }
    string mode;
    double daughterExcitationEnergy = 0;
    double intensity = 0;
    double Q = 0;
    string modifier;

    istringstream iss(line);
    iss >> mode >> daughterExcitationEnergy >> intensity >> Q >> modifier;
    if (Q > 0.) {
      // cout << "Adding DecayChannel " << mode << " Excitation Energy " <<
      // excitationEnergy << " to " << daughterExcitationEnergy << endl;
      if (mode.find("shellEC") != string::npos) {
        // TODO
        continue;
      }
      DecayChannel* dc =
          new DecayChannel(mode, Q, intensity, lifetime, excitationEnergy,
                           daughterExcitationEnergy);
      p->AddDecayChannel(dc);
    }
  }

  ostringstream gammaFileSS;
  gammaFileSS << OptionContainer::GetInstance().GetOption<string>("gammadata");
  gammaFileSS << "z" << Z << ".a" << A;
  ifstream gammaDataFile(gammaFileSS.str().c_str());
  if (gammaDataFile.is_open()) {
    while (getline(gammaDataFile, line)) {
      double initEnergy, Q;
      double intensity;
      double convIntensity;
      double kCoeff, lCoeff1, lCoeff2, lCoeff3, mCoeff1, mCoeff2, mCoeff3,
          mCoeff4, mCoeff5;
      double lifetime;
      string angMom;
      string polarity;

      istringstream iss(line);
      iss >> initEnergy >> Q >> intensity >> polarity >> lifetime >> angMom >>
          convIntensity >> kCoeff >> lCoeff1 >> lCoeff2 >> lCoeff3 >> mCoeff1 >>
          mCoeff2 >> mCoeff3 >> mCoeff4 >> mCoeff5;
      // cout << "Adding gamma decay level " << initEnergy << " " << Q << endl;
      DecayChannel* dcGamma =
          new DecayChannel("Gamma", Q, intensity / (1. + convIntensity),
                           lifetime, initEnergy, initEnergy - Q);
      p->AddDecayChannel(dcGamma);
      // TODO Implement conversion electrons
      // p->AddDecayChannel(new DecayChannel("ConversionElectron", Q))
    }
  }
  RegisterParticle(p);
  return true;
}

bool DecayManager::Initialise(string name, int Z, int A,
                              double excitationEnergy, string _filename, int threads) {
  cout << "Initialising..." << endl;
  filename = _filename;
  initStateName = name;
  initExcitationEn = excitationEnergy;
  NRTHREADS = threads;
  struct stat infoRD;
  struct stat infoG;
  int i = stat(
      OptionContainer::GetInstance().GetOption<string>("radiationdata").c_str(),
      &infoRD);
  int j = stat(
      OptionContainer::GetInstance().GetOption<string>("radiationdata").c_str(),
      &infoG);
  if (i == 0 && j == 0 && S_ISDIR(infoRD.st_mode) && S_ISDIR(infoG.st_mode)) {
    return GenerateNucleus(name, Z, A);
  } else {
    cerr << "ERROR: Data files not found. Set CRADLE_RadiationData and "
            "CRADLE_GammaData to their correct folders." << endl;
    return false;
  }
}

std::string DecayManager::GenerateEvent(int eventNr) {
  double time = 0.;
  ostringstream eventDataSS;
  std::vector<Particle*> particleStack;
  Particle* ini = GetNewParticle(initStateName);
  ini->SetExcitationEnergy(initExcitationEn);
  particleStack.push_back(ini);
  while (!particleStack.empty()) {
    Particle* p = particleStack.back();
    // cout << "Decaying particle " << p->GetName() << endl;
    vector<Particle*> finalStates;
    double decayTime = p->IsStable();
    if (decayTime >= 0.) {
      try {
        finalStates = p->Decay();
        time += decayTime;
        // cout << "Decay finished" << endl;
      } catch (const std::invalid_argument& e) {
        // cout << "Decay Mode for particle " << p->GetName() << " not found.
        // Aborting." << endl;
        return false;
      }
    } else {
      // cout << "Particle " << p->GetName() << " is stable" << endl;
      eventDataSS << eventNr << "\t" << time << "\t" << p->GetInfoForFile() << "\n";
    }
    delete particleStack.back();
    particleStack.pop_back();
    if (!finalStates.empty()) {
      particleStack.insert(particleStack.end(), finalStates.begin(),
                           finalStates.end());
    }
  }
  return eventDataSS.str();
}

bool DecayManager::MainLoop(int nrParticles) {
  cout << "Starting Main Loop " << endl;
  ofstream fileStream;
  fileStream.open(filename.c_str());

  std::ios::sync_with_stdio(false);
  boost::progress_display show_progress(nrParticles);
  boost::progress_timer t;
  for (int i = 0; i < nrParticles; i+=NRTHREADS) {
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
