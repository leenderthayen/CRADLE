#include "CRADLE/Cradle.h"
#include "CRADLE/ConfigParser.h"
#include "CRADLE/ReactionEngine.h"

#include "PDS/Factory/ParticleFactory.h"

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include <queue>
#include <utility>

namespace CRADLE {

  Cradle::Cradle(std::string _outputName) : outputName(_outputName) {
    InitialiseLoggers();
    PDS::ParticleFactory::RegisterBasicParticles();
    reactionEngine = std::make_shared<ReactionEngine>();
  }

  Cradle::~Cradle() {
  }

  void Cradle::Initialise(std::string configFilename, int argc, const char** argv) {
    ConfigOptions configOptions = ParseOptions(configFilename, argc, argv);
    Initialise(configOptions);
  }

  void Cradle::Initialise(ConfigOptions _configOptions) {
    //cout << "Initialising..." << endl;
    //TODO check
    configOptions = _configOptions;
    initStateName = configOptions.nuclearOptions.Name;
    initExcitationEn = configOptions.nuclearOptions.Energy;
    outputName = configOptions.general.Output;
  }

  void Cradle::SetReactionEngine(std::shared_ptr<ReactionEngine> _reactionEngine){
    reactionEngine = _reactionEngine;
  }

  std::shared_ptr<PDS::core::DynamicParticle> Cradle::ConstructInitialParticle() {
    PDS::core::DynamicParticle dynPart = PDS::ParticleFactory::GetNewDynamicParticleFromGeant4(
      configOptions.nuclearOptions.Charge, configOptions.nuclearOptions.Nucleons, configOptions.nuclearOptions.Energy);
    return std::make_shared<PDS::core::DynamicParticle>(std::move(dynPart));
  }

  std::shared_ptr<PDS::core::Vertex> Cradle::ConstructInitialVertex() {
    // Initial vertex has ID 0
    std::shared_ptr<PDS::core::Vertex> initVertex = std::make_shared<PDS::core::Vertex>(0);
    ublas::vector<double> initPos(4);
    initPos(0) = 0;
    initPos(1) = 0;
    initPos(2) = 0;
    initPos(3) = 0;

    initVertex->SetPosition(initPos);

    std::shared_ptr<PDS::core::DynamicParticle> initState = ConstructInitialParticle();
    initState->SetProductionVertex(initVertex);
    initVertex->AddParticleOut(initState);

    return initVertex;
  }

  // Simple FIFO (First In, First Out) decayer with no cuts
  // Using std::queue, simply decays until all products are stable
  std::vector<std::shared_ptr<PDS::core::Vertex> > Cradle::UnlimitedDecay(const std::shared_ptr<PDS::core::Vertex>& prodVertex) {
    std::vector<std::shared_ptr<PDS::core::Vertex> > vertexCollection;
    std::queue<std::shared_ptr<PDS::core::DynamicParticle> > queue;

    for (auto & p : prodVertex->GetParticlesOut()) {
      if (!p->IsStable()) {
        queue.push(p);
      }
    }

    unsigned vertexID = 1;

    while (!queue.empty()) {
      std::shared_ptr<PDS::core::Vertex> v =
      reactionEngine->ProcessParticle(queue.front(), queue.front()->GetProductionVertex()->GetPosition());
      queue.pop();
      v->SetID(vertexID);
      vertexID++;
      vertexCollection.push_back(v);
      for (auto & p : vertexCollection.back()->GetParticlesOut()) {
        if (!p->IsStable()) {
          queue.push(p);
        }
      }
    }
    return vertexCollection;
  }

  Event Cradle::BreadthFirstDecay(const std::shared_ptr<PDS::core::Vertex>& prodVertex, int maxDepth) {
    Event event(0);
    std::vector<PDS::core::Vertex> vertexCollection;
    std::queue<std::shared_ptr<PDS::core::DynamicParticle> > queue;

    // int currentDepth = 0;
    // int depthIndex = 0;
    //
    // for (auto & p : prodVertex->GetParticlesOut()) {
    //   if (!p->IsStable()) {
    //     queue.push(p);
    //   }
    // }
    //
    // while (!queue.empty()) {
    //   //std::cout << queue.front()->GetName() << std::endl;
    //   //reactionEngine->ProcessParticle(queue.front(), prodVertex->GetPosition());
    //   event.AddVertex(reactionEngine->ProcessParticle(queue.front(), prodVertex->GetPosition()));
    //   queue.pop();
    //   // --depthIndex;
    //   // if (depthIndex < 0) {
    //   //   ++currentDepth;
    //   //   depthIndex = queue.size()-1;
    //   //   for (auto & p : event.GetLastVertex().GetParticlesOut()) {
    //   //     if (!p.IsStable()) {
    //   //       ++depthIndex;
    //   //     }
    //   //   }
    //   //   //depthIndex = event.GetLastVertex().GetParticlesOut().size()-1;
    //   // }
    //   for (auto & p : event.GetLastVertex().GetParticlesOut()) {
    //     if (!p->IsStable()) {
    //       queue.push(p);
    //     }
    //   }
    // }
    return event;
  }


  void Cradle::EventLoop(int nrEvents) {
    //Check why this is
    std::ios::sync_with_stdio(false);

    for (int i = 0; i < nrEvents; ++i) {
      Event event(eventCounter);
      eventCounter++;

      std::shared_ptr<PDS::core::Vertex> v = ConstructInitialVertex();
      event.AddVertex(v);
      event.AddVertices(UnlimitedDecay(v));

      events.push_back(event);
    }
  }

  void Cradle::EventLoopMT(int nrEvents, int nrThreads) {
    // int Q = (nrEvents-1)/nrThreads;
    // int R = (nrEvents-1)%nrThreads;

    // std::future<std::string> f[nrThreads];
    // for (int t = 0; t < nrThreads; t++) {
    //   if (t<R) f[t] = std::async(std::launch::async, &Cradle::GenerateEvents, this, Q+1);
    //   else f[t] = std::async(std::launch::async, &Cradle::GenerateEvents, this, Q);
    // }
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
