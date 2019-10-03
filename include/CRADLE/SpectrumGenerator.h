#ifndef SPECTRUMGENERATOR
#define SPECTRUMGENERATOR

#include <vector>
#include <string>

#include "CRADLEConfig.h"

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

#ifdef USE_BSG
class BSG: public SpectrumGenerator {
  public:
    static BSG& GetInstance() {
      static BSG instance;
      return instance;
    }
    std::vector<std::vector<double> >* GenerateSpectrum(Particle*, Particle*, double);

  protected:
    BSG();
    BSG(BSG const& copy);
    BSG& operator=(BSG const& copy);
    void WriteINIFile(const std::string, Particle*, Particle*, double);
};
#endif // End of USE_BSG if

#endif // End of SPECTRUMGENERATOR if
