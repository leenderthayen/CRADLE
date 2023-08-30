#ifndef PARTICLE
#define PARTICLE

#include <vector>
#include <boost/numeric/ublas/vector.hpp>
#include <string>
#include <sstream>
#include <random>

namespace CRADLE {

class DecayChannel;

namespace ublas = boost::numeric::ublas;

class Particle {
  private:
    double mass;
    int charge;
    int neutrons;
    double spin;
    double currentExcitationEnergy;

    std::string name;

    ublas::vector<double> fourMomentum;
    std::vector<DecayChannel*> decayChannels;

    static std::default_random_engine randomGen;
  public:
    Particle();
    Particle(const std::string&, double, int, int, double, double);
    Particle(const Particle&);
    Particle& operator=(const Particle& rhs);

    inline std::string ListInformation() const {
      std::ostringstream oss;
      oss << "-------------------------------------------------\n";
      oss << "  Particle: " << name << "\n";
      oss << "-------------------------------------------------\n";
      oss << "  Charge: " << charge << "\t\tNeutrons: " << neutrons << "\tSpin: " << spin << "\n";
      oss << "  Mass: " << mass << "\tG.S. Lifetime: " << GetLifetime() << "\n";
      return oss.str();
    }
    std::vector<Particle*> Decay();
    double GetDecayTime();
    inline std::string GetName() const {
      std::string n;
      std::ostringstream oss(n);
      oss << name << " (" << currentExcitationEnergy << " keV) Kin. En.: " << GetKinEnergy();
      return oss.str();
    };
    inline std::string GetRawName() const { return name; };
    inline std::string GetInfoForFile() const {
      std::string n;
      std::ostringstream oss(n);
      oss << name << "\t" << currentExcitationEnergy << "\t" << GetKinEnergy() << "\t" << fourMomentum(0) << "\t" << fourMomentum(1) << "\t" << fourMomentum(2) << "\t" << fourMomentum(3);
      return oss.str();
    };

    inline void SetMomentum(ublas::vector<double> v) { fourMomentum = v; };
    inline void SetExcitationEnergy(double e) { currentExcitationEnergy = e; fourMomentum(0)+=e; };
    double GetLifetime() const;
    inline int GetCharge() const { return charge; };
    inline int GetNeutrons() const { return neutrons; };
    inline double GetMass() const { return mass + currentExcitationEnergy; };
    inline ublas::vector<double> GetMomentum() const { return fourMomentum; };
    inline double GetExcitationEnergy() const { return currentExcitationEnergy; };
    inline void AddDecayChannel(DecayChannel* dc) { decayChannels.push_back(dc); };
    ublas::vector<double> GetVelocity() const;
    inline double GetKinEnergy() const { return fourMomentum(0)-GetMass(); };
    inline void SetKinEnergy(double e) { fourMomentum(0) = GetMass() + e; };
    inline ublas::vector<double> Get3Momentum() const {
      ublas::vector<double> v (3);
      v[0] = fourMomentum[1];
      v[1] = fourMomentum[2];
      v[2] = fourMomentum[3];
      return v;
    };
    inline std::vector<DecayChannel*>& GetDecayChannels() { return decayChannels; };
};

}//End of CRADLE namespace

#endif
