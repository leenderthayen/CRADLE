#include "CRADLE/DecayManager.hh"
#include "CRADLE/Utilities.hh"
#include "CRADLE/DecayChannel.hh"
#include "CRADLE/Particle.hh"
#include "CRADLE/DecayMode.hh"
#include "CRADLE/SpectrumGenerator.hh"

#include "TFile.h"
#include "TTree.h"

#include <boost/progress.hpp>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <sys/types.h>
#include <sys/stat.h>
#include <future>

#include <iomanip>

template <typename A, typename B>
std::pair<B, A> flip_pair(const std::pair<A, B> &p)
{
  return std::pair<B, A>(p.second, p.first);
}

template <typename A, typename B>
std::multimap<B, A> flip_map(const std::map<A, B> &src)
{
  std::multimap<B, A> dst;
  std::transform(src.begin(), src.end(), std::inserter(dst, dst.begin()),
                 flip_pair<A, B>);
  return dst;
}

namespace CRADLE
{

  using std::cout;
  using std::endl;
  using std::map;
  using std::pair;
  using std::string;
  using std::vector;

  DecayManager::~DecayManager()
  {
    // cout << "Destroying decaymanager" << endl;
    /*for (vector<Particle*>::iterator it = particleStack.begin();
         it != particleStack.end(); ++it) {
      delete *it;
    }*/
    for (map<const string, Particle *>::iterator it = registeredParticles.begin();
         it != registeredParticles.end(); ++it)
    {
      for (vector<DecayChannel *>::iterator it2 =
               (it->second)->GetDecayChannels().begin();
           it2 != (it->second)->GetDecayChannels().end(); ++it2)
      {
        delete *it2;
      }
      delete it->second;
    }
    registeredParticles.clear();

    for (map<const string, vector<vector<double>> *>::iterator it =
             registeredDistributions.begin();
         it != registeredDistributions.end(); ++it)
    {
      delete it->second;
    }
    registeredDistributions.clear();
  }

  void DecayManager::RegisterDecayMode(const string name, DecayMode &dm)
  {
    registeredDecayModes.insert(pair<string, DecayMode &>(name, dm));
    if (configOptions.general.Verbosity > 0)
      cout << "Registered DecayMode " << name << endl;
  }

  DecayMode &DecayManager::GetDecayMode(const string name)
  {
    if (registeredDecayModes.count(name) == 0)
    {
      throw std::invalid_argument("DecayMode " + name + " not registered. Aborting.");
    }
    return registeredDecayModes.at(name);
  }

  void DecayManager::RegisterParticle(Particle *p)
  {
    registeredParticles.insert(pair<string, Particle *>(p->GetRawName(), p));
    // cout << "Registered particle " << p->GetRawName() << endl;
  }

  Particle *DecayManager::GetNewParticle(const string name, int Z, int A)
  {
    if (registeredParticles.count(name) == 0)
    {
      GenerateNucleus(name, Z, A);
    }
    return new Particle(*(registeredParticles.at(name)));
  }

  void DecayManager::RegisterDistribution(const string name,
                                          vector<vector<double>> *dist)
  {
    registeredDistributions.insert(
        pair<string, vector<vector<double>> *>(name, dist));
    // cout << "Registered distribution " << name << endl;
  }

  vector<vector<double>> *DecayManager::GetDistribution(const string name)
  {
    if (registeredDistributions.count(name) == 0)
    {
      throw std::invalid_argument("Distribution not registered.");
    }
    return registeredDistributions.at(name);
  }

  /////// ajout de SL 12/05/2023//////////////////////////////////////
  void DecayManager::RegisterBetaType(const string name,
                                      const string nameType)
  {
    registeredBetaType.insert(
        pair<string, string>(name, nameType));
    // cout << "Registered BetaType " << name << endl;
  }

  string DecayManager::GetBetaType(const string name)
  {

    if (registeredBetaType.count(name) == 0)
    {
      throw std::invalid_argument("Transition not registered.");
    }
    return registeredBetaType.at(name);
  }
  /////////////////////////////////////////////////////////////////////

  void DecayManager::RegisterBasicParticles()
  {
    RegisterParticle(new Particle("e-", utilities::EMASSC2, -1, 0, 0.5, 0.));
    RegisterParticle(new Particle("e+", utilities::EMASSC2, 1, 0, 0.5, 0.));
    RegisterParticle(new Particle("p", utilities::PMASSC2, 1, 0, 0.5, 0.));
    RegisterParticle(new Particle("n", utilities::NMASSC2, 0, 1, 0.5, 0.));
    RegisterParticle(new Particle("alpha", utilities::ALPHAMASSC2, 2, 2, 0., 0.));
    RegisterParticle(new Particle("enu", 0., 0, 0, 0.5, 0.));
    RegisterParticle(new Particle("enubar", 0., 0, 0, 0.5, 0.));
    RegisterParticle(new Particle("gamma", 0., 0, 0, 0., 0.));
  }

  void DecayManager::RegisterBasicDecayModes()
  {
    RegisterDecayMode("BetaMinus", BetaMinus::GetInstance());
    RegisterDecayMode("BetaPlus", BetaPlus::GetInstance());
    // RegisterDecayMode("ConversionElectron", ConversionElectron::GetInstance());
    RegisterDecayMode("Proton", Proton::GetInstance());
    RegisterDecayMode("Alpha", Alpha::GetInstance());
    RegisterDecayMode("Gamma", Gamma::GetInstance());
    RegisterDecayMode("IT", Gamma::GetInstance());
  }

  void DecayManager::RegisterSpectrumGenerator(const string decayMode, SpectrumGenerator &sg)
  {
    try
    {
      DecayMode &dm = GetDecayMode(decayMode);
      dm.SetSpectrumGenerator(&sg);
      if (configOptions.general.Verbosity > 0)
        cout << "Registered " << decayMode << " Spectrum Generator " << typeid(sg).name() << endl;
    }
    catch (const std::invalid_argument &e)
    {
      cout << "Cannot register" << typeid(sg).name() << "spectrum generator. Decay mode "
           << decayMode << " not registered." << endl;
    }
  }

  void DecayManager::RegisterBasicSpectrumGenerators()
  {
    RegisterSpectrumGenerator("Proton", DeltaSpectrumGenerator::GetInstance());
    RegisterSpectrumGenerator("Alpha", DeltaSpectrumGenerator::GetInstance());
    RegisterSpectrumGenerator("Gamma", DeltaSpectrumGenerator::GetInstance());
    RegisterSpectrumGenerator("IT", DeltaSpectrumGenerator::GetInstance());
    RegisterSpectrumGenerator("BetaPlus", SimpleBetaDecay::GetInstance());
    RegisterSpectrumGenerator("BetaMinus", SimpleBetaDecay::GetInstance());
  }

  void DecayManager::ListRegisteredParticles()
  {
    cout << "--------------------------------------------------------\n";
    cout << " List of registered particles\n";
    cout << "--------------------------------------------------------\n\n";
    for (map<const string, Particle *>::iterator it = registeredParticles.begin();
         it != registeredParticles.end(); ++it)
    {
      cout << it->second->ListInformation();
      cout << "\n";
    }
    cout << "--------------------------------------------------------\n\n"
         << endl;
  }

  bool DecayManager::GenerateNucleus(string name, int Z, int A)
  {
    std::ostringstream filename;
    filename << configOptions.envOptions.Radiationdata;
    filename << "/z" << Z << ".a" << A;
    std::ifstream radDataFile((filename.str()).c_str());

    // cout << "Generating nucleus " << name << endl;

    string line;
    double excitationEnergy = 0.;
    double lifetime;
    double atomicMass = utilities::GetAMEMass(configOptions.envOptions.AMEdata, Z, A);

    if (atomicMass == 0)
    {
      atomicMass = utilities::GetApproximateMass(Z, A);
    }

    Particle *p = new Particle(name, atomicMass, Z,
                               (A - Z), 0., 0);

    // cout << filename.str() << endl;

    while (getline(radDataFile, line))
    {
      // cout<<line<<endl;

      if (!line.compare(0, 1, "#"))
      {
        // Comment line
        continue;
      }
      else if (!line.compare(0, 1, "P"))
      {
        // Parent line
        std::istringstream iss(line);
        string p;
        string flag;

        iss >> p >> excitationEnergy >> flag >> lifetime;
        continue;
      }
      // cout << "Lifetime: " << lifetime << endl;
      string mode;
      double daughterExcitationEnergy = 0;
      double intensity = 0;
      double Q = 0;
      string modifier;
      string flag;

      std::istringstream iss(line);
      iss >> mode >> daughterExcitationEnergy >> flag >> intensity >> Q >> modifier;
      // cout << "Mode : " << mode << endl;
      // cout << "Daughter Energy :" << daughterExcitationEnergy << endl;
      // cout << "Flag :" << flag <<endl;
      // cout << "Intensity : " << intensity << endl;
      // cout << "Q : " << Q << endl;
      // cout << "Modifier : " << modifier << endl;
      // cout << "\n" <<endl;

      if (Q > 0.)
      {
        /*cout << "Adding DecayChannel " << mode << " Excitation Energy " <<
        excitationEnergy << " to " << daughterExcitationEnergy << endl;*/
        if (mode.find("shellEC") != string::npos)
        {
          // TODO
          continue;
        }
        DecayChannel *dc =
            new DecayChannel(mode, &GetDecayMode(mode), Q, intensity, lifetime, excitationEnergy,
                             daughterExcitationEnergy);
        p->AddDecayChannel(dc);
      }
    }

    std::ostringstream gammaFileSS;
    gammaFileSS << configOptions.envOptions.Gammadata;
    gammaFileSS << "z" << Z << ".a" << A;
    std::ifstream gammaDataFile(gammaFileSS.str().c_str());
    if (gammaDataFile.is_open())
    {
      while (getline(gammaDataFile, line))
      {
        int levelNr;
        double initEnergy, E;
        double intensity;
        double convIntensity;
        double kCoeff, lCoeff1, lCoeff2, lCoeff3, mCoeff1, mCoeff2, mCoeff3,
            mCoeff4, mCoeff5;
        double lifetime;
        string angMom;
        string polarity;
        string flag;

        int nGammas;

        std::istringstream iss(line);
        iss >> levelNr >> flag >> initEnergy >> lifetime >> angMom >> nGammas;
        for (int i = 0; i < nGammas; ++i)
        {
          getline(gammaDataFile, line);
          int daughterLevelNr;
          int multipolarity;
          double multipolarityMixing;
          std::istringstream issLevel(line);
          issLevel >> daughterLevelNr >> E >> intensity >> multipolarity >> multipolarityMixing >> convIntensity >> kCoeff >> lCoeff1 >> lCoeff2 >> lCoeff3 >> mCoeff1 >>
              mCoeff2 >> mCoeff3 >> mCoeff4 >> mCoeff5;

          // cout << "Adding gamma decay level " << initEnergy << " " << E << endl;
          if ((initEnergy - E) >= 0)
          {
            DecayChannel *dcGamma =
                new DecayChannel("Gamma", &GetDecayMode("Gamma"), E, intensity / (1. + convIntensity),
                                 lifetime, initEnergy, initEnergy - E);
            p->AddDecayChannel(dcGamma);
          }
          else
          {
            std::cerr << "WARNING: Attempted to add gamma branch to a final state with negative excitation energy. Please check you are using the correct version of PhotonEvaporation.\nCurrent filename: " << gammaFileSS.str() << std::endl;
          }
        }
        // iss >> initEnergy >> Q >> intensity >> polarity >> lifetime >> angMom >>
        //    convIntensity >> kCoeff >> lCoeff1 >> lCoeff2 >> lCoeff3 >> mCoeff1 >>
        //    mCoeff2 >> mCoeff3 >> mCoeff4 >> mCoeff5;
        // TODO Implement conversion electrons
        // p->AddDecayChannel(new DecayChannel("ConversionElectron", Q))
      }
    }
    RegisterParticle(p);
    return true;
  }

  bool DecayManager::Initialise(std::string configFilename, int argc, const char **argv)
  {
    ConfigOptions configOptions = ParseOptions(configFilename, argc, argv);
    return Initialise(configOptions);
  }

  bool DecayManager::Initialise(ConfigOptions _configOptions)
  {
    // cout << "Initialising..." << endl;
    configOptions = _configOptions;
    initStateName = configOptions.nuclearOptions.Name;
    initExcitationEn = configOptions.nuclearOptions.Energy;
    outputName = configOptions.general.Output;
    NRTHREADS = configOptions.general.Threads;
    if (initStateName != "" && configOptions.nuclearOptions.Nucleons > 0)
    {
      struct stat infoRD;
      struct stat infoG;
      int i = stat(
          configOptions.envOptions.Radiationdata.c_str(),
          &infoRD);
      int j = stat(
          configOptions.envOptions.Gammadata.c_str(),
          &infoG);

      if (i == 0 && j == 0 && S_ISDIR(infoRD.st_mode) && S_ISDIR(infoG.st_mode))
      {
        RegisterBasicParticles();
        RegisterBasicDecayModes();
        RegisterBasicSpectrumGenerators();
        return GenerateNucleus(initStateName, configOptions.nuclearOptions.Charge, configOptions.nuclearOptions.Nucleons);
      }
      else
      {
        std::cerr << "ERROR: Data files not found. Set Radiationdata and "
                     "Gammadata to their correct folders."
                  << std::endl;
        return false;
      }
    }
    else
    {
      std::cerr << "ERROR: Initial nucleus is not defined." << std::endl;
      return false;
    }
  }

  // working with 0,0,0 and time
  //  std::string DecayManager::GenerateEvent(int eventNr) {
  //    double time = 0.;
  //    double checkTime = 0.;
  //    int subEventNr=0;
  //    int totSubEvents = 0;
  //    int totEvents = 0;
  //    std::ostringstream eventData;
  //    std::ostringstream subHeader;
  //    std::ostringstream subEventData;
  //    std::vector<Particle *> particleStack;
  //    Particle* ini = GetNewParticle(initStateName);
  //    ini->SetExcitationEnergy(initExcitationEn);
  //    particleStack.push_back(ini);
  //    while (!particleStack.empty()) {

  //     Particle* p = particleStack.back();
  //     cout << "\n Decaying particle " << p->GetName() << endl;
  //     vector<Particle*> finalStates;
  //     double decayTime = p->GetDecayTime();
  //     std::cout << eventNr << "\t" << subEventNr << std::endl;
  //     std::cout << "     Time =\t" << time      << "\n "
  //               << "CheckTime =\t" << checkTime << "\n "
  //               << "decayTime =\t" << decayTime << std::endl;

  //     if ((time + decayTime) <= configOptions.cuts.Lifetime)
  //     {
  //       try
  //       {
  //         finalStates = p->Decay();
  //         time += decayTime;
  //         //cout << "Decay finished" << endl;
  //       }
  //       catch (const std::invalid_argument& e)
  //       {
  //         std::cout << "Decay Mode for particle " << p->GetName() << " not found. Aborting." << endl;
  //         return "";
  //       }
  //     }
  //     else
  //     {
  //       if (time != checkTime)
  //       {
  //         subHeader << eventNr << std::setw(8) << subEventNr << "\t\t" << totSubEvents << "\n" << subEventData.str();
  //         totSubEvents = 0;
  //         ++subEventNr;
  //         checkTime = time;

  //         subEventData.str(std::string());
  //       }
  //       ++totEvents;
  //       ++totSubEvents;

  //       subEventData << eventNr << "\t\t" << std::fixed<<std::setprecision(4)<<roundf(time*100)/100. << "\t" << p->GetInfoForFile() << "\n";
  //     }
  //     delete particleStack.back();
  //     particleStack.pop_back();
  //     if (!finalStates.empty())
  //     {
  //       particleStack.insert(particleStack.end(), finalStates.begin(),
  //                            finalStates.end());
  //     }
  //   }
  //   //Write down the last event that occured!
  //   subHeader << eventNr << std::setw(8) << subEventNr << "\t\t" << totSubEvents << "\n"
  //             << subEventData.str();
  //   eventData << eventNr << "\t\t" << totEvents << "\n"
  //             << subHeader.str();

  //   cout<<"start"<<endl;
  //   cout<<eventData.str()<<endl;
  //   cout<<"end"<<endl;

  //   return eventData.str();
  // }

  std::vector<ParticleData> DecayManager::GenerateEvent_ROOT(int eventNr, int verbosity)
  {
    std::vector<ParticleData> vec;
    ParticleData ParticleData_ini;
    ParticleData_ini.event = 0;
    ParticleData_ini.time = 0;
    ParticleData_ini.code = 0;
    ParticleData_ini.excitation_energy = 0;
    ParticleData_ini.kinetic_energy = 0;
    ParticleData_ini.px = 0;
    ParticleData_ini.py = 0;
    ParticleData_ini.pz = 0;
    double time = 0.;
    double checkTime = 0.;
    int totEvents = 0;
    std::vector<Particle *> particleStack;
    Particle *ini = GetNewParticle(initStateName);
    ini->SetExcitationEnergy(initExcitationEn);
    particleStack.push_back(ini);
    while (!particleStack.empty())
    {
       ParticleData_ini.event = 0;
        ParticleData_ini.time = 0;
        ParticleData_ini.code = 0;
        ParticleData_ini.excitation_energy = 0;
        ParticleData_ini.kinetic_energy = 0;
        ParticleData_ini.px = 0;
        ParticleData_ini.py = 0;
        ParticleData_ini.pz = 0;
        double mom;

      Particle *p = particleStack.back();
      vector<Particle *> finalStates;
      double decayTime = p->GetDecayTime();
      // cout << "\n Decaying particle " << p->GetName() << endl;
      // std::cout << eventNr << "\t" << subEventNr << std::endl;
      // std::cout << "     Time =\t" << time    if (particleDefinition) {
      //           << "CheckTime =\t" << checkTime << "\n "
      //           << "decayTime =\t" << decayTime << std::endl;
      if (verbosity == 0)
      {
        if (p->GetRawName() == "p" || p->GetRawName() == "e+" || p->GetRawName() == "e-" || p->GetRawName() == "gamma" || p->GetRawName() == "alpha" || p->GetRawName() == "2He" || p->GetRawName() == "n")
        {
          vec.push_back(ParticleData_ini);
          vec[totEvents].event = eventNr;
          vec[totEvents].code = screening::NametoPDG(p->GetRawName());
          vec[totEvents].time = roundf(time * 10000) / 10000.;
          vec[totEvents].excitation_energy = p->GetExcitationEnergy();
          vec[totEvents].kinetic_energy = p->GetKinEnergy();
          mom = sqrt(p->GetMomentum()[1]*p->GetMomentum()[1] + p->GetMomentum()[2]*p->GetMomentum()[2] + p->GetMomentum()[3]*p->GetMomentum()[3]);
          vec[totEvents].px = p->GetMomentum()[1] / (mom);
          vec[totEvents].py = p->GetMomentum()[2] / (mom);
          vec[totEvents].pz = p->GetMomentum()[3] / (mom);
          ++totEvents;
        }
      }
      else
      {
        vec.push_back(ParticleData_ini);
        vec[totEvents].event = eventNr;
        vec[totEvents].code = screening::NametoPDG(p->GetRawName());
        vec[totEvents].time = roundf(time * 10000) / 10000.;
        vec[totEvents].excitation_energy = p->GetExcitationEnergy();
        vec[totEvents].kinetic_energy = p->GetKinEnergy();
        mom = sqrt(p->GetMomentum()[1]*p->GetMomentum()[1] + p->GetMomentum()[2]*p->GetMomentum()[2] + p->GetMomentum()[3]*p->GetMomentum()[3]);
        vec[totEvents].px = p->GetMomentum()[1] / (mom);
        vec[totEvents].py = p->GetMomentum()[2] / (mom);
        vec[totEvents].pz = p->GetMomentum()[3] / (mom);
        ++totEvents;
      }

      if ((time + decayTime) <= configOptions.cuts.Lifetime)
      {
        try
        {
          time += decayTime;
          finalStates = p->Decay();
        }
        catch (const std::invalid_argument &e)
        {
          std::cerr << "Decay Mode for particle " << p->GetName() << " not found. Aborting." << endl;
        }
      }
      delete particleStack.back();
      particleStack.pop_back();
      if (!finalStates.empty())
      {
        particleStack.insert(particleStack.end(), finalStates.begin(),
                             finalStates.end());
      }
    }
    return vec;
  }

  std::string DecayManager::GenerateEvent_TXT(int eventNr, int verbosity)
  {
    double time = 0.;
    double checkTime = 0.;
    int subEventNr = 0;
    int totSubEvents = 0;
    int totEvents = 0;
    std::ostringstream eventData;
    std::ostringstream subHeader;
    std::ostringstream subEventData;
    std::vector<Particle *> particleStack;
    Particle *ini = GetNewParticle(initStateName);
    ini->SetExcitationEnergy(initExcitationEn);
    particleStack.push_back(ini);
    if (verbosity == 0)
    {
      while (!particleStack.empty())
      {
        Particle *p = particleStack.back();
        vector<Particle *> finalStates;
        double decayTime = p->GetDecayTime();
        // cout << "\n Decaying particle " << p->GetRawName() << endl;
        //  std::cout << eventNr << "\t" << subEventNr << std::endl;
        //  std::cout << "     Time =\t" << time      << "\n "
        //   << "CheckTime =\t" << checkTime << "\n "
        //   << "decayTime =\t" << decayTime << std::endl;

        if (decayTime > configOptions.cuts.Lifetime && p->GetRawName() == "p" || p->GetRawName() == "e+" || p->GetRawName() == "e-" || p->GetRawName() == "gamma" || p->GetRawName() == "alpha" || p->GetRawName() == "2He" || p->GetRawName() == "n")
        {
          ++totSubEvents;
          subEventData << eventNr << "\t\t" << std::fixed << std::setprecision(4) << roundf(time * 10000) / 10000. << "\t" << p->GetInfoForFile() << "\n";
        }
        if ((time + decayTime) <= configOptions.cuts.Lifetime)
        {
          try
          {
            time += decayTime;
            finalStates = p->Decay();
          }
          catch (const std::invalid_argument &e)
          {
            std::cout << "Decay Mode for particle " << p->GetName() << " not found. Aborting." << endl;
            return "";
          }
        }

        delete particleStack.back();
        particleStack.pop_back();
        if (!finalStates.empty())
        {
          particleStack.insert(particleStack.end(), finalStates.begin(),
                               finalStates.end());
        }
      }
    }
    else
    {
      while (!particleStack.empty())
      {
        Particle *p = particleStack.back();
        vector<Particle *> finalStates;
        double decayTime = p->GetDecayTime();
        // cout << "\n Decaying particle " << p->GetRawName() << endl;
        //  std::cout << eventNr << "\t" << subEventNr << std::endl;
        //  std::cout << "     Time =\t" << time      << "\n "
        //   << "CheckTime =\t" << checkTime << "\n "
        //   << "decayTime =\t" << decayTime << std::endl;

        ++totSubEvents;
        subEventData << eventNr << "\t\t" << std::fixed << std::setprecision(4) << roundf(time * 10000) / 10000. << "\t" << p->GetInfoForFile() << "\n";

        if ((time + decayTime) <= configOptions.cuts.Lifetime)
        {
          try
          {
            time += decayTime;
            finalStates = p->Decay();
            subHeader << eventNr << std::setw(8) << subEventNr << "\t\t" << totSubEvents << "\n"
                      << subEventData.str();
            subEventData.str(std::string());
            totEvents += totSubEvents;
            totSubEvents = 0;
            ++subEventNr;
          }
          catch (const std::invalid_argument &e)
          {
            std::cout << "Decay Mode for particle " << p->GetName() << " not found. Aborting." << endl;
            return "";
          }
        }

        delete particleStack.back();
        particleStack.pop_back();
        if (!finalStates.empty())
        {
          particleStack.insert(particleStack.end(), finalStates.begin(),
                               finalStates.end());
        }
      }
    }
    totEvents += totSubEvents;
    subHeader << eventNr << std::setw(8) << subEventNr << "\t\t" << totSubEvents << "\n"
              << subEventData.str();
    eventData << eventNr << "\t\t" << totEvents << "\n"
              << subHeader.str();

    return eventData.str();
  }

  // old one
  //  std::string DecayManager::GenerateEvent(int eventNr) {
  //    double time = 0.;
  //    std::ostringstream eventDataSS;
  //    std::vector<Particle*> particleStack;
  //    Particle* ini = GetNewParticle(initStateName);
  //    ini->SetExcitationEnergy(initExcitationEn);
  //    particleStack.push_back(ini);
  //    while (!particleStack.empty()) {
  //      Particle* p = particleStack.back();
  //      //cout << "Decaying particle " << p->GetName() << endl;
  //      vector<Particle*> finalStates;
  //      double decayTime = p->GetDecayTime();

  //     if ((time + decayTime) <= configOptions.cuts.Lifetime) {

  //       try {
  //         finalStates = p->Decay();
  //         time += decayTime;
  //         //cout << "Decay finished" << endl;
  //       } catch (const std::invalid_argument& e) {
  //         cout << "Decay Mode for particle " << p->GetName() << " not found. Aborting." << endl;
  //         return "";
  //       }
  //     } else {
  //       //cout << "Particle " << p->GetName() << " is stable" << endl;
  //       eventDataSS << eventNr << "\t" << time << "\t" << p->GetInfoForFile() << "\n";
  //     }
  //     delete particleStack.back();
  //     particleStack.pop_back();
  //     if (!finalStates.empty()) {
  //       particleStack.insert(particleStack.end(), finalStates.begin(),
  //                            finalStates.end());
  //     }
  //   }
  //   return eventDataSS.str();
  // }

  bool DecayManager::MainLoop()
  {
    int nrParticles = configOptions.general.Loop;
    int verbosity = configOptions.general.Verbosity_file;
    if (nrParticles < 1)
    {
      std::cerr << "ERROR: Incorrect number of events (" << nrParticles << ")" << std::endl;
      return true;
    }
    cout << "Starting Main Loop (" << nrParticles << " events)" << endl;

    std::ios::sync_with_stdio(false);
    boost::progress_display show_progress(nrParticles);
    boost::progress_timer t;
    // fileStream << GenerateEvent(0);   ///// and i started to 0 before

    if (outputName.find("root") != std::string::npos)
    {
      TFile* outputFile = new TFile(outputName.c_str(), "RECREATE");
      TTree* tree = new TTree("ParticleTree", "Tree for Particle Data");
      
      ParticleData pData;
      tree->Branch("event", &pData.event);
      tree->Branch("time", &pData.time);
      tree->Branch("code", &pData.code);
      tree->Branch("energy", &pData.kinetic_energy);
      tree->Branch("excitation_energy", &pData.excitation_energy);
      tree->Branch("px", &pData.px);
      tree->Branch("py", &pData.py);
      tree->Branch("pz", &pData.pz);

      for (int i = 0; i < nrParticles; i += NRTHREADS)
      {
        // cout << "LOOP NR " << i+1 << endl;
        int threads = std::min(NRTHREADS, nrParticles - i);
        std::future<std::vector<ParticleData>> f[threads];
        for (int t = 0; t < threads; t++)
        {
          f[t] = std::async(std::launch::async, &DecayManager::GenerateEvent_ROOT, this, i + t, verbosity);
        }

        for (int t = 0; t < threads; t++)
        {
          for (const auto &particle : f[t].get())
          {
            pData = particle;
            tree->Fill();
          }
          ++show_progress;
        }
      }
      outputFile->Write();
      outputFile->Close();
    }

    else if (outputName.find("txt") != std::string::npos)
    {
      std::ofstream fileStream;
      fileStream.open(outputName.c_str());
      for (int i = 0; i < nrParticles; i += NRTHREADS)
      {
        // cout << "LOOP NR " << i+1 << endl;
        int threads = std::min(NRTHREADS, nrParticles - i);
        std::future<std::string> f[threads];
        for (int t = 0; t < threads; t++)
        {
          f[t] = std::async(std::launch::async, &DecayManager::GenerateEvent_TXT, this, i + t, verbosity);
        }

        for (int t = 0; t < threads; t++)
        {
          fileStream << f[t].get();
          ++show_progress;
        }
      }
      fileStream.flush();
      fileStream.close();
    }
    else
    {
      std::cerr << ("Choose .txt or .root for your output file") << std::endl;
      return true;
    }

    std::cout << "Done! Time taken: ";
    return true;
  }

} // End of CRADLE namespace