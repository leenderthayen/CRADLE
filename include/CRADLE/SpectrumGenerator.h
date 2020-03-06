#ifndef CRADLE_SPECTRUM_GENERATOR_H
#define CRADLE_SPECTRUM_GENERATOR_H

#include "PDS/Core/DynamicParticle.h"

#include <vector>
#include <string>
#include <map>

namespace CRADLE {

class SpectrumGenerator {
  public:
    virtual std::vector<std::vector<double> >* GenerateSpectrum(PDS::core::DynamicParticle&, PDS::core::DynamicParticle&, double) = 0;
    SpectrumGenerator();
    virtual ~SpectrumGenerator();

    void RegisterDistribution(const std::string, std::vector<std::vector<double> >*);
    std::vector<std::vector<double> >* GetDistribution(const std::string);

  private:
    std::map<const std::string, std::vector<std::vector<double> >* > registeredDistributions;
};

class DeltaSpectrumGenerator: public SpectrumGenerator {
  public:
    static DeltaSpectrumGenerator& GetInstance() {
      static DeltaSpectrumGenerator instance;
      return instance;
    }
    std::vector<std::vector<double> >* GenerateSpectrum(PDS::core::DynamicParticle&, PDS::core::DynamicParticle&, double);

  protected:
    DeltaSpectrumGenerator();
};

class SimpleBetaDecay: public SpectrumGenerator {
  public:
    static SimpleBetaDecay& GetInstance() {
      static SimpleBetaDecay instance;
      return instance;
    }
    std::vector<std::vector<double> >* GenerateSpectrum(PDS::core::DynamicParticle&, PDS::core::DynamicParticle&, double);

  protected:
    SimpleBetaDecay();
};

// #ifdef USE_BSG
// class BSG: public SpectrumGenerator {
//   public:
//     static BSG& GetInstance() {
//       static BSG instance;
//       return instance;
//     }
//     std::vector<std::vector<double> >* GenerateSpectrum(PDS::core::DynamicParticle*, PDS::core::DynamicParticle*, double);
//
//   protected:
//     BSG();
//     BSG(BSG const& copy);
//     BSG& operator=(BSG const& copy);
// };
// #endif // End of USE_BSG if

}//end of CRADLE namespace

#endif // End of SPECTRUMGENERATOR if
