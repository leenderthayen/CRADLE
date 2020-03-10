#ifndef CRADLE_REACTION_MODE_H
#define CRADLE_REACTION_MODE_H

#include "PDS/Core/DynamicParticle.h"
#include "CRADLE/SpectrumGenerator.h"

#include <vector>
#include <string>

namespace CRADLE {

class ReactionMode{
  public:
    static std::vector<PDS::core::DynamicParticle> activate(PDS::core::DynamicParticle&, double Q, double finalExcitationEnergy);
    virtual ~ReactionMode() = 0;

  protected:
    static inline std::map<std::string, SpectrumGenerator*>*  generators = nullptr;
};

class BetaMinus: public ReactionMode {
  public:
    static std::vector<PDS::core::DynamicParticle> activate(PDS::core::DynamicParticle&, double, double);

  protected:
    BetaMinus();

};

class BetaPlus: public ReactionMode {
  public:
    static std::vector<PDS::core::DynamicParticle> activate(PDS::core::DynamicParticle&, double, double);

  protected:
    BetaPlus();
};

class ConversionElectron: public ReactionMode {
    public:
    static std::vector<PDS::core::DynamicParticle> activate(PDS::core::DynamicParticle&, double, double);

  protected:
    ConversionElectron();
};

class Proton: public ReactionMode {
    public:
    static std::vector<PDS::core::DynamicParticle> activate(PDS::core::DynamicParticle&, double, double);

  protected:
    Proton();
};

class Alpha: public ReactionMode {
    public:
    static std::vector<PDS::core::DynamicParticle> activate(PDS::core::DynamicParticle&, double, double);

  protected:
    Alpha();
};

class Gamma: public ReactionMode {
    public:
    static std::vector<PDS::core::DynamicParticle> activate(PDS::core::DynamicParticle&, double, double);

  protected:
    Gamma();
};

}//end of CRADLE namespace
#endif
