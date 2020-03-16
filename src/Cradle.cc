#include "CRADLE/Cradle.h"
#include "CRADLE/ConfigParser.h"
#include "CRADLE/ReactionEngine.h"

#include "PDS/Factory/ParticleFactory.h"

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include <queue>

namespace CRADLE {

  Cradle::Cradle(std::string _outputName) : outputName(_outputName) {
    InitialiseLoggers();
    PDS::ParticleFactory::RegisterBasicParticles();
  }

  Cradle::~Cradle() { }

  void Cradle::Initialise(std::string configFilename, int argc, const char** argv) {
    ConfigOptions configOptions = ParseOptions(configFilename, argc, argv);
    Initialise(configOptions);
    reactionEngine = new ReactionEngine();
  }

  void Cradle::Initialise(ConfigOptions _configOptions) {
    //cout << "Initialising..." << endl;
    //TODO check
    configOptions = &_configOptions;
    initStateName = configOptions->nuclearOptions.Name;
    initExcitationEn = configOptions->nuclearOptions.Energy;
    outputName = configOptions->general.Output;

    //  PDS::ParticleFactory::GenerateNucleus(initStateName, configOptions.nuclearOptions.Charge, configOptions.nuclearOptions.Nucleons);
  }

  void Cradle::SetReactionEngine(std::shared_ptr<ReactionEngine> _reactionEngine){
    reactionEngine = _reactionEngine;
  }

  Event Cradle::BreadthFirstDecay(PDS::core::DynamicParticle& initState, int maxDepth) {
    Event event;
    std::queue<PDS::core::DynamicParticle> queue;

    int currentDepth = 0;
    int depthIndex = 0;

    queue.push(initState);

    while (!queue.empty()) {
      event.AddVertex(reactionEngine->ProcessParticle(queue.pop()));
      // --depthIndex;
      // if (depthIndex < 0) {
      //   ++currentDepth;
      //   depthIndex = queue.size()-1;
      //   for (auto & p : event.GetLastVertex().GetParticlesOut()) {
      //     if (!p.IsStable()) {
      //       ++depthIndex;
      //     }
      //   }
      //   //depthIndex = event.GetLastVertex().GetParticlesOut().size()-1;
      // }
      for (auto & p : event.GetLastVertex().GetParticlesOut()) {
        if (!p.IsStable()) {
          queue.push(p);
        }
      }
    }

    return event;
  }

  Event Cradle::Next() {
    PDS::core::DynamicParticle initState = ConstructInitialParticle();

    return BreadthFirstDecay(initState, -1);
  }

  void Cradle::GenerateEvents(int nrEvents) {
    //Check why this is
    std::ios::sync_with_stdio(false);

    for (int i = 0; i < nrEvents; ++i) {
      Next();
    }
  }

  void Cradle::MainLoop(int nrEvents, int nrThreads) {
    int Q = (nrEvents-1)/nrThreads;
    int R = (nrEvents-1)%nrThreads;

    std::future<std::string> f[nrThreads];
    for (int t = 0; t < nrThreads; t++) {
      if (t<R) f[t] = std::async(std::launch::async, &Cradle::GenerateEvents, this, Q+1);
      else f[t] = std::async(std::launch::async, &Cradle::GenerateEvents, this, Q);
    }
  }

  //
  // std::string Cradle::GenerateEvents(int nrEvents) {
  //   std::ios::sync_with_stdio(false);
  //   // boost::progress_display show_progress(nrEvents);
  //   // boost::progress_timer t;
  //
  //   std::ostringstream threadDataSS;
  //   for(int i=0; i < nrEvents ; i++){
  //     threadDataSS << reactionEngine->GenerateEvent(i, initStateName, initExcitationEn, *configOptions);
  //     // ++show_progress;
  //   }
  //   return threadDataSS.str();
  // }

  //Initialization/loading

  void Cradle::InitialiseLoggers() {
    /**
    * Remove result & log files if they already exist
    */
    if (std::ifstream(outputName + ".log")) std::remove((outputName + ".log").c_str());
    if (std::ifstream(outputName + ".raw")) std::remove((outputName + ".raw").c_str());
    if (std::ifstream(outputName + ".txt")) std::remove((outputName + ".txt").c_str());

    debugFileLogger = spdlog::get("debug_file");
    if (!debugFileLogger) {
      debugFileLogger = spdlog::basic_logger_mt("debug_file", outputName + ".log");
      debugFileLogger->set_level(spdlog::level::debug);
    }
    debugFileLogger->debug("Debugging logger created");
    consoleLogger = spdlog::get("console");
    if (!consoleLogger) {
      consoleLogger = spdlog::stdout_color_mt("console");
      consoleLogger->set_pattern("%v");
      consoleLogger->set_level(spdlog::level::warn);
    }
    debugFileLogger->debug("Console logger created");
    rawSpectrumLogger = spdlog::get("CRADLE_raw");
    if (!rawSpectrumLogger) {
      rawSpectrumLogger = spdlog::basic_logger_mt("CRADLE_raw", outputName + ".raw");
      rawSpectrumLogger->set_pattern("%v");
      rawSpectrumLogger->set_level(spdlog::level::info);
    }
    debugFileLogger->debug("Raw spectrum logger created");
    resultsFileLogger = spdlog::get("CRADLE_results_file");
    if (!resultsFileLogger) {
      resultsFileLogger = spdlog::basic_logger_mt("CRADLE_results_file", outputName + ".txt");
      resultsFileLogger->set_pattern("%v");
      resultsFileLogger->set_level(spdlog::level::info);
    }
    debugFileLogger->debug("Results file logger created");
  }

}//end of CRADLE namespace
