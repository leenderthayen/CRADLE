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
    SpectrumGenerator();
    virtual ~SpectrumGenerator();

    std::vector<std::vector<double> >* GetSpectrum(PDS::core::Particle&, PDS::core::Particle&, double);

  private:
    virtual std::vector<std::vector<double> >* GenerateSpectrum(PDS::core::Particle&, PDS::core::Particle&, double) = 0;
    void RegisterDistribution(const std::string, std::vector<std::vector<double> >*);
    std::vector<std::vector<double> >* GetDistribution(const std::string);
    bool DistributionExists(const std::string);

    std::map<const std::string, std::vector<std::vector<double> >* > registeredDistributions;
};

class DeltaSpectrumGenerator: public SpectrumGenerator {
  public:
    static DeltaSpectrumGenerator& GetInstance() {
      static DeltaSpectrumGenerator instance;
      return instance;
    }

  protected:
    DeltaSpectrumGenerator();

    std::vector<std::vector<double> >* GenerateSpectrum(PDS::core::Particle&, PDS::core::Particle&, double);
};

class SimpleBetaDecay: public SpectrumGenerator {
  public:
    static SimpleBetaDecay& GetInstance() {
      static SimpleBetaDecay instance;
      return instance;
    }

  protected:
    SimpleBetaDecay();

    std::vector<std::vector<double> >* GenerateSpectrum(PDS::core::Particle&, PDS::core::Particle&, double);
};

//#ifdef USE_BSG
class ExternalBSG: public SpectrumGenerator {
  public:
    ExternalBSG(std::string);
    ~ExternalBSG() {};

    const BSG::Generator* GetGenerator() const;

  private:
    BSG::Generator* generator = nullptr;

    std::vector<std::vector<double> >* GenerateSpectrum(PDS::core::Particle&, PDS::core::Particle&, double);
};
//#endif // End of USE_BSG if

}//end of CRADLE namespace

#endif // End of SPECTRUMGENERATOR if
