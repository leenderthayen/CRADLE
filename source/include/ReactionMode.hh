#ifndef REACTIONMODE
#define REACTIONMODE

#include <vector>
#include <string>

namespace PDS {
  namespace core {
    class DynamicParticle;
  }
}

class SpectrumGenerator;

namespace ublas = boost::numeric::ublas;

class ReactionMode{
  public:
    virtual static std::vector<PDS::core::DynamicParticle*> activate(PDS::core::DynamicParticle*, double, double, SpectrumGenerator*) = 0;
    ReactionMode();
    virtual ~ReactionMode();


  protected:
};

class BetaMinus: public ReactionMode {
  public:
    static std::vector<PDS::core::DynamicParticle*> activate(PDS::core::DynamicParticle*, double, double);

  protected:
    BetaMinus();

};

class BetaPlus: public ReactionMode {
  public:
    static std::vector<PDS::core::DynamicParticle*> activate(PDS::core::DynamicParticle*, double, double);

  protected:
    BetaPlus();
};

class ConversionElectron: public ReactionMode {
    public:
    static std::vector<PDS::core::DynamicParticle*> activate(PDS::core::DynamicParticle*, double, double);

  protected:
    ConversionElectron();
};

class Proton: public ReactionMode {
    public:
    static std::vector<PDS::core::DynamicParticle*> activate(PDS::core::DynamicParticle*, double, double);

  protected:
    Proton();
};

class Alpha: public ReactionMode {
    public:
    static std::vector<PDS::core::DynamicParticle*> activate(PDS::core::DynamicParticle*, double, double);

  protected:
    Alpha();
};

class Gamma: public ReactionMode {
    public:
    static std::vector<PDS::core::DynamicParticle*> activate(PDS::core::DynamicParticle*, double, double);

  protected:
    Gamma();
};
#endif
