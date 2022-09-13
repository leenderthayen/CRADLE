#ifndef SPECTRUMGENERATOR
#define SPECTRUMGENERATOR

#include <vector>
#include <string>

#include "CRADLEConfig.h"

namespace CRADLE {

class Particle;

class SpectrumGenerator {
  public:
    virtual std::vector<std::vector<double> >* GenerateSpectrum(Particle*, Particle*, double) = 0;
    SpectrumGenerator();
    virtual ~SpectrumGenerator();
};

class DeltaSpectrumGenerator: public SpectrumGenerator {
  public:
    static DeltaSpectrumGenerator& GetInstance() {
      static DeltaSpectrumGenerator instance;
      return instance;
    }
    std::vector<std::vector<double> >* GenerateSpectrum(Particle*, Particle*, double);

  protected:
    DeltaSpectrumGenerator();
    DeltaSpectrumGenerator(DeltaSpectrumGenerator const& copy);
    DeltaSpectrumGenerator& operator=(DeltaSpectrumGenerator const& copy);
};

class SimpleBetaDecay: public SpectrumGenerator {
  public:
    static SimpleBetaDecay& GetInstance() {
      static SimpleBetaDecay instance;
      return instance;
    }
    std::vector<std::vector<double> >* GenerateSpectrum(Particle*, Particle*, double);

  protected:
    SimpleBetaDecay();
    SimpleBetaDecay(SimpleBetaDecay const& copy);
    SimpleBetaDecay& operator=(SimpleBetaDecay const& copy);
};

class AdvancedBetaDecay: public SpectrumGenerator {
  public:
    static AdvancedBetaDecay& GetInstance() {
      static AdvancedBetaDecay instance;
      return instance;
    }
    std::vector<std::vector<double> >* GenerateSpectrum(Particle*, Particle*, double);

  protected:
    AdvancedBetaDecay();
    AdvancedBetaDecay(AdvancedBetaDecay const& copy);
    AdvancedBetaDecay& operator=(AdvancedBetaDecay const& copy);
};

}//End of CRADLE namespace

#endif // End of SPECTRUMGENERATOR if
