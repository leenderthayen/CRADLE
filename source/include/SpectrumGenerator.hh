#ifndef SPECTRUMGENERATOR
#define SPECTRUMGENERATOR

#include <vector>

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
    void WriteINIFile(const char*, Particle*, Particle*, double);
};
#endif
