#ifndef CRADLE_REACTION_ENGINE_H
#define CRADLE_REACTION_ENGINE_H

#include <map>
#include <vector>
#include <random>

namespace PDS {
  namespace core {
    class DynamicParticle;
    enum class ReactionModeName;
  }
}

namespace CRADLE {

class SpectrumGenerator;
struct ConfigOptions;
struct CouplingConstants;
struct BetaDecay;


typedef std::vector<PDS::core::DynamicParticle> (*activator)(PDS::core::DynamicParticle&,
  double, double, SpectrumGenerator&,CouplingConstants,BetaDecay);
typedef std::map<PDS::core::ReactionModeName, activator> reaction_mode_map;
typedef std::map<PDS::core::ReactionModeName, SpectrumGenerator&> spectrum_generator_map;

class ReactionEngine {
  public:
    ReactionEngine();
    ~ReactionEngine();

    void RegisterBasicSpectrumGenerators();
    void RegisterBasicReactionModes();
    void RegisterSpectrumGenerator(PDS::core::ReactionModeName, SpectrumGenerator&);
    void RegisterReactionMode(PDS::core::ReactionModeName, activator);
    std::string GenerateEvent(int,std::string, double, ConfigOptions);

  private:
    static std::default_random_engine randomGen;
    reaction_mode_map registeredReactionModeMap;
    spectrum_generator_map registeredSpectrumGeneratorMap;

    std::vector<PDS::core::DynamicParticle > Decay(PDS::core::DynamicParticle, ConfigOptions);
    inline std::string GetInfoForFile(PDS::core::DynamicParticle) const;
    inline double GetDecayTime(double) const;
};

}//end of CRADLE namespace
#endif
