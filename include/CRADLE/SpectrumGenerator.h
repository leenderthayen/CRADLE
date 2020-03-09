#ifndef CRADLE_SPECTRUM_GENERATOR_H
#define CRADLE_SPECTRUM_GENERATOR_H

#include "PDS/Core/ParticleDefinition.h"

#include <vector>
#include <string>
#include <map>

//#ifdef USE_BSG
#include "BSG/Generator.h"
//#endif

namespace CRADLE {

class SpectrumGenerator {
  public:
    virtual std::vector<std::vector<double> >* GenerateSpectrum(PDS::core::Particle&, PDS::core::Particle&, double) = 0;
    SpectrumGenerator();
    virtual ~SpectrumGenerator();

  private:
    void RegisterDistribution(const std::string, std::vector<std::vector<double> >*);
    std::vector<std::vector<double> >* GetDistribution(const std::string);
    
    std::map<const std::string, std::vector<std::vector<double> >* > registeredDistributions;
};

class DeltaSpectrumGenerator: public SpectrumGenerator {
  public:
    static DeltaSpectrumGenerator& GetInstance() {
      static DeltaSpectrumGenerator instance;
      return instance;
    }
    std::vector<std::vector<double> >* GenerateSpectrum(PDS::core::Particle&, PDS::core::Particle&, double);

  protected:
    DeltaSpectrumGenerator();
};

class SimpleBetaDecay: public SpectrumGenerator {
  public:
    static SimpleBetaDecay& GetInstance() {
      static SimpleBetaDecay instance;
      return instance;
    }
    std::vector<std::vector<double> >* GenerateSpectrum(PDS::core::Particle&, PDS::core::Particle&, double);

  protected:
    SimpleBetaDecay();
};

//#ifdef USE_BSG
class ExternalBSG: public SpectrumGenerator {
  public:
    ExternalBSG(std::string);
    ~ExternalBSG() {};
    std::vector<std::vector<double> >* GenerateSpectrum(PDS::core::Particle&, PDS::core::Particle&, double);

    const BSG::Generator* GetGenerator() const;

  private:
    BSG::Generator* generator = nullptr;
};
//#endif // End of USE_BSG if

}//end of CRADLE namespace

#endif // End of SPECTRUMGENERATOR if
