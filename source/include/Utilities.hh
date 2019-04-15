#ifndef UTILITIES
#define UTILITIES

#include <math.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <map>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/symmetric.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/math/special_functions/gamma.hpp>
#include <boost/math/special_functions/legendre.hpp>
#include <complex>
#include "gsl/gsl_sf_gamma.h"
#include "gsl/gsl_sf_result.h"
#include "gsl/gsl_complex_math.h"

namespace utilities {
  using namespace boost::numeric::ublas;

  const double PI = 3.14159265359;
  const double C = 299792458;//m/s
  const double EMASSC2 = 510.9989461;//keV
  const double PMASSC2 = 938272.046;//keV
  const double NMASSC2 = 939565.4133;//keV
  const double ALPHAMASSC2 = 3727379.508;//keV
  const double FINESTRUCTURE = 0.0072973525664;
  const double E = 2.718281828459045;
  const double HBAR = 6.58211889e-16;//ev*s
  const double a_CORR = -1.0;

  const std::string atoms[] = {"H", "He", "Li", "Be", "B", "C", "N", "O", "F", "Ne", "Na", "Mg", "Al", "Si", "P", "S", "Cl", "Ar", "K", "Ca", "Sc", "Ti", "V", "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn", "Ga", "Ge", "As", "Se", "Br", "Kr", "Rb", "Sr", "Y", "Zr", "Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag", "Cd", "In", "Sn", "Sb", "Te", "I", "Xe", "Cs", "Ba", "La", "Ce", "Pr", "Nd", "Pm", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb", "Lu", "Hf", "Ta", "W", "Re", "Os", "Ir", "Pt", "Au", "Hg", "Tl", "Pb", "Bi", "Po", "At", "Rn", "Fr", "Ra", "Ac", "Th", "Pa", "U", "Np", "Pu", "Am", "Cm", "Bk", "Cf", "Es", "Fm", "Md", "No", "Lr", "Rf", "Db", "Sg", "Bh", "Hs", "Mt"};

  inline vector<double> RandomDirection () {
    double z, phi;
    vector<double> v(3);

    z = rand() / (double)RAND_MAX * 2. - 1.;
    phi = rand() / (double)RAND_MAX * 2. * PI;

    v(0) = std::sqrt(1.-z*z)*std::cos(phi);
    v(1) = std::sqrt(1.-z*z)*std::sin(phi);
    v(2) = z;

    return v;
  };

  inline double Random(double begin, double end) { return rand() / (double)RAND_MAX * (end - begin) + begin; };

  inline double RandomFromDistribution(std::vector<std::vector<double> >& pd) {
    double begin = pd[0][0];
    double end = pd[pd.size()-1][0];
    double stepSize = pd[1][0] - pd[0][0];
    double max = 0.;

    for (std::vector<std::vector<double> >::size_type i = 0; i != pd.size(); i++) {
      if (pd[i][1] > max) {
        max = pd[i][1];
      }
    }

    double r = 0.;
    double q = 0.;
    do {
      r = rand() / (double)RAND_MAX;
      q = rand() / (double)RAND_MAX * max;
    } while (q > pd[r*pd.size()][1]);
    return r*(end - begin) + begin;
  }

  inline double GetSpeed(double energy, double massc2) {
    double gamma = energy/massc2 + 1.;
    return std::sqrt(1.-1./(gamma*gamma))*C;
  }

  inline double GetNorm(const vector<double>& v) {
    double sum = 0.;
    for (vector<double>::size_type i = 0; i != v.size(); i++) {
      sum+=v[i]*v[i];
    }
    return std::sqrt(sum);
  }

  inline double FourDimDot(vector<double>& a, vector<double>& b) {
    //Mostly minus convention
    return a[0]*b[0]-(a[1]*b[1]+a[2]*b[2]+a[3]*b[3]);
  }

  inline double CalculateXiBetaDecay(double& cs, double& ct, double& cv, double& ca, double& mf, double& mgt) {
    return mf*mf*(2.*cs*cs+2.*cv*cv)+mgt*mgt*(2.*ct*ct+2.*ca*ca);
  }

  inline double CalculateBetaNeutrinoAsymmetry(double cs, double ct, double cv, double ca, double mf, double mgt) {
    return (mf*mf*(-2.*cs*cs+2.*cv*cv)+mgt*mgt/3.*(2.*ct*ct-2.*ca*ca))/CalculateXiBetaDecay(cs, ct, cv, ca, mf, mgt);
  }

  inline vector<double> NormaliseVector(const vector<double>& v) {
    vector<double> newV (v);
    double norm = GetNorm(v);
    if (norm != 0) {
      return newV/norm;
    }
    return v;
  }

  inline double LambdaKinematic(double x, double y, double z) {
    return x*x+y*y+z*z-2*x*y-2*y*z-2*x*z;
  }

  inline double PhaseSpace(double W, double W0) {
    return std::sqrt(W*W-1.)*W*std::pow(W0-W, 2.);
  }

  inline double SimpleFermiFunction(double Z, double v) {
    double nu = Z*FINESTRUCTURE/v*C;
    return 2.*PI*nu/(1.-std::pow(E, -2.*PI*nu));
  }

  inline double FermiFunction(double Z, double W, double R) {
    double gamma = std::sqrt(1.-(FINESTRUCTURE*Z)*(FINESTRUCTURE*Z));
    double p = std::sqrt(W*W-1);
    double first = 2*(gamma+1);
    //the second term will be incorporated in the fifth
    //double second = 1/pow(gsl_sf_gamma(2*gamma+1),2);
    double third = std::pow(2*p*R,2*(gamma-1));
    double fourth = std::exp(M_PI*FINESTRUCTURE*Z*W/p);

    //the fifth is a bit tricky
    //we use the complex gamma function from GSL
    gsl_sf_result magn;
    gsl_sf_result phase;
    gsl_sf_lngamma_complex_e(gamma, FINESTRUCTURE*Z*W/p,&magn,&phase);
    //now we have what we want in magn.val

    //but we incorporate the second term here as well
    double fifth = std::exp(2*(magn.val-gsl_sf_lngamma(2*gamma+1)));

    return first*third*fourth*fifth;
  }
  
  inline double ApproximateRadius(double A) {
    return (1.15+1.8*std::pow(A, -2./3.)-1.2*std::pow(A, -4./3.))*EMASSC2*1000./HBAR/C*std::pow(A, 1./3.);
  }

  inline double GetSpectrumHeight(double Z, double A, double Q, double E, bool advancedFermi) {
    double W = E/EMASSC2+1.;
    double W0 = Q/EMASSC2+1.;
    double R = ApproximateRadius(A);
    if (advancedFermi) { 
      return PhaseSpace(W, W0)*FermiFunction(Z, W, R);
    }
    else {
      return PhaseSpace(W, W0)*SimpleFermiFunction(Z, GetSpeed(E, EMASSC2));
    }
  }

  inline std::vector<std::vector<double> >* GenerateBetaSpectrum(double Z, double A, double Q, bool advancedFermi) {
    std::vector<std::vector<double> >* dist = new std::vector<std::vector<double> >();
    double stepSize = 1.0;

    double currentEnergy = stepSize;
    while(currentEnergy <= Q) {
      double s = GetSpectrumHeight(Z, A, Q, currentEnergy, advancedFermi);
      std::vector<double> pair;
      pair.push_back(currentEnergy);
      pair.push_back(s);
      dist->push_back(pair);
      currentEnergy+=stepSize;
    }
    return dist;
  }

  inline vector<double> CrossProduct(vector<double> first, vector<double> second) {
    vector<double> v (3);
    
    v(0) = first(1)*second(2) - first(2)*second(1);
    v(1) = first(2)*second(0) - first(0)*second(2);
    v(2) = first(0)*second(1) - first(1)*second(0);
    
    return v;
  }

  inline vector<double> RotateAroundVector(vector<double>& initial, vector<double>& axis, double angle) {
    vector<double> vect(3);
    double u = axis[0];
    double v = axis[1];
    double w = axis[2];
    double x = initial[0];
    double y = initial[1];
    double z = initial[2];

    double c = std::cos(angle);
    double s = std::sin(angle);

    vect(0) = u*(u*x+v*y+w*z)*(1.-c)+x*c+(-w*y+v*z)*s;
    vect(1) = v*(u*x+v*y+w*z)*(1.-c)+y*c+(w*x-u*z)*s;
    vect(2) = w*(u*x+v*y+w*z)*(1.-c)+z*c+(-v*x+u*y)*s;

    return vect;
  }

  inline vector<double> GetParticleDirection(vector<double>& dir2, std::vector<double>& A) {
    dir2 = NormaliseVector(dir2);

    std::vector<std::vector<double> > dist;
    double stepSize = PI/180;
    double currentAngle = 0.;

    while (currentAngle <= PI) {
      std::vector<double> pair;
      pair.push_back(currentAngle);
      double W = 0.;
      for (std::vector<double>::size_type i = 0; i != A.size(); i++) {
        W+=A[i]*boost::math::legendre_p(i, std::cos(currentAngle));
      }
      pair.push_back(W);
      dist.push_back(pair);
      currentAngle+=stepSize;
    }

    double theta = 1.-RandomFromDistribution(dist)/PI;
    theta = std::acos(2*theta-1.);
    //std::cout << theta;
    vector<double> perp = CrossProduct(dir2, RandomDirection());
    vector<double> dir = RotateAroundVector(dir2, perp, theta);
    return dir;
  }

  inline vector<double> GetParticleDirection(std::vector<vector<double> >& dirs, std::vector<std::vector<double> >& A) {
    vector<double> dir (3);
  }

  inline vector<double> LorentzBoost(vector<double>& velocity, vector<double>& v) {
    double speed = GetNorm(velocity);
    double beta = speed;
    double gamma = 1./std::sqrt(1.-std::pow(speed, 2.));

    vector<double> dir = NormaliseVector(velocity);

    symmetric_matrix<double, upper> boost(4,4);
    boost(0, 0) = gamma;
    boost(0, 1) = -gamma*beta*dir[0];
    boost(0, 2) = -gamma*beta*dir[1];
    boost(0, 3) = -gamma*beta*dir[2];
    boost(1, 1) = 1.+(gamma-1.)*dir[0]*dir[0];
    boost(1, 2) = (gamma-1.)*dir[0]*dir[1];
    boost(1, 3) = (gamma-1.)*dir[0]*dir[2];
    boost(2, 2) = 1.+(gamma-1.)*dir[1]*dir[1];
    boost(2, 3) = (gamma-1.)*dir[1]*dir[2];
    boost(3, 3) = 1.+(gamma-1.)*dir[2]*dir[2];

    return prod(boost, v);
  }

  inline double GetApproximateMass(int Z, int A) {
    double b = 15.5*A-16.8*std::pow(A, 2.0/3.0)- 0.72*Z*(Z-1)*std::pow(A, -1.0/3.0) - 23.*std::pow(A-2*Z, 2.)/A;
    if (A%2 == 0) {
      double asym = 34.*std::pow(A, -3.0/4.0);
      if (Z%2 == 0) {
        b+=asym;
      }
      else {
        b-=asym;
      }
    }
    return Z*PMASSC2+(A-Z)*NMASSC2-b;
  }

  inline std::vector<std::vector<double> > ReadDistribution(const char * filename) {
    std::ifstream distReader(filename, std::ifstream::in);

    if (!distReader.is_open()) {
      std::cout << "Error: Could not find file " << filename << std::endl;
    }
    std::vector<std::vector<double> > dist;

    double x, y;
    while (distReader >> x >> y) {
      std::vector<double> pair;
      pair.push_back(x);
      pair.push_back(y);
      dist.push_back(pair);
    }
   return dist;
  }
}
#endif
