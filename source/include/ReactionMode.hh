#ifndef REACTIONMODE
#define REACTIONMODE

#include <vector>
#include <string>

namespace PDS {
  namespace core {
    class DynamicParticle;
  }
}

struct CouplingConstants;
struct BetaDecay;
class SpectrumGenerator;

class ReactionMode{
  public:
    static std::vector<PDS::core::DynamicParticle> activate(PDS::core::DynamicParticle&, double, double, SpectrumGenerator&, CouplingConstants, BetaDecay);
    ReactionMode();
    virtual ~ReactionMode() = 0;


  protected:
};

class BetaMinus: public ReactionMode {
  public:
    static std::vector<PDS::core::DynamicParticle> activate(PDS::core::DynamicParticle&, double, double, SpectrumGenerator&, CouplingConstants, BetaDecay);

  protected:
    BetaMinus();

};

class BetaPlus: public ReactionMode {
  public:
    static std::vector<PDS::core::DynamicParticle> activate(PDS::core::DynamicParticle&, double, double, SpectrumGenerator&, CouplingConstants, BetaDecay);

  protected:
    BetaPlus();
};

class ConversionElectron: public ReactionMode {
    public:
    static std::vector<PDS::core::DynamicParticle> activate(PDS::core::DynamicParticle&, double, double, SpectrumGenerator&, CouplingConstants, BetaDecay);

  protected:
    ConversionElectron();
};

class Proton: public ReactionMode {
    public:
    static std::vector<PDS::core::DynamicParticle> activate(PDS::core::DynamicParticle&, double, double, SpectrumGenerator&, CouplingConstants, BetaDecay);

  protected:
    Proton();
};

class Alpha: public ReactionMode {
    public:
    static std::vector<PDS::core::DynamicParticle> activate(PDS::core::DynamicParticle&, double, double, SpectrumGenerator&, CouplingConstants, BetaDecay);

  protected:
    Alpha();
};

class Gamma: public ReactionMode {
    public:
    static std::vector<PDS::core::DynamicParticle> activate(PDS::core::DynamicParticle&, double, double, SpectrumGenerator&, CouplingConstants, BetaDecay);

  protected:
    Gamma();
};
#endif
