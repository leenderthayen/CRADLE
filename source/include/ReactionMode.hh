#ifndef REACTIONMODE
#define REACTIONMODE

#include <vector>
#include <boost/numeric/ublas/vector.hpp>
#include <string>


class Particle;
class SpectrumGenerator;
class DynamicParticle;

namespace ublas = boost::numeric::ublas;

class ReactionMode{
  public:
    virtual static std::vector<DynamicParticle*> activate(DynamicParticle*, double, double) = 0;
    ReactionMode();
    virtual ~ReactionMode();

    void SetSpectrumGenerator(SpectrumGenerator*);

  protected:
    static void ThreeBodyDecay(ublas::vector<double>&, Particle*, Particle*, Particle*, ublas::vector<double>&, double);
    static void TwoBodyDecay(ublas::vector<double>&, Particle*, Particle*, double);
    SpectrumGenerator* spectrumGen;
};

class BetaMinus: public ReactionMode {
  public:
    static std::vector<DynamicParticle*> activate(DynamicParticle*, double, double);

  protected:
    BetaMinus();

};

class BetaPlus: public ReactionMode {
  public:
    static std::vector<DynamicParticle*> activate(DynamicParticle*, double, double);

  protected:
    BetaPlus();
};

class ConversionElectron: public ReactionMode {
    public:
    static std::vector<DynamicParticle*> activate(DynamicParticle*, double, double);

  protected:
    ConversionElectron();
};

class Proton: public ReactionMode {
    public:
    static std::vector<DynamicParticle*> activate(DynamicParticle*, double, double);

  protected:
    Proton();
};

class Alpha: public ReactionMode {
    public:
    static std::vector<DynamicParticle*> activate(DynamicParticle*, double, double);

  protected:
    Alpha();
};

class Gamma: public ReactionMode {
    public:
    static std::vector<DynamicParticle*> activate(DynamicParticle*, double, double);

  protected:
    Gamma();
};
#endif
