#ifndef CRADLE_KINEMATIC_STRUCTURES_H
#define CRADLE_KINEMATIC_STRUCTURES_H

namespace CRADLE {
  namespace kinematic {

    inline void ThreeBodyDecay(vector<double>& velocity, PDS::core::DynamicParticle& finalState1, PDS::core::DynamicParticle& finalState2, PDS::core::DynamicParticle& finalState3, vector<double>& dir2, double Q) {
      //Perform decay in CoM frame
      vector<double> momentum1 = finalState1.GetFourMomentum();
      vector<double> momentum2 (4);
      vector<double> momentum3 (4);

      vector<double> p2 (3);
      double p2Norm = 0.;

      double mass1 = finalState1.GetRestMass();
      double mass2 = finalState2.GetRestMass();
      double mass3 = finalState3.GetRestMass();
      vector<double> p1 = finalState1.Get3Momentum();

      double a = mass2*mass2;
      double b = mass3*mass3;
      double c = GetNorm(p1);
      double d = Q + mass1 + mass2 + mass3 - momentum1(0);
      double e = inner_prod(p1, dir2)/c;

      double first = 1./2./(c*c*e*e-d*d);
      double second = a*a*d*d-2*a*b*d*d+4.*a*c*c*d*d*e*e-2.*a*c*c*d*d-2.*a*d*d*d*d+b*b*d*d+2*b*c*c*d*d-2.*b*d*d*d*d+c*c*c*c*d*d-2.*c*c*d*d*d*d+d*d*d*d*d*d;
      double third = a*c*e-b*c*e-c*c*c*e+c*d*d*e;

      p2Norm = first*(-std::sqrt(second)+third);
      p2 = p2Norm*dir2;

      vector<double> p3 = -(p1+p2);
      double p3Norm = GetNorm(p3);

      momentum2(0) = std::sqrt(a+p2Norm*p2Norm);
      momentum2(1) = p2(0);
      momentum2(2) = p2(1);
      momentum2(3) = p2(2);

      momentum3(0) = std::sqrt(b+p3Norm*p3Norm);
      momentum3(1) = p3(0);
      momentum3(2) = p3(1);
      momentum3(3) = p3(2);

      // Perform Lorentz boost back to lab frame
      finalState1.SetFourMomentum(LorentzBoost(velocity, momentum1));
      finalState2.SetFourMomentum(LorentzBoost(velocity, momentum2));
      finalState3.SetFourMomentum(LorentzBoost(velocity, momentum3));

    }

    inline void TwoBodyDecay(vector<double>& velocity, PDS::core::DynamicParticle& finalState1, PDS::core::DynamicParticle& finalState2, double Q) {
      vector<double> momentum1 (4);
      vector<double> momentum2 (4);

      vector<double> dir = RandomDirection();

      double mass1 = finalState1.GetRestMass();
      double mass2 = finalState2.GetRestMass();

      double M = Q + mass1 + mass2;

      double p = 1./(2.*M)*std::sqrt((M*M-std::pow(mass1-mass2, 2.))*(M*M-std::pow(mass1+mass2, 2.)));

      double energy1 = std::sqrt(mass1*mass1+p*p);
      double energy2 = std::sqrt(mass2*mass2+p*p);

      momentum1(0) = energy1;
      momentum1(1) = -p*dir[0];
      momentum1(2) = -p*dir[1];
      momentum1(3) = -p*dir[2];

      momentum2(0) = energy2;
      momentum2(1) = p*dir[0];
      momentum2(2) = p*dir[1];
      momentum2(3) = p*dir[2];

      // Perform Lorentz boost back to lab frame
      finalState1.SetFourMomentum(LorentzBoost(velocity, momentum1));
      finalState2.SetFourMomentum(LorentzBoost(velocity, momentum2));
    }

  }//end of kinematic namespace
}//end of CRADLE namespace

#endif
