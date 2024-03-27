#ifndef UTILITIES
#define UTILITIES

#include <math.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
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
#include "gsl/gsl_sf_dilog.h"

#include "CRADLE/Screening.hh"
#include "CRADLE/DecayManager.hh"
#include "CRADLE/ConfigParser.hh"
#include "CRADLE/DecayMode.hh"
#include "CRADLE/Particle.hh"


namespace CRADLE {

namespace utilities {
  using namespace boost::numeric::ublas;

  const double PI = 3.14159265359;
  const double C = 299792458;//m/s
  const double EMASSC2 = 510.9989461;//keV
  const double PMASSC2 = 938272.046;//keV
  const double NMASSC2 = 939565.4133;//keV
  const double UMASSC2 = 931494.10242;//keV
  const double ALPHAMASSC2 = 3727379.4066;//keV
  const double FINESTRUCTUREMASSC2 = 3727379.508;//keV
  const double FINESTRUCTURE = 0.0072973525664;
  const double E = 2.718281828459045;
  const double HBAR = 6.58211889e-16;//ev*s
  const double NATURALLENGTH = HBAR*C/EMASSC2/1000.;//m
  const double a_CORR = -1.0;
  const double EULER_MASCHERONI_CONSTANT = 0.577215664901532;  /**< the Euler-Mascheroni constant */

  enum DecayType { FERMI, GAMOW_TELLER, MIXED };

  const std::string atoms[] = {"H", "He", "Li", "Be", "B", "C", "N", "O", "F", "Ne", "Na", "Mg", "Al", "Si", "P", "S", "Cl", "Ar", "K", "Ca", "Sc", "Ti", "V", "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn", "Ga", "Ge", "As", "Se", "Br", "Kr", "Rb", "Sr", "Y", "Zr", "Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag", "Cd", "In", "Sn", "Sb", "Te", "I", "Xe", "Cs", "Ba", "La", "Ce", "Pr", "Nd", "Pm", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb", "Lu", "Hf", "Ta", "W", "Re", "Os", "Ir", "Pt", "Au", "Hg", "Tl", "Pb", "Bi", "Po", "At", "Rn", "Fr", "Ra", "Ac", "Th", "Pa", "U", "Np", "Pu", "Am", "Cm", "Bk", "Cf", "Es", "Fm", "Md", "No", "Lr", "Rf", "Db", "Sg", "Bh", "Hs", "Mt"};


   /////// ajout de SL 12/05/2023///////////////////////
  inline double GetJpi(int A, int Z, double levelEn)
  {

    DecayManager& dm = DecayManager::GetInstance();
    std::string line;
    std::ostringstream gammaFileSS;
    gammaFileSS << dm.configOptions.envOptions.Gammadata;
    gammaFileSS << "/z" << Z << ".a" << A;
    std::ifstream gammaDataFile(gammaFileSS.str().c_str());

    if (gammaDataFile.is_open()) {
      while (getline(gammaDataFile, line)) {
        int levelNr;
        double initEnergy;
        double lifetime;
        double angMom;
        std::string flag;

        int nGammas;

        std::istringstream iss(line);
        iss >> levelNr >> flag >> initEnergy >> lifetime >> angMom >> nGammas;
        for (int i = 0; i < nGammas; ++i) {
          getline(gammaDataFile, line);
          continue;
      }
        
        if (initEnergy >= levelEn-1. && initEnergy <= levelEn+1.)
          {
            return angMom;
          }

      }
    }
    else
    {
      std::cerr << "Erreur lors de l'ouverture du fichier " << gammaFileSS.str() << std::endl;
    }
  }


  inline std::string FindBetaType(Particle* initState, Particle* finalState)
  {
    DecayManager& dm = DecayManager::GetInstance();
    std::string Type = "None";

    int Z_init = initState->GetCharge();
    int Z_final = finalState->GetCharge();

    double Jpi_init = GetJpi(Z_init + initState->GetNeutrons(), Z_init, initState->GetExcitationEnergy());
    double Jpi_final = GetJpi(Z_final + finalState->GetNeutrons(), Z_final, finalState->GetExcitationEnergy());

    // std::cout<<"INITIAL :\t "<<"Jpi = "<<Jpi_init<<"  "<<"Energy = "<< initState->GetExcitationEnergy()<<std::endl;
    // std::cout<<"FINAL :\t\t "<<"Jpi = "<<Jpi_final<<"  "<<"Energy = "<< finalState->GetExcitationEnergy()<<std::endl;

    if (Jpi_final == 0. && Jpi_init == 0.)/// J check
    {
      Type = "Fermi";
      return Type;
    }

    else
    {
      Type = "Gamow-Teller";
      return Type;
    }

  }
//////////////////////////////////////////////////////////////////////////////////////////////////////

  inline vector<double> RandomDirection () {
    double z, phi;
    vector<double> v(3);

    z = rand() / (double)RAND_MAX * 2. - 1.;
    phi = rand() / (double)RAND_MAX * 2. * PI;

    v(0) = std::sqrt(1.-z*z)*std::cos(phi);
    v(1) = std::sqrt(1.-z*z)*std::sin(phi);
    v(2) = z;

    return v;
  }

  inline double Random(double begin, double end) { return rand() / (double)RAND_MAX * (end - begin) + begin; }

  inline double RandomFromDistribution(std::vector<std::vector<double> >& pd) {
    double begin = pd[0][0];
    double end = pd[pd.size()-1][0];
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

  inline double CalculateXiBetaDecay(double cs, double csp, double ct, double ctp, double cv, double cvp, double ca, double cap, double mf, double mgt) {
    return mf*mf*(cs*cs+cv*cv+csp*csp+cvp*cvp)+mgt*mgt*(ct*ct+ctp*ctp+ca*ca+cap*cap);
  }

  inline double CalculateFierz(double cs, double csp, double ct, double ctp, double cv, double cvp, double ca, double cap, double mf, double mgt, double a, double b)
  {
    if (std::isnan(a) && std::isnan(b)){
    return 2.*(mf*mf*(cs*cv+csp*cvp)+mgt*mgt*(ct*ca+ctp*cap))/CalculateXiBetaDecay(cs, csp, ct, ctp, cv, cvp, ca, cap, mf, mgt);
  }
    else {
      return b;
    }
  }

  inline double CalculateBetaNeutrinoAsymmetry(double cs, double csp, double ct, double ctp, double cv, double cvp, double ca, double cap, double mf, double mgt, double a, double b)
  {
    if (std::isnan(a) && std::isnan(b)){
    return (mf*mf*(-cs*cs-csp*csp+cv*cv+cvp*cvp)+mgt*mgt/3.*(ct*ct+ctp*ctp-ca*ca-cap*cap))/CalculateXiBetaDecay(cs, csp, ct, ctp, cv, cvp, ca, cap, mf, mgt);
  }
    else {
      return a;
    }
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

  inline double SimpleFermiFunction(int Z, double v) {
    double nu = Z*FINESTRUCTURE/v*C;
    return 2.*PI*nu/(1.-std::pow(E, -2.*PI*nu));
  }

  inline double FermiFunction(int Z, double W, double R, int betaType) {
    double gamma = std::sqrt(1. - std::pow(FINESTRUCTURE * Z, 2.));
    double p = std::sqrt(W * W - 1.);
    double first = 2. * (gamma + 1.);
    // the second term will be incorporated in the fifth
    // double second = 1/std::pow(gsl_sf_gamma(2*gamma+1),2);
    double third = std::pow(2. * p * R, 2. * (gamma - 1.));
    double fourth = std::exp(betaType * M_PI * FINESTRUCTURE * Z * W / p);

    // the fifth is a bit tricky
    // we use the complex gamma function from GSL
    gsl_sf_result magn;
    gsl_sf_result phase;
    gsl_sf_lngamma_complex_e(gamma, betaType * FINESTRUCTURE * Z * W / p, &magn, &phase);
    // now we have what we wAt in magn.val

    // but we incorporate the second term here as well
    double fifth = std::exp(2. * (magn.val - gsl_sf_lngamma(2. * gamma + 1.)));

    double result = first * third * fourth * fifth;
    return result;
  }

  inline double L0Correction(double W, int Z, double r, int betaType) {     /////////// L0 Correction ---- L0 (voir specific, ajouter le sum)
                                       //double aPos[], double aNeg[]) {
    double gamma = std::sqrt(1. - std::pow(FINESTRUCTURE * Z, 2.));
    double sum = 0;
    double common = 0;
    double specific = 0;

    /////////////////////// Completion du code SL 10/05/2023
    double bNeg[7][6] = { {0.115, -1.8123, 8.2498, -11.223, -14.854, 32.086},
                          {-0.00062, 0.007165, 0.01841, -0.53736, 1.2691, -1.5467},
                          {0.02482, -0.5975, 4.84199, -15.3374, 23.9774, -12.6534},
                          {-0.14038, 3.64953, -38.8143, 172.1368, -346.708, 288.7873},
                          {0.008152, -1.15664, 49.9663, -273.711, 657.8292, -603.7033},
                          {1.2145, -23.9931, 149.9718, -471.2985, 662.1909, -305.6804},
                          {-1.5632, 33.4192, -255.1333, 938.5297, -1641.2845, 1095.358} };
    double aNeg[7] = {0,0,0,0,0,0,0};


    double bPos[7][6] = { {0.0701, -2.572, 27.5971, -128.658, 272.264, -214.925},
                          {-0.002308, 0.066463, -0.6407, 2.63606, -5.6317, 4.0011},
                          {0.07936, -2.09284, 18.45462, -80.9375, 160.8384, -124.8927},
                          {-0.93832, 22.02513, -197.00221, 807.1878, -1566.6077, 1156.3287},
                          {4.276181, -96.82411, 835.26505, -3355.8441, 6411.3255, -4681.573},
                          {-8.2135, 179.0862, -1492.1295, 5872.5362, -11038.7299, 7963.4701},
                          {5.4583, -115.8922, 940.8305, -3633.9181, 6727.6296, -4795.0481} };
    double aPos[7] = {0,0,0,0,0,0,0};

    for (int n=0; n<=6; n++)
    {
      for (int x=1; x<=6; x++)
      {
        if (betaType == 1)
          aPos[n] += bPos[n][x-1]*std::pow(FINESTRUCTURE*Z, x);
        else
          aNeg[n] += bNeg[n][x-1]*std::pow(FINESTRUCTURE*Z, x);
      }
    }


    for (int i = 1; i < 7; i++) {
      if (betaType == 1)
        sum += aPos[i] * std::pow(W * r, i - 1);
      else
        sum += aNeg[i] * std::pow(W * r, i - 1);
    }

    if (betaType == 1)
      specific = aPos[0] * r / W + 0.22 * (r - 0.0164) * std::pow(FINESTRUCTURE * Z, 4.5) + sum;
    else
      specific = aNeg[0] * r / W + 0.41 * (r - 0.0164) * std::pow(FINESTRUCTURE * Z, 4.5) + sum;
    ///////////////////////

    common = (1. + 13. / 60. * std::pow(FINESTRUCTURE * Z, 2) -
             betaType * W * r * FINESTRUCTURE * Z * (41. - 26. * gamma) / 15. / (2. * gamma - 1)) -
             betaType * FINESTRUCTURE * Z * r * gamma * (17. - 2. * gamma) / 30. / W /
                 (2. * gamma - 1);


    return (common + specific) * 2. / (1. + gamma);
  }

  inline double UCorrection(double W, int Z, int betaType) {        //////////////U Correction ----- U (OK)
    double result = 1.;
    double a0 = -5.6E-5 - betaType * 4.94E-5 * Z + 6.23E-8 * std::pow(Z, 2);
    double a1 = 5.17E-6 + betaType * 2.517E-6 * Z + 2.00E-8 * std::pow(Z, 2);
    double a2 = -9.17e-8 + betaType * 5.53E-9 * Z + 1.25E-10 * std::pow(Z, 2);

    double p = std::sqrt(W * W - 1);

    result = 1. + a0 + a1 * p + a2 * p * p;

    return result;
  }

  inline std::tuple<double, double> CCorrectionComponents(            ////////Shape Factor ----- C (OK)
      double W, double W0, int Z, int A, double R, int betaType, int decayType,
      double gA, double gP, double fc1, double fb, double fd,
      double ratioM121) {
    double AC0, AC1, ACm1, AC2;
    double VC0, VC1, VCm1, VC2;

    //Uniformly charged sphere results
    double F1111 = 27./35.;
    double F1221 = 57./70.;
    double F1222 = 233./210.;
    double F1211 = -3./70.;

    VC0 = -std::pow(W0 * R, 2.) / 5. -
          betaType * 2. / 9. * FINESTRUCTURE * Z * W0 * R * F1111 -
          std::pow(FINESTRUCTURE * Z, 2.) / 3. * F1222;

    VC1 = 4. / 15. * W0 * R * R -
          betaType * 2. / 3. * FINESTRUCTURE * Z * R * (F1221 - F1111 / 3.);

    VCm1 = 2. / 15. * W0 * R * R - betaType * FINESTRUCTURE * Z * R / 3. * F1211;

    VC2 = -4. / 15. * R * R;

    AC0 = -1. / 3. * std::pow(FINESTRUCTURE * Z, 2.) * F1222 -
          1. / 5. * (W0 * W0 - 1.) * R * R +
          betaType * 2. / 27. * FINESTRUCTURE * Z * W0 * R * F1111 + 11. / 45. * R * R;

    AC1 = 4. / 9. * W0 * R * R -
          betaType * 2. / 3. * FINESTRUCTURE * Z * R * (1. / 9. * F1111 + F1221);

    ACm1 = -2. / 45. * W0 * R * R + betaType * FINESTRUCTURE * Z * R / 3. * F1211;

    AC2 = -4. / 9. * R * R;

    double cShape = 0.;

    if (decayType == FERMI) {
      cShape = 1. + VC0 + VC1 * W + VCm1 / W + VC2 * W * W;
    } else if (decayType == GAMOW_TELLER) {
      cShape = 1. + AC0 + AC1 * W + ACm1 / W + AC2 * W * W;
    }

    double cNS = 0;

    if (decayType == GAMOW_TELLER) {
      double M = A * NMASSC2 / EMASSC2;

      double Lambda = std::sqrt(2.)/3.*10.*ratioM121;

      double phi = gP/gA/std::pow(2.*M*R/A, 2);

      double NSC0 = -1. / 45. * R * R * Lambda +
                    1. / 3. * W0 / M / fc1 * (-betaType * 2. * fb + fd) +
                    betaType * 2. / 5. * FINESTRUCTURE * Z / M / R / fc1 *
                    (betaType * 2. * fb + fd) -
                    betaType * 2. / 35. * FINESTRUCTURE * Z * W0 * R * Lambda;

      double NSC1 = betaType * 4. / 3. * fb / M / fc1 -
                    2. / 45. * W0 * R * R * Lambda +
                    betaType * FINESTRUCTURE * Z * R * 2. / 35. * Lambda;

      double NSCm1 = -1. / 3. / M / fc1 * (betaType * 2. * fb + fd) +
                     2. / 45. * W0 * R * R * Lambda;

      double NSC2 = 2. / 45. * R * R * Lambda;

      double gamma = std::sqrt(1.-std::pow(FINESTRUCTURE*Z, 2));

      double P0 = betaType*2./25.*FINESTRUCTURE*Z*R*W0 + 51./250.*std::pow(FINESTRUCTURE*Z, 2);
      double P1 = betaType*2./25.*FINESTRUCTURE*Z*R;
      double Pm1 = -2./3.*gamma*W0*R*R+betaType*26./25.*FINESTRUCTURE*Z*R*gamma;

      cNS = NSC0 + NSC1 * W + NSCm1 / W + NSC2 * W * W;

       cNS += phi*(P0 + P1 * W + Pm1 / W);
    }

    return std::make_tuple(cShape, cNS);
  }

  inline double QCorrection(double W, double W0, int Z, int A, ////////// Q Correction ---- Q (changement de la récupération de la valeur de a)
                                      int betaType, int decayType) {

    DecayManager& dm = DecayManager::GetInstance();
    double CS = dm.configOptions.couplingConstants.CS;
    double CSP = dm.configOptions.couplingConstants.CSP;
    double CV = dm.configOptions.couplingConstants.CV;
    double CVP = dm.configOptions.couplingConstants.CVP;
    double CA = dm.configOptions.couplingConstants.CA;
    double CAP = dm.configOptions.couplingConstants.CAP;
    double CT = dm.configOptions.couplingConstants.CT;
    double CTP = dm.configOptions.couplingConstants.CTP;
    double a_conf = dm.configOptions.couplingConstants.a;
    double b_conf = dm.configOptions.couplingConstants.b;

    double mf = 0.;
    double mgt = 0.;
    if (decayType == FERMI) {
      mf = 1.;
    }
    else {
      mgt = 1.;
    }

    double a = utilities::CalculateBetaNeutrinoAsymmetry(CS, CSP, CT, CTP, CV, CVP, CA, CAP, mf, mgt, a_conf, b_conf);

    double M = A * (PMASSC2 + NMASSC2) / 2. / EMASSC2;

    double p = std::sqrt(W * W - 1.);

    return 1. - betaType * M_PI * FINESTRUCTURE * Z / M / p * (1. + a * (W0 - W) / 3. / M);
  }

  inline double Spence(double x) { return -gsl_sf_dilog(x); }

  inline double RadiativeCorrection(double W, double W0, int Z,     ////// Radiative Correction ------ R (OK)
                                              double R, double gA,
                                              double gM) {
    // 1st order, based on the 5th Wilkinson article
    double beta = std::sqrt(1.0 - 1.0 / W / W);

    double g = 3. * std::log(PMASSC2 / EMASSC2) - 0.75 +
               4. * (std::atanh(beta) / beta - 1.) *
                   ((W0 - W) / 3. / W - 1.5 + std::log(2 * (W0 - W)));
    g += 4.0 / beta * Spence(2. * beta / (1. + beta)) +
         std::atanh(beta) / beta *
             (2. * (1. + beta * beta) + (W0 - W) * (W0 - W) / 6. / W / W -
              4. * std::atanh(beta));

    double O1corr =
        FINESTRUCTURE / 2. / M_PI *
        (g - 3. * std::log(PMASSC2 / EMASSC2 / 2. / W0));

    double L =
        1.026725 * std::pow(1. - 2. * FINESTRUCTURE / 3. / M_PI * std::log(2. * W0), 9. / 4.);

    // 2nd order
    double d1f, d2, d3, d14;
    double lambda = std::sqrt(10) / R;
    double lambdaOverM =
        lambda / NMASSC2 * EMASSC2;  // this is dimensionless

    d14 = std::log(PMASSC2 / EMASSC2) -
          5. / 3. * std::log(2 * W) + 43. / 18.;

    d1f = std::log(lambdaOverM) - EULER_MASCHERONI_CONSTANT + 4. / 3. -
          std::log(std::sqrt(10.0)) -
          3.0 / M_PI / std::sqrt(10.0) * lambdaOverM * (0.5 + EULER_MASCHERONI_CONSTANT +
                                          std::log(std::sqrt(10) / lambdaOverM));

    d2 = 3.0 / 2.0 / M_PI / std::sqrt(10.0) * lambdaOverM *
         (1. - M_PI / 2. / std::sqrt(10) * lambdaOverM);

    d3 = 3.0 * gA * gM / M_PI / std::sqrt(10.0) * lambdaOverM *
         (EULER_MASCHERONI_CONSTANT - 1. + std::log(std::sqrt(10) / lambdaOverM) +
          M_PI / 4 / std::sqrt(10) * lambdaOverM);

    double O2corr = FINESTRUCTURE * FINESTRUCTURE * Z * (d14 + d1f + d2 + d3);

    // 3rd order
    double a = 0.5697;
    double b =
        4. / 3. / M_PI * (11. / 4. - EULER_MASCHERONI_CONSTANT - M_PI * M_PI / 6);
    double f = std::log(2 * W) - 5. / 6.;
    double g2 = 0.5 * (std::pow(std::log(R), 2.) - std::pow(std::log(2 * W), 2.)) +
                5. / 3. * std::log(2 * R * W);

    double O3corr = std::pow(FINESTRUCTURE, 3) * std::pow(Z, 2) *
                    (a * std::log(lambda / W) + b * f + 4. / M_PI / 3. * g2 -
                     0.649 * std::log(2 * W0));

    return (1 + O1corr) * (L + O2corr + O3corr);
  }

  inline double NeutrinoRadiativeCorrection(double Wv) { ////// vNeutrino Radiative Correction ------ Rnu (OK (attention à Wv) mais pas utilisé)
    double h = 0.;
    double pv = std::sqrt(Wv * Wv - 1);
    double beta = pv / Wv;
    h += 3 * std::log(PMASSC2 / EMASSC2) + 23 / 4. +
         8 / beta * Spence(2 * beta / (1 + beta)) +
         8 * (std::atan(beta) / beta - 1) * std::log(2 * Wv * beta) +
         4 * std::atan(beta) / beta *
             ((7 + 3 * beta * beta) / 8 - 2 * std::atan(beta));
    return 1 + FINESTRUCTURE / 2 / M_PI * h;
  }

  inline double RecoilCorrection(double W, double W0, int A,  /////// RECOIL CORRECTION ------ Rn (OK mais pas utilisé)
                                             int decayType, double mixingRatio) {
    double Vr0, Vr1, Vr2, Vr3;
    double Ar0, Ar1, Ar2, Ar3;
    double M = A * (PMASSC2 + NMASSC2) / 2. /
               EMASSC2;  // in units of electron mass
    double M2 = M*M;

    Ar0 = -2. * W0 / 3. / M - W0 * W0 / 6. / M2 - 77. / 18. / M2;
    Ar1 = -2. / 3. / M + 7. * W0 / 9. / M2;
    Ar2 = 10. / 3. / M - 28. * W0 / 9. / M2;
    Ar3 = 88. / 9. / M2;

    Vr0 = W0 * W0 / 2. / M2 - 11. / 6. / M2;
    Vr1 = W0 / 3. / M2;
    Vr2 = 2. / M - 4. * W0 / 3. / M2;
    Vr3 = 16. / 3. / M2;

    if (decayType == FERMI) {
      return 1 + Vr0 + Vr1 / W + Vr2 * W + Vr3 * W * W;
    } else if (decayType == GAMOW_TELLER) {
      return 1 + Ar0 + Ar1 / W + Ar2 * W + Ar3 * W * W;
    } else if (mixingRatio > 0) {
      return 1 +
             1. / (1 + std::pow(mixingRatio, 2)) *
                 (Vr0 + Vr1 / W + Vr2 * W + Vr3 * W * W) +
             1. / (1 + 1. / std::pow(mixingRatio, 2)) *
                 (Ar0 + Ar1 / W + Ar2 * W + Ar3 * W * W);
    }
    return 1;
  }

  inline double AtomicScreeningCorrection(double W, int Z,          ///////// Atomic Screening ------- S (OK mais pas utilisé)
                                                    int betaType) {
    std::vector<double> Aby, Bby;

    screening::PotParam(Z - 1 * betaType, Aby, Bby);

    double l = 2 * (Aby[0] * Bby[0] + Aby[1] * Bby[1] + Aby[2] * Bby[2]);

    double p = std::sqrt(W * W - 1);

    double Wt = W - betaType * 0.5 * FINESTRUCTURE * (Z - betaType) * l;

    std::complex<double> pt;

    pt = 0.5 * p +
         0.5 * std::sqrt(std::complex<double>(p * p -
                                              betaType * 2 * FINESTRUCTURE * Z * Wt * l));

    double y = betaType * FINESTRUCTURE * Z * W / p;
    std::complex<double> yt = betaType * FINESTRUCTURE * Z * Wt / pt;

    double gamma = std::sqrt(1. - pow(FINESTRUCTURE * Z, 2.));

    /*cout << "V: " << W-Wt << endl;
    cout << W << " " << yt.real() << " " << yt.imag() << endl;
    cout << W << " " << pt.real() << " " << pt.imag() << endl;*/

    gsl_sf_result magn;
    gsl_sf_result phase;
    gsl_sf_lngamma_complex_e(gamma, y, &magn, &phase);

    gsl_sf_result magnT;
    gsl_sf_result phaseT;
    gsl_sf_lngamma_complex_e(gamma - yt.imag(), yt.real(), &magnT, &phaseT);
    // now we have what we wAt in magn.val

    double first = Wt / W;
    double second = std::exp(2 * (magnT.val - magn.val));

    gsl_sf_lngamma_complex_e(gamma - 2 * pt.imag() / l, 2 * pt.real() / l, &magnT,
                             &phaseT);
    gsl_sf_lngamma_complex_e(1, 2 * p / l, &magn, &phase);
    double third = std::exp(2 * (magnT.val - magn.val));
    double fourth = std::exp(-M_PI * y);
    double fifth = std::pow(2 * p / l, 2 * (1 - gamma));

    return first * second * third * fourth * fifth;
  }

  // inline double AtomicExchangeCorrection(double W, double exPars[9]) {    ///////////// ATOMIC EXCHNAGE -------- X (OK avec exPars définit mais pas utilisé) (oir csv)
  //   double E = W - 1;
  //
  //   return 1 + exPars[0] / E + exPars[1] / E / E +
  //          exPars[2] * std::exp(-exPars[3] * E) +
  //          exPars[4] * sin(std::pow(W - exPars[6], exPars[5]) + exPars[7]) /
  //          std::pow(W, exPars[8]);
  // }

  //modified by SL 10/05/2023
  inline double AtomicExchangeCorrection(double W, int Z) {    ///////////// ATOMIC EXCHNAGE -------- X (crée par SL 10/05/2023)
    double E = W - 1;

    std::vector<double> exPars;
    screening::AtomicExchangeFitParams(Z, exPars);

    return 1 + exPars[0] / E + exPars[1] / E / E +
           exPars[2] * std::exp(-exPars[3] * E) +
           exPars[4] * sin(std::pow(W - exPars[6], exPars[5]) + exPars[7]) /
           std::pow(W, exPars[8]);
  }

  inline double AtomicMismatchCorrection(double W, double W0, int Z, ///////////ATOMIC OVERLAP ------ r (OK mais pas utilisé)
                                                   int A, int betaType) {
    double dBdZ2 = (44.200 * std::pow(Z - betaType, 0.41) +
                    2.3196E-7 * std::pow(Z - betaType, 4.45)) /
                   EMASSC2 / 1000.;

    double K = -0.872 + 1.270 * std::pow(Z, 0.097) + 9.062E-11 * std::pow(Z, 4.5);
    double vp = std::sqrt(1 - 1 / W / W);
    double l = 1.83E-3 * K * Z / vp;
    double M = A * (PMASSC2 + NMASSC2) / 2. / EMASSC2;
    // assume as A average the recoil velocity at half-momentum trAsfer ~
    // std::sqrt(W0^2-1)/2
    double vR = std::sqrt(1 - M * M / (M * M + (W0 * W0 - 1) / 4.));

    double psi2 = 1 + 2 * FINESTRUCTURE / vp * (std::atan(1 / l) - l / 2 / (1 + l * l));

    double C0 = -FINESTRUCTURE * FINESTRUCTURE * Z * FINESTRUCTURE / vp * l / (1 + l * l) / psi2;

    double C1 = 2 * FINESTRUCTURE * FINESTRUCTURE * Z * vR / vp *
                ((0.5 + l * l) / (1 + l * l) - l * std::atan(1 / l)) / psi2;

    return 1 - 2 / (W0 - W) * (0.5 * dBdZ2 + 2 * (C0 + C1));
  }

  inline double ApproximateRadius(double A) {
    // Return radius in m
    return (1.15+1.8*std::pow(A, -2./3.)-1.2*std::pow(A, -4./3.))*std::pow(A, 1./3.)*1.E-15;
  }

  inline double GetSpectrumHeight(int Z, int A, double Q, double E, bool advanced, std::string DecayType) {
    double W = E/EMASSC2+1.;
    double W0 = Q/EMASSC2+1.;
    int decayType;
    double R = std::sqrt(5./3.)*ApproximateRadius(A)/NATURALLENGTH;
    if (advanced) {
      int betaType = (int)((Z > 0) - (Z < 0));
      Z = std::abs(Z);
      double cShape, cNS;

      DecayManager& dm = DecayManager::GetInstance();
      if (DecayType == "Fermi") {
        decayType = FERMI;
      }
      else{
        decayType = GAMOW_TELLER;
      }

      std::tie(cShape, cNS) = CCorrectionComponents(W, W0, Z, A, R, betaType, decayType, 1.27, -229, 1, 4.*A, 1*A, 0);
      double CCorr = cShape + cNS;
      //My correction SL 10/05/2023
      return PhaseSpace(W, W0)*FermiFunction(Z, W, R, betaType)*AtomicExchangeCorrection(W, Z)*L0Correction(W, Z, R, betaType)*CCorr*UCorrection(W, Z, betaType)*AtomicScreeningCorrection(W, Z, betaType)*RadiativeCorrection(W, W0, Z, R, 1.27, 4.706)*RecoilCorrection(W, W0, A, decayType, 0)*AtomicMismatchCorrection(W, W0, Z, A, betaType)*QCorrection(W, W0, Z, A, betaType, decayType);

      //Raw implementation
      //return PhaseSpace(W, W0)*FermiFunction(Z, W, R, betaType)*L0Correction(W, Z, R, betaType)*CCorr*UCorrection(W, Z, betaType)*AtomicScreeningCorrection(W, Z, betaType)*RadiativeCorrection(W, W0, Z, R, 1.27, 4.7);

      //No Correction
      //return PhaseSpace(W, W0)*FermiFunction(Z, W, R, betaType);
    }
    else {
      return PhaseSpace(W, W0)*SimpleFermiFunction(Z, GetSpeed(E, EMASSC2));
    }
  }

  inline std::vector<std::vector<double> >* GenerateBetaSpectrum(int Z, int A, double Q, bool advancedFermi, std::string DecayType) {
    std::vector<std::vector<double> >* dist = new std::vector<std::vector<double> >();
    double stepSize = 1.0;

    double currentEnergy = stepSize;
    while(currentEnergy <= Q) {
      double s = GetSpectrumHeight(Z, A, Q, currentEnergy, advancedFermi, DecayType);
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
    int N = 1800;
    double stepSize = 2./N;
    double currentCosAngle = -1;

    while (currentCosAngle <= 1) {
      std::vector<double> pair;
      pair.push_back(currentCosAngle);
      double W = 0.;
      for (std::vector<double>::size_type i = 0; i != A.size(); i++) {
        W+=A[i]*boost::math::legendre_p(i, currentCosAngle);
      }
      pair.push_back(W);
      dist.push_back(pair);
      currentCosAngle+=stepSize;
    }

    double costheta = RandomFromDistribution(dist);
    double theta = std::acos(costheta); //// remplacement de theta par costheta par SL 10/05/2023
    vector<double> perp = CrossProduct(dir2, RandomDirection());
    perp = NormaliseVector(perp); ///Ajouté par SL 10/05/2023
    vector<double> dir = RotateAroundVector(dir2, perp, theta);
    return dir;
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

  inline double GetAMEMass(std::string filename, int Z, int A) {
    std::ifstream ameDataFile(filename.c_str());
    std::string line;


    int skipHeader = 36;
    int currentLine = 0;

    while(getline(ameDataFile, line)) {
      currentLine++;
      if (currentLine <= skipHeader) {
        continue;
      } else {
        int nMinusz;
	int n;
	int z;
	int a;
	std::string name;
	std::string O;
	double massExcess;
	double massExcessUnc;
	double bindingEnergy;
	double bindingEnergyUnc;
	std::string betaDecay;
	double betaEnergy;
	double betaEnergyUnc;
	double atomicMass;
	double atomicMassUnc;

	nMinusz = std::stoi(line.substr(1, 3));
	n = std::stoi(line.substr(4, 5));
	z = std::stoi(line.substr(9, 5));
	a = std::stoi(line.substr(14, 5));
	name = line.substr(19, 4);
	//O = line.substr(23, 5);
	//std::cout << "Name " << name << std::endl;
	//massExcess = std::stod(line.substr(30, 14));
	//massExcessUnc

	std::string atomicMassString = line.substr(106, 15).replace(3, 1, "");
	std::replace(atomicMassString.begin(), atomicMassString.end(), '#', '.');
	atomicMass = std::stod(atomicMassString)*1e-6*UMASSC2;
	if (z == Z && a == A) {
          std::cout << "Found AME atomic mass for " << a << name << ": " << atomicMass << std::endl;
	  return atomicMass;
	}
      }
    }
    return 0.;
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
}//End of utilities namespace
}//End of CRADLE namespace
#endif
