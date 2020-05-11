#ifndef CRADLE_REACTION_MODE_H
#define CRADLE_REACTION_MODE_H

#include "CRADLE/SpectrumGenerator.h"

#include "PDS/Core/DynamicParticle.h"
#include "PDS/Units/GlobalSystemOfUnits.h"

#include <vector>
#include <string>
#include <memory>

namespace CRADLE {

  typedef std::vector<PDS::core::DynamicParticle> (*Activator)(const PDS::core::DynamicParticle&,
    double, double, std::map<std::string, std::shared_ptr<SpectrumGenerator> >);
  typedef double (*BranchingCalculator)(const PDS::core::Particle&, double);

  struct Process {
    double staticBranchingRatio = -1;
    BranchingCalculator bc;
    Activator a;
    std::map<std::string, std::shared_ptr<SpectrumGenerator> > generators;
  };

  class ReactionMode {
  public:
    ReactionMode();
    ReactionMode(Process);
    ~ReactionMode();
    std::vector<PDS::core::DynamicParticle> Activate(const PDS::core::DynamicParticle&, double Q, double finalExcitationEnergy) const;

    inline void AddProcess(Process p) { processes.push_back(p); }
    inline const std::vector<Process>& GetProcesses() const { return processes; }

    bool RemoveProcessAtIndex(unsigned i);
  private:
    std::vector<Process> processes;
  };

  class ReactionModeFactory {
  public:
    static ReactionMode DefaultBetaMinus(bool advancedFermiFunction = false, double stepSize = 1.0 * keV);
    static ReactionMode DefaultBetaPlus(bool advancedFermiFunction = false, double stepSize = 1.0 * keV);
    static ReactionMode DefaultProtonSeparation();
    static ReactionMode DefaultNeutronSeparation();
    static ReactionMode DefaultAlpha();
    static ReactionMode DefaultGamma();
    static ReactionMode ExternalBSGBetaPlus(std::string configFile);
    static ReactionMode ExternalBSGBetaMinus(std::string configFile);
  };

}//end of CRADLE namespace
#endif
