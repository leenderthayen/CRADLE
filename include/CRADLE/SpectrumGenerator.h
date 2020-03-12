#ifndef CRADLE_SPECTRUM_GENERATOR_H
#define CRADLE_SPECTRUM_GENERATOR_H

#include "PDS/Core/ParticleDefinition.h"
#include "PDS/Units/GlobalSystemOfUnits.h"

#include "BSG/Generator.h"

#include <vector>
#include <string>
#include <map>

namespace CRADLE {

  class SpectrumGenerator {
  public:
    SpectrumGenerator();
    virtual ~SpectrumGenerator();

    const std::vector<std::vector<double> >* GetSpectrum(PDS::core::Particle&, PDS::core::Particle&, double);

  private:
    void RegisterDistribution(const std::string, std::vector<std::vector<double> >*);
    std::vector<std::vector<double> >* GetDistribution(const std::string);
    bool DistributionExists(const std::string);

    virtual std::vector<std::vector<double> >* GenerateSpectrum(PDS::core::Particle&, PDS::core::Particle&, double) = 0;

    std::map<const std::string, std::vector<std::vector<double> >* > registeredDistributions;
  };

  class DeltaSpectrumGenerator: public SpectrumGenerator {
  private:
    std::vector<std::vector<double> >* GenerateSpectrum(PDS::core::Particle&, PDS::core::Particle&, double);
  };

  class BasicBetaSpectrumGenerator: public SpectrumGenerator {
  public:
    BasicBetaSpectrumGenerator(bool a = false, double ss = 1.0 * keV);
  private:
    std::vector<std::vector<double> >* GenerateSpectrum(PDS::core::Particle&, PDS::core::Particle&, double);

    bool advancedFermiFunction;
    double stepSize;
  };

  class ExternalBSG: public SpectrumGenerator {
  public:
    ExternalBSG(std::string);
    ~ExternalBSG();

    const BSG::Generator* GetGenerator() const;

  private:
    BSG::Generator* generator = nullptr;

    std::vector<std::vector<double> >* GenerateSpectrum(PDS::core::Particle&, PDS::core::Particle&, double);
  };

}//end of CRADLE namespace

#endif // End of SPECTRUMGENERATOR if
