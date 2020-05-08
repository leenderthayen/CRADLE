#ifndef CRADLE_DECAY_MANAGER_H
#define CRADLE_DECAY_MANAGER_H

#include "CRADLE/ConfigParser.h"
#include "CRADLE/Event.h"

#include "PDS/Core/DynamicParticle.h"

#include "spdlog/spdlog.h"

#include <vector>
#include <map>
#include <string>
#include <memory>
#include <atomic>

namespace CRADLE {

  class ReactionEngine;

  class Cradle {
  public:

    Cradle(std::string);
    ~Cradle();

    void Initialise(std::string, int argc = 0, const char** argv = nullptr);
    void Initialise(ConfigOptions);
    void EventLoopMT(int, int);
    void EventLoop(int);

    void SetReactionEngine(std::shared_ptr<ReactionEngine>);

    inline std::shared_ptr<ReactionEngine> GetReactionEngine() const { return reactionEngine; }

    inline std::vector<Event> GetEvents() const { return events; }

    std::shared_ptr<PDS::core::DynamicParticle> ConstructInitialParticle();
    std::shared_ptr<PDS::core::Vertex> ConstructInitialVertex();

  private:
    void InitialiseLoggers();

    void FlushEvents();

    std::vector<std::shared_ptr<PDS::core::Vertex> > UnlimitedDecay(const std::shared_ptr<PDS::core::Vertex>& prodVertex);
    Event BreadthFirstDecay(const std::shared_ptr<PDS::core::Vertex>& prodVertex, int maxDepth);
    //Event DepthFirstDecay(const PDS::core::DynamicParticle&, int maxDepth);

    std::string outputName;
    std::string initStateName;
    double initExcitationEn;
    std::shared_ptr<ReactionEngine> reactionEngine;
    ConfigOptions configOptions;
    std::vector<Event> events;

    std::shared_ptr<spdlog::logger> consoleLogger;
    std::shared_ptr<spdlog::logger> debugFileLogger;
    std::shared_ptr<spdlog::logger> rawSpectrumLogger;
    std::shared_ptr<spdlog::logger> resultsFileLogger;

    std::atomic<unsigned long long> eventCounter = 1;
  };

}//end of CRADLE namespace
#endif
