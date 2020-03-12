#ifndef CRADLE_DECAY_MANAGER_H
#define CRADLE_DECAY_MANAGER_H

#include "CRADLE/ConfigParser.h"

#include "spdlog/spdlog.h"

#include <vector>
#include <map>
#include <string>

namespace CRADLE {

class ReactionEngine;

class Cradle {
  public:

    Cradle(std::string);
    ~Cradle();

    void Initialise(std::string, int argc = 0, const char** argv = nullptr);
    void Initialise(ConfigOptions);
    // bool Next();
    // bool MainLoop(int,int);

    void SetReactionEngine(ReactionEngine* );

    inline ReactionEngine* GetReactionEngine() { return reactionEngine; }

  private:
    //std::string GenerateEvents(int);

    void InitialiseLoggers();

    std::string outputName;
    std::string initStateName;
    double initExcitationEn;
    ReactionEngine* reactionEngine;
    ConfigOptions* configOptions;

    std::shared_ptr<spdlog::logger> consoleLogger;
    std::shared_ptr<spdlog::logger> debugFileLogger;
    std::shared_ptr<spdlog::logger> rawSpectrumLogger;
    std::shared_ptr<spdlog::logger> resultsFileLogger;
};

}//end of CRADLE namespace
#endif
