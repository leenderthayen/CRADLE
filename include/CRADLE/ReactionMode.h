#ifndef CRADLE_REACTION_MODE_H
#define CRADLE_REACTION_MODE_H

#include "CRADLE/SpectrumGenerator.h"

#include "PDS/Core/DynamicParticle.h"

#include <vector>
#include <string>

namespace CRADLE {

  typedef std::vector<PDS::core::DynamicParticle> (*Activator)(PDS::core::DynamicParticle&,
    double, double, std::map<std::string, SpectrumGenerator*>);
  typedef double (*BranchingCalculator)(PDS::core::Particle&, double);

  struct Process {
    double staticBranchingRatio = -1;
    BranchingCalculator bc;
    Activator a;
    std::map<std::string, SpectrumGenerator*> generators;
  };

  class ReactionMode {
  public:
    ReactionMode();
    ~ReactionMode() {};
    std::vector<PDS::core::DynamicParticle> Activate(PDS::core::DynamicParticle&, double Q, double finalExcitationEnergy) const;

    inline void AddProcess(Process p) { processes.push_back(p); }
    inline const std::vector<Process>& GetProcesses() const { return processes; }

    bool RemoveProcessAtIndex(unsigned i);
  private:
    std::vector<Process> processes;
    //std::vector<PDS::core::DynamicParticle> ActivateProcess(PDS::core::DynamicParticle&, double, double);
  };

  class ReactionModeFactory {
  public:
    static ReactionMode& DefaultBetaMinus();
    static ReactionMode& DefaultBetaPlus();
    static ReactionMode& DefaultProtonSeparation();
    static ReactionMode& DefaultNeutronSeparation();
    static ReactionMode& DefaultAlpha();
    static ReactionMode& DefaultGamma();
  };

}//end of CRADLE namespace
#endif
