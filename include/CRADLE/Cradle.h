#ifndef CRADLE_DECAY_MANAGER_H
#define CRADLE_DECAY_MANAGER_H

#include "CRADLE/ConfigParser.h"

#include "spdlog/spdlog.h"

#include <vector>
#include <map>
#include <string>
#include <memory>

namespace CRADLE {

  class ReactionEngine;
  class Event;

  class Cradle {
  public:

    Cradle(std::string);
    ~Cradle();

    void Initialise(std::string, int argc = 0, const char** argv = nullptr);
    void Initialise(ConfigOptions);
    void Next();
    void GenerateEvents(int);

    //TODO vector.back() on empty container causes undefined behaviour
    inline Event GetLastEvent() { return events.back(); };

    void SetReactionEngine(std::shared_ptr<ReactionEngine>);

    inline ReactionEngine* GetReactionEngine() { return reactionEngine; }

  private:
    bool MainLoop(int, int);

    void InitialiseLoggers();

    PDS::core::DynamicParticle ConstructInitialParticle();

    void FlushEvents();

    Event BreadthFirstDecay(const PDS::core::DynamicParticle&, int maxDepth);
    Event DepthFirstDecay(const PDS::core::DynamicParticle&, int maxDepth);


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
  };

}//end of CRADLE namespace
#endif
