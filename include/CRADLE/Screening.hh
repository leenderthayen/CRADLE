#ifndef SCREENING
#define SCREENING

#include <vector>

namespace CRADLE {

namespace screening {

  const double FINESTRUCTURE = 0.0072973525664;

/**
 * Returns the fit parameters of the atomic potential
 * by Salvat et al., Physical Review A 36 (1987) 467-474
 * for Z <= 92. If Z is larger, the Moliere potential is used
 *
 * @param Zloc the proton number of the atom
 * @param Aby reference to a vector to be filled with A coefficients
 * @param Bby reference to a vector to be filled with B coefficients
 */

inline void AtomicExchangeFitParams(int Z, std::vector<double> &exPars)
{
  exPars.clear();

  if (Z == 2){
	exPars.push_back(-1.086e-07);
	exPars.push_back(1.068e-10);
	exPars.push_back(1.871e-05);
	exPars.push_back(17.22);
	exPars.push_back(1.113e-05);
	exPars.push_back(14.88);
	exPars.push_back(0.952);
	exPars.push_back(-0.8818);
	exPars.push_back(2.393);
	}

if (Z == 3){
	exPars.push_back(2.997e-05);
	exPars.push_back(1.687e-08);
	exPars.push_back(-0.001976);
	exPars.push_back(10.24);
	exPars.push_back(0.0001058);
	exPars.push_back(-0.7653);
	exPars.push_back(0.8487);
	exPars.push_back(-1.244);
	exPars.push_back(3.414);
	}

if (Z == 4){
	exPars.push_back(4.988e-05);
	exPars.push_back(2.844e-08);
	exPars.push_back(-0.001998);
	exPars.push_back(6.092);
	exPars.push_back(6.669e-05);
	exPars.push_back(-1.009);
	exPars.push_back(0.8709);
	exPars.push_back(-1.423);
	exPars.push_back(-2.134);
	}

if (Z == 5){
	exPars.push_back(7.62e-05);
	exPars.push_back(3.072e-08);
	exPars.push_back(-0.002104);
	exPars.push_back(4.125);
	exPars.push_back(6.329e-05);
	exPars.push_back(-1.289);
	exPars.push_back(0.7189);
	exPars.push_back(-2.24);
	exPars.push_back(-0.9738);
	}

if (Z == 6){
	exPars.push_back(0.0001062);
	exPars.push_back(2.468e-08);
	exPars.push_back(-0.002182);
	exPars.push_back(3.037);
	exPars.push_back(5.568e-05);
	exPars.push_back(-1.74);
	exPars.push_back(0.7762);
	exPars.push_back(-1.903);
	exPars.push_back(-0.6206);
	}

if (Z == 7){
	exPars.push_back(0.0001401);
	exPars.push_back(8.89e-09);
	exPars.push_back(-0.002264);
	exPars.push_back(2.388);
	exPars.push_back(0.00234);
	exPars.push_back(12.94);
	exPars.push_back(1.0);
	exPars.push_back(-0.3328);
	exPars.push_back(9.933);
	}

if (Z == 8){
	exPars.push_back(0.0001742);
	exPars.push_back(-1.178e-08);
	exPars.push_back(-0.002299);
	exPars.push_back(1.951);
	exPars.push_back(0.002662);
	exPars.push_back(10.42);
	exPars.push_back(1.0);
	exPars.push_back(-0.327);
	exPars.push_back(10.13);
	}

if (Z == 9){
	exPars.push_back(0.0002128);
	exPars.push_back(-4.182e-08);
	exPars.push_back(-0.002432);
	exPars.push_back(1.723);
	exPars.push_back(0.0005917);
	exPars.push_back(1.173);
	exPars.push_back(0.9959);
	exPars.push_back(-0.551);
	exPars.push_back(1.876);
	}

if (Z == 10){
	exPars.push_back(0.0002439);
	exPars.push_back(-1.109e-07);
	exPars.push_back(-0.002335);
	exPars.push_back(1.469);
	exPars.push_back(0.000414);
	exPars.push_back(0.6387);
	exPars.push_back(0.9951);
	exPars.push_back(-0.5716);
	exPars.push_back(1.975);
	}

if (Z == 11){
	exPars.push_back(0.0002596);
	exPars.push_back(-1.301e-07);
	exPars.push_back(-0.002453);
	exPars.push_back(3.983);
	exPars.push_back(0.0007148);
	exPars.push_back(2.133);
	exPars.push_back(0.9346);
	exPars.push_back(-0.8554);
	exPars.push_back(0.2942);
	}

if (Z == 12){
	exPars.push_back(0.0002734);
	exPars.push_back(-1.45e-07);
	exPars.push_back(-0.001154);
	exPars.push_back(0.513);
	exPars.push_back(0.00194);
	exPars.push_back(7.942);
	exPars.push_back(0.9964);
	exPars.push_back(-0.4244);
	exPars.push_back(9.947);
	}

if (Z == 13){
	exPars.push_back(0.0002891);
	exPars.push_back(-1.66e-07);
	exPars.push_back(-0.000889);
	exPars.push_back(0.2754);
	exPars.push_back(-0.001986);
	exPars.push_back(7.023);
	exPars.push_back(0.999);
	exPars.push_back(-0.4004);
	exPars.push_back(0.8219);
	}

if (Z == 14){
	exPars.push_back(0.0001751);
	exPars.push_back(-7.528e-08);
	exPars.push_back(0.0381);
	exPars.push_back(98.43);
	exPars.push_back(-0.0005707);
	exPars.push_back(-0.1716);
	exPars.push_back(0.9995);
	exPars.push_back(-0.4109);
	exPars.push_back(10.76);
	}

if (Z == 15){
	exPars.push_back(0.0001929);
	exPars.push_back(-9.963e-08);
	exPars.push_back(0.03712);
	exPars.push_back(87.51);
	exPars.push_back(-0.0002431);
	exPars.push_back(-0.8754);
	exPars.push_back(0.8911);
	exPars.push_back(-1.241);
	exPars.push_back(-2.264);
	}

if (Z == 16){
	exPars.push_back(0.0002041);
	exPars.push_back(-1.164e-07);
	exPars.push_back(0.03362);
	exPars.push_back(74.63);
	exPars.push_back(0.0002384);
	exPars.push_back(-0.8782);
	exPars.push_back(0.8939);
	exPars.push_back(-1.228);
	exPars.push_back(1.302);
	}

if (Z == 17){
	exPars.push_back(0.0002136);
	exPars.push_back(-1.276e-07);
	exPars.push_back(0.03155);
	exPars.push_back(64.44);
	exPars.push_back(0.0002344);
	exPars.push_back(-0.9904);
	exPars.push_back(0.9057);
	exPars.push_back(-1.181);
	exPars.push_back(1.636);
	}

if (Z == 18){
	exPars.push_back(0.0002226);
	exPars.push_back(-7.221e-08);
	exPars.push_back(0.04052);
	exPars.push_back(91.46);
	exPars.push_back(0.0004061);
	exPars.push_back(-0.2184);
	exPars.push_back(0.999);
	exPars.push_back(-0.4153);
	exPars.push_back(7.299);
	}

if (Z == 19){
	exPars.push_back(0.0001916);
	exPars.push_back(-2.147e-07);
	exPars.push_back(0.04966);
	exPars.push_back(50.15);
	exPars.push_back(0.002803);
	exPars.push_back(17.22);
	exPars.push_back(0.998);
	exPars.push_back(-0.4577);
	exPars.push_back(8.447);
	}

if (Z == 20){
	exPars.push_back(0.0001935);
	exPars.push_back(-1.588e-07);
	exPars.push_back(0.03318);
	exPars.push_back(43.35);
	exPars.push_back(-0.0002428);
	exPars.push_back(-1.565);
	exPars.push_back(0.9078);
	exPars.push_back(-1.183);
	exPars.push_back(5.468);
	}

if (Z == 21){
	exPars.push_back(0.0002985);
	exPars.push_back(-1.152e-07);
	exPars.push_back(0.03727);
	exPars.push_back(67.18);
	exPars.push_back(0.001018);
	exPars.push_back(4.676);
	exPars.push_back(0.9735);
	exPars.push_back(-0.7184);
	exPars.push_back(4.303);
	}

if (Z == 22){
	exPars.push_back(0.0002494);
	exPars.push_back(-1.617e-07);
	exPars.push_back(0.03268);
	exPars.push_back(39.45);
	exPars.push_back(-0.0001136);
	exPars.push_back(-2.425);
	exPars.push_back(0.8876);
	exPars.push_back(-1.448);
	exPars.push_back(-0.5778);
	}

if (Z == 23){
	exPars.push_back(0.0002098);
	exPars.push_back(-1.46e-07);
	exPars.push_back(0.0328);
	exPars.push_back(31.89);
	exPars.push_back(-0.0001133);
	exPars.push_back(-2.865);
	exPars.push_back(0.7308);
	exPars.push_back(-2.082);
	exPars.push_back(-0.2984);
	}

if (Z == 24){
	exPars.push_back(0.000285);
	exPars.push_back(-1.841e-07);
	exPars.push_back(0.03129);
	exPars.push_back(38.03);
	exPars.push_back(-0.000492);
	exPars.push_back(1.98);
	exPars.push_back(0.7955);
	exPars.push_back(-1.637);
	exPars.push_back(0.4723);
	}

if (Z == 25){
	exPars.push_back(0.0002617);
	exPars.push_back(-1.806e-07);
	exPars.push_back(0.03229);
	exPars.push_back(31.92);
	exPars.push_back(-0.0004422);
	exPars.push_back(1.403);
	exPars.push_back(0.7347);
	exPars.push_back(-1.975);
	exPars.push_back(0.6128);
	}

if (Z == 26){
	exPars.push_back(0.0002662);
	exPars.push_back(-1.898e-07);
	exPars.push_back(0.03202);
	exPars.push_back(29.75);
	exPars.push_back(-0.0004369);
	exPars.push_back(1.363);
	exPars.push_back(0.7008);
	exPars.push_back(-2.157);
	exPars.push_back(0.8981);
	}

if (Z == 27){
	exPars.push_back(0.0002659);
	exPars.push_back(-1.926e-07);
	exPars.push_back(0.03203);
	exPars.push_back(27.68);
	exPars.push_back(-0.0004216);
	exPars.push_back(1.245);
	exPars.push_back(0.685);
	exPars.push_back(-2.253);
	exPars.push_back(1.07);
	}

if (Z == 28){
	exPars.push_back(0.00023);
	exPars.push_back(-1.786e-07);
	exPars.push_back(0.03185);
	exPars.push_back(23.44);
	exPars.push_back(-0.0002733);
	exPars.push_back(0.2695);
	exPars.push_back(0.6736);
	exPars.push_back(-2.394);
	exPars.push_back(0.712);
	}

if (Z == 29){
	exPars.push_back(0.0002932);
	exPars.push_back(-2.19e-07);
	exPars.push_back(0.03161);
	exPars.push_back(26.34);
	exPars.push_back(-0.0004634);
	exPars.push_back(1.464);
	exPars.push_back(0.685);
	exPars.push_back(-2.215);
	exPars.push_back(1.496);
	}

if (Z == 30){
	exPars.push_back(0.0001543);
	exPars.push_back(-1.5e-07);
	exPars.push_back(0.03586);
	exPars.push_back(19.45);
	exPars.push_back(-0.00055);
	exPars.push_back(-0.2227);
	exPars.push_back(0.9987);
	exPars.push_back(-0.509);
	exPars.push_back(-0.4903);
	}

if (Z == 31){
	exPars.push_back(0.0002639);
	exPars.push_back(-2.105e-07);
	exPars.push_back(0.02886);
	exPars.push_back(20.65);
	exPars.push_back(-0.0003007);
	exPars.push_back(0.7157);
	exPars.push_back(0.678);
	exPars.push_back(-2.297);
	exPars.push_back(1.62);
	}

if (Z == 32){
	exPars.push_back(0.0002855);
	exPars.push_back(-2.329e-07);
	exPars.push_back(0.03486);
	exPars.push_back(23.01);
	exPars.push_back(-0.002991);
	exPars.push_back(8.912);
	exPars.push_back(0.9955);
	exPars.push_back(-0.4896);
	exPars.push_back(5.965);
	}

if (Z == 33){
	exPars.push_back(-4.79e-05);
	exPars.push_back(-1.026e-07);
	exPars.push_back(0.05153);
	exPars.push_back(17.04);
	exPars.push_back(0.01144);
	exPars.push_back(34.96);
	exPars.push_back(0.9991);
	exPars.push_back(-0.3853);
	exPars.push_back(11.55);
	}

if (Z == 34){
	exPars.push_back(7.407e-05);
	exPars.push_back(-1.032e-07);
	exPars.push_back(0.03488);
	exPars.push_back(15.47);
	exPars.push_back(0.001788);
	exPars.push_back(2.474);
	exPars.push_back(0.9847);
	exPars.push_back(-0.5869);
	exPars.push_back(3.104);
	}

if (Z == 35){
	exPars.push_back(7.408e-05);
	exPars.push_back(-9.532e-08);
	exPars.push_back(0.03537);
	exPars.push_back(15.36);
	exPars.push_back(0.001793);
	exPars.push_back(2.177);
	exPars.push_back(0.9782);
	exPars.push_back(-0.6341);
	exPars.push_back(2.949);
	}

if (Z == 36){
	exPars.push_back(0.000199);
	exPars.push_back(-1.086e-07);
	exPars.push_back(0.02094);
	exPars.push_back(12.74);
	exPars.push_back(0.000988);
	exPars.push_back(2.218);
	exPars.push_back(0.9673);
	exPars.push_back(-0.7077);
	exPars.push_back(2.972);
	}

if (Z == 37){
	exPars.push_back(0.0004465);
	exPars.push_back(-3.168e-07);
	exPars.push_back(0.02259);
	exPars.push_back(24.14);
	exPars.push_back(-0.00328);
	exPars.push_back(9.347);
	exPars.push_back(0.9962);
	exPars.push_back(-0.4799);
	exPars.push_back(5.931);
	}

if (Z == 38){
	exPars.push_back(0.0001775);
	exPars.push_back(-1.787e-07);
	exPars.push_back(0.03279);
	exPars.push_back(13.9);
	exPars.push_back(0.003394);
	exPars.push_back(4.506);
	exPars.push_back(0.9814);
	exPars.push_back(-0.6096);
	exPars.push_back(2.95);
	}

if (Z == 39){
	exPars.push_back(0.0001566);
	exPars.push_back(-1.638e-07);
	exPars.push_back(0.032);
	exPars.push_back(13.55);
	exPars.push_back(0.003428);
	exPars.push_back(4.234);
	exPars.push_back(0.9823);
	exPars.push_back(-0.6032);
	exPars.push_back(3.034);
	}

if (Z == 40){
	exPars.push_back(0.000213);
	exPars.push_back(-1.489e-07);
	exPars.push_back(0.03627);
	exPars.push_back(14.87);
	exPars.push_back(-0.00354);
	exPars.push_back(4.125);
	exPars.push_back(0.98);
	exPars.push_back(-0.6241);
	exPars.push_back(-0.3205);
	}

if (Z == 41){
	exPars.push_back(0.0001328);
	exPars.push_back(-1.679e-07);
	exPars.push_back(0.03384);
	exPars.push_back(13.13);
	exPars.push_back(0.008717);
	exPars.push_back(26.32);
	exPars.push_back(0.999);
	exPars.push_back(-0.3848);
	exPars.push_back(36.96);
	}

if (Z == 42){
	exPars.push_back(0.0001691);
	exPars.push_back(-1.566e-07);
	exPars.push_back(0.0256);
	exPars.push_back(11.14);
	exPars.push_back(0.002885);
	exPars.push_back(3.548);
	exPars.push_back(0.9741);
	exPars.push_back(-0.6512);
	exPars.push_back(3.159);
	}

if (Z == 43){
	exPars.push_back(0.000109);
	exPars.push_back(-1.306e-07);
	exPars.push_back(0.03082);
	exPars.push_back(11.54);
	exPars.push_back(0.003426);
	exPars.push_back(3.301);
	exPars.push_back(0.9772);
	exPars.push_back(-0.6339);
	exPars.push_back(3.2);
	}

if (Z == 44){
	exPars.push_back(0.0001312);
	exPars.push_back(-1.38e-07);
	exPars.push_back(0.02775);
	exPars.push_back(10.6);
	exPars.push_back(0.003116);
	exPars.push_back(3.132);
	exPars.push_back(0.9732);
	exPars.push_back(-0.6554);
	exPars.push_back(3.253);
	}

if (Z == 45){
	exPars.push_back(0.00012);
	exPars.push_back(-1.333e-07);
	exPars.push_back(0.02818);
	exPars.push_back(10.27);
	exPars.push_back(0.003148);
	exPars.push_back(2.954);
	exPars.push_back(0.972);
	exPars.push_back(-0.6624);
	exPars.push_back(3.291);
	}

if (Z == 46){
	exPars.push_back(0.0001367);
	exPars.push_back(-1.365e-07);
	exPars.push_back(0.0272);
	exPars.push_back(9.318);
	exPars.push_back(0.002644);
	exPars.push_back(2.635);
	exPars.push_back(0.9632);
	exPars.push_back(-0.7076);
	exPars.push_back(3.329);
	}

if (Z == 47){
	exPars.push_back(0.0001606);
	exPars.push_back(-1.613e-07);
	exPars.push_back(0.02531);
	exPars.push_back(9.299);
	exPars.push_back(0.002794);
	exPars.push_back(2.633);
	exPars.push_back(0.9656);
	exPars.push_back(-0.6955);
	exPars.push_back(3.362);
	}

if (Z == 48){
	exPars.push_back(0.0001065);
	exPars.push_back(-1.359e-07);
	exPars.push_back(0.02966);
	exPars.push_back(9.752);
	exPars.push_back(0.003311);
	exPars.push_back(2.561);
	exPars.push_back(0.973);
	exPars.push_back(-0.6541);
	exPars.push_back(3.421);
	}

if (Z == 49){
	exPars.push_back(0.0001474);
	exPars.push_back(-1.503e-07);
	exPars.push_back(0.02565);
	exPars.push_back(9.084);
	exPars.push_back(0.002762);
	exPars.push_back(2.288);
	exPars.push_back(0.9661);
	exPars.push_back(-0.6909);
	exPars.push_back(3.442);
	}

if (Z == 50){
	exPars.push_back(0.0001701);
	exPars.push_back(-1.763e-07);
	exPars.push_back(0.02568);
	exPars.push_back(9.253);
	exPars.push_back(0.002607);
	exPars.push_back(2.119);
	exPars.push_back(0.968);
	exPars.push_back(-0.6803);
	exPars.push_back(3.464);
	}

if (Z == 51){
	exPars.push_back(0.0002022);
	exPars.push_back(-2.052e-07);
	exPars.push_back(0.02561);
	exPars.push_back(9.642);
	exPars.push_back(0.00244);
	exPars.push_back(1.981);
	exPars.push_back(0.973);
	exPars.push_back(-0.6499);
	exPars.push_back(3.501);
	}

if (Z == 52){
	exPars.push_back(0.0002768);
	exPars.push_back(-2.693e-07);
	exPars.push_back(0.02824);
	exPars.push_back(11.83);
	exPars.push_back(0.002437);
	exPars.push_back(2.027);
	exPars.push_back(0.9994);
	exPars.push_back(-0.4774);
	exPars.push_back(3.72);
	}

if (Z == 53){
	exPars.push_back(-0.000345);
	exPars.push_back(9.634e-08);
	exPars.push_back(0.05688);
	exPars.push_back(10.13);
	exPars.push_back(0.004885);
	exPars.push_back(1.901);
	exPars.push_back(0.9779);
	exPars.push_back(-0.6305);
	exPars.push_back(3.521);
	}

if (Z == 54){
	exPars.push_back(9.169e-05);
	exPars.push_back(-2.279e-08);
	exPars.push_back(0.01653);
	exPars.push_back(4.935);
	exPars.push_back(-0.001601);
	exPars.push_back(1.182);
	exPars.push_back(0.865);
	exPars.push_back(-1.113);
	exPars.push_back(0.9171);
	}

if (Z == 55){
	exPars.push_back(0.0002354);
	exPars.push_back(-1.77e-07);
	exPars.push_back(0.02134);
	exPars.push_back(8.269);
	exPars.push_back(-0.0006621);
	exPars.push_back(-0.2613);
	exPars.push_back(0.8194);
	exPars.push_back(-1.357);
	exPars.push_back(0.2195);
	}

if (Z == 56){
	exPars.push_back(7.078e-05);
	exPars.push_back(-1.059e-07);
	exPars.push_back(0.03504);
	exPars.push_back(9.273);
	exPars.push_back(0.004701);
	exPars.push_back(19.81);
	exPars.push_back(0.9996);
	exPars.push_back(-0.3563);
	exPars.push_back(6.236);
	}

if (Z == 57){
	exPars.push_back(0.0001089);
	exPars.push_back(-1.237e-07);
	exPars.push_back(0.03055);
	exPars.push_back(8.57);
	exPars.push_back(-0.03235);
	exPars.push_back(101.9);
	exPars.push_back(1.001);
	exPars.push_back(-0.1102);
	exPars.push_back(13.89);
	}

if (Z == 58){
	exPars.push_back(0.0001046);
	exPars.push_back(-1.22e-07);
	exPars.push_back(0.03049);
	exPars.push_back(8.359);
	exPars.push_back(0.008993);
	exPars.push_back(96.62);
	exPars.push_back(1.001);
	exPars.push_back(-0.2621);
	exPars.push_back(8.552);
	}

if (Z == 59){
	exPars.push_back(0.0002514);
	exPars.push_back(-1.869e-07);
	exPars.push_back(0.02607);
	exPars.push_back(8.187);
	exPars.push_back(-0.01006);
	exPars.push_back(162.8);
	exPars.push_back(1.001);
	exPars.push_back(-0.2876);
	exPars.push_back(4.61);
	}

if (Z == 60){
	exPars.push_back(7.021e-05);
	exPars.push_back(-2.183e-07);
	exPars.push_back(0.03195);
	exPars.push_back(7.964);
	exPars.push_back(-0.01556);
	exPars.push_back(127.7);
	exPars.push_back(1.001);
	exPars.push_back(-0.249);
	exPars.push_back(43.05);
	}

if (Z == 61){
	exPars.push_back(0.0001422);
	exPars.push_back(-1.595e-07);
	exPars.push_back(0.02965);
	exPars.push_back(7.861);
	exPars.push_back(0.01142);
	exPars.push_back(104.8);
	exPars.push_back(1.001);
	exPars.push_back(-0.23);
	exPars.push_back(9.077);
	}

if (Z == 62){
	exPars.push_back(0.0001357);
	exPars.push_back(-1.504e-07);
	exPars.push_back(0.02981);
	exPars.push_back(7.695);
	exPars.push_back(0.01141);
	exPars.push_back(110.5);
	exPars.push_back(1.001);
	exPars.push_back(-0.2636);
	exPars.push_back(2.127);
	}

if (Z == 63){
	exPars.push_back(9.268e-05);
	exPars.push_back(-1.322e-07);
	exPars.push_back(0.03169);
	exPars.push_back(7.59);
	exPars.push_back(0.004465);
	exPars.push_back(19.45);
	exPars.push_back(1.0);
	exPars.push_back(-0.3413);
	exPars.push_back(0.1178);
	}

if (Z == 64){
	exPars.push_back(0.0001582);
	exPars.push_back(-1.679e-07);
	exPars.push_back(0.02751);
	exPars.push_back(7.133);
	exPars.push_back(0.01192);
	exPars.push_back(120.4);
	exPars.push_back(1.001);
	exPars.push_back(-0.2522);
	exPars.push_back(2.296);
	}

if (Z == 65){
	exPars.push_back(0.0001382);
	exPars.push_back(-1.602e-07);
	exPars.push_back(0.02932);
	exPars.push_back(7.15);
	exPars.push_back(0.01424);
	exPars.push_back(122.1);
	exPars.push_back(1.001);
	exPars.push_back(-0.2567);
	exPars.push_back(2.181);
	}

if (Z == 66){
	exPars.push_back(0.0001438);
	exPars.push_back(-1.691e-07);
	exPars.push_back(0.02895);
	exPars.push_back(6.952);
	exPars.push_back(0.004329);
	exPars.push_back(21.83);
	exPars.push_back(1.0);
	exPars.push_back(-0.3387);
	exPars.push_back(0.1043);
	}

if (Z == 67){
	exPars.push_back(0.0001406);
	exPars.push_back(-1.705e-07);
	exPars.push_back(0.02902);
	exPars.push_back(6.809);
	exPars.push_back(0.004528);
	exPars.push_back(21.38);
	exPars.push_back(0.9997);
	exPars.push_back(-0.3521);
	exPars.push_back(-0.007053);
	}

if (Z == 68){
	exPars.push_back(0.000136);
	exPars.push_back(-1.667e-07);
	exPars.push_back(0.02921);
	exPars.push_back(6.686);
	exPars.push_back(0.01386);
	exPars.push_back(107.9);
	exPars.push_back(1.001);
	exPars.push_back(-0.257);
	exPars.push_back(2.216);
	}

if (Z == 69){
	exPars.push_back(0.0001365);
	exPars.push_back(-1.685e-07);
	exPars.push_back(0.02896);
	exPars.push_back(6.51);
	exPars.push_back(0.01446);
	exPars.push_back(115.2);
	exPars.push_back(1.001);
	exPars.push_back(-0.2762);
	exPars.push_back(1.777);
	}

if (Z == 70){
	exPars.push_back(9.703e-05);
	exPars.push_back(-1.463e-07);
	exPars.push_back(0.03027);
	exPars.push_back(6.39);
	exPars.push_back(0.005402);
	exPars.push_back(20.21);
	exPars.push_back(0.9999);
	exPars.push_back(-0.3464);
	exPars.push_back(6.341);
	}

if (Z == 71){
	exPars.push_back(7.948e-05);
	exPars.push_back(-1.331e-07);
	exPars.push_back(0.02962);
	exPars.push_back(6.112);
	exPars.push_back(0.01362);
	exPars.push_back(100.9);
	exPars.push_back(1.001);
	exPars.push_back(-0.2597);
	exPars.push_back(14.75);
	}

if (Z == 72){
	exPars.push_back(3.533e-05);
	exPars.push_back(-9.579e-08);
	exPars.push_back(0.02999);
	exPars.push_back(5.866);
	exPars.push_back(0.004382);
	exPars.push_back(16.75);
	exPars.push_back(0.9996);
	exPars.push_back(-0.3557);
	exPars.push_back(12.67);
	}

if (Z == 73){
	exPars.push_back(1.075e-05);
	exPars.push_back(-7.545e-08);
	exPars.push_back(0.02959);
	exPars.push_back(5.605);
	exPars.push_back(0.007651);
	exPars.push_back(72.04);
	exPars.push_back(1.0);
	exPars.push_back(-0.2904);
	exPars.push_back(20.65);
	}

if (Z == 74){
	exPars.push_back(-8.774e-06);
	exPars.push_back(-6.116e-08);
	exPars.push_back(0.02884);
	exPars.push_back(5.354);
	exPars.push_back(0.003267);
	exPars.push_back(14.47);
	exPars.push_back(0.9996);
	exPars.push_back(-0.3547);
	exPars.push_back(19.08);
	}

if (Z == 75){
	exPars.push_back(-0.0001441);
	exPars.push_back(-8.529e-08);
	exPars.push_back(0.03097);
	exPars.push_back(5.158);
	exPars.push_back(-0.0001192);
	exPars.push_back(-3.617);
	exPars.push_back(0.3964);
	exPars.push_back(-5.578);
	exPars.push_back(0.1985);
	}

if (Z == 76){
	exPars.push_back(-4.895e-05);
	exPars.push_back(-2.286e-08);
	exPars.push_back(0.02821);
	exPars.push_back(4.969);
	exPars.push_back(0.002519);
	exPars.push_back(12.37);
	exPars.push_back(0.9983);
	exPars.push_back(-0.387);
	exPars.push_back(37.83);
	}

if (Z == 77){
	exPars.push_back(-0.0001488);
	exPars.push_back(5.76e-08);
	exPars.push_back(0.03026);
	exPars.push_back(4.861);
	exPars.push_back(0.002579);
	exPars.push_back(10.92);
	exPars.push_back(0.9956);
	exPars.push_back(-0.4393);
	exPars.push_back(12.48);
	}

if (Z == 78){
	exPars.push_back(-0.0001027);
	exPars.push_back(2.336e-08);
	exPars.push_back(0.02722);
	exPars.push_back(4.576);
	exPars.push_back(0.001626);
	exPars.push_back(9.83);
	exPars.push_back(0.9954);
	exPars.push_back(-0.4347);
	exPars.push_back(12.63);
	}

if (Z == 79){
	exPars.push_back(-4.278e-05);
	exPars.push_back(-2.664e-08);
	exPars.push_back(0.02494);
	exPars.push_back(4.352);
	exPars.push_back(-7.813e-07);
	exPars.push_back(-9.021);
	exPars.push_back(0.9478);
	exPars.push_back(-1.1);
	exPars.push_back(-12.95);
	}

if (Z == 80){
	exPars.push_back(-0.0002557);
	exPars.push_back(1.265e-07);
	exPars.push_back(0.03189);
	exPars.push_back(4.558);
	exPars.push_back(-9.565e-05);
	exPars.push_back(-3.286);
	exPars.push_back(0.492);
	exPars.push_back(-5.381);
	exPars.push_back(-5.901);
	}

if (Z == 81){
	exPars.push_back(-7.098e-05);
	exPars.push_back(5.439e-09);
	exPars.push_back(0.02371);
	exPars.push_back(3.945);
	exPars.push_back(9.153e-05);
	exPars.push_back(-5.026);
	exPars.push_back(0.221);
	exPars.push_back(-8.591);
	exPars.push_back(-0.05621);
	}

if (Z == 82){
	exPars.push_back(-0.0001143);
	exPars.push_back(3.228e-08);
	exPars.push_back(0.0253);
	exPars.push_back(3.939);
	exPars.push_back(-0.0002001);
	exPars.push_back(-4.59);
	exPars.push_back(0.1759);
	exPars.push_back(-10.52);
	exPars.push_back(3.104);
	}

if (Z == 83){
	exPars.push_back(-1.895e-05);
	exPars.push_back(-4.816e-08);
	exPars.push_back(0.02249);
	exPars.push_back(3.641);
	exPars.push_back(-1.259e-05);
	exPars.push_back(-4.992);
	exPars.push_back(0.9787);
	exPars.push_back(-0.7329);
	exPars.push_back(2.16);
	}

if (Z == 84){
	exPars.push_back(0.000137);
	exPars.push_back(-1.503e-07);
	exPars.push_back(0.0204);
	exPars.push_back(3.482);
	exPars.push_back(-0.004136);
	exPars.push_back(30.65);
	exPars.push_back(0.9998);
	exPars.push_back(-0.353);
	exPars.push_back(9.1);
	}

if (Z == 85){
	exPars.push_back(0.0002154);
	exPars.push_back(-2.676e-07);
	exPars.push_back(0.02117);
	exPars.push_back(3.698);
	exPars.push_back(-0.01036);
	exPars.push_back(18.87);
	exPars.push_back(1.0);
	exPars.push_back(-0.337);
	exPars.push_back(3.309);
	}

if (Z == 86){
	exPars.push_back(-0.00073);
	exPars.push_back(5.375e-07);
	exPars.push_back(0.04329);
	exPars.push_back(3.907);
	exPars.push_back(0.003152);
	exPars.push_back(1.051);
	exPars.push_back(0.2893);
	exPars.push_back(-6.118);
	exPars.push_back(-0.5791);
	}

if (Z == 87){
	exPars.push_back(4.102e-05);
	exPars.push_back(-5.003e-08);
	exPars.push_back(0.01967);
	exPars.push_back(2.9);
	exPars.push_back(0.002401);
	exPars.push_back(17.43);
	exPars.push_back(0.9683);
	exPars.push_back(-0.7324);
	exPars.push_back(4.706);
	}

if (Z == 88){
	exPars.push_back(-0.0001022);
	exPars.push_back(1.486e-08);
	exPars.push_back(0.02474);
	exPars.push_back(3.148);
	exPars.push_back(-0.002388);
	exPars.push_back(3.931);
	exPars.push_back(0.3439);
	exPars.push_back(-4.958);
	exPars.push_back(14.64);
	}

if (Z == 89){
	exPars.push_back(-0.0001146);
	exPars.push_back(2.322e-08);
	exPars.push_back(0.02495);
	exPars.push_back(3.07);
	exPars.push_back(-0.002624);
	exPars.push_back(3.817);
	exPars.push_back(0.372);
	exPars.push_back(-4.506);
	exPars.push_back(2.018);
	}

if (Z == 90){
	exPars.push_back(-0.0002164);
	exPars.push_back(1.138e-07);
	exPars.push_back(0.0261);
	exPars.push_back(2.975);
	exPars.push_back(-0.002449);
	exPars.push_back(3.434);
	exPars.push_back(0.4243);
	exPars.push_back(-3.825);
	exPars.push_back(1.952);
	}

if (Z == 91){
	exPars.push_back(-0.0001648);
	exPars.push_back(6.655e-08);
	exPars.push_back(0.02496);
	exPars.push_back(2.873);
	exPars.push_back(-7.087e-05);
	exPars.push_back(-4.039);
	exPars.push_back(0.5707);
	exPars.push_back(-5.835);
	exPars.push_back(-6.273);
	}

if (Z == 92){
	exPars.push_back(0.0009309);
	exPars.push_back(-7.636e-07);
	exPars.push_back(0.03979);
	exPars.push_back(5.967);
	exPars.push_back(-0.001319);
	exPars.push_back(-0.881);
	exPars.push_back(0.8498);
	exPars.push_back(-1.813);
	exPars.push_back(-0.5036);
	}

if (Z == 93){
	exPars.push_back(0.0004583);
	exPars.push_back(-3.712e-07);
	exPars.push_back(0.03995);
	exPars.push_back(4.31);
	exPars.push_back(-0.00565);
	exPars.push_back(2.561);
	exPars.push_back(0.3357);
	exPars.push_back(-4.846);
	exPars.push_back(2.008);
	}

if (Z == 94){
	exPars.push_back(0.0004697);
	exPars.push_back(-3.735e-07);
	exPars.push_back(0.04037);
	exPars.push_back(4.265);
	exPars.push_back(0.005618);
	exPars.push_back(2.467);
	exPars.push_back(0.3493);
	exPars.push_back(-4.619);
	exPars.push_back(-1.166);
	}

if (Z == 95){
	exPars.push_back(0.000482);
	exPars.push_back(-3.848e-07);
	exPars.push_back(0.04061);
	exPars.push_back(4.202);
	exPars.push_back(0.0113);
	exPars.push_back(10.15);
	exPars.push_back(0.06044);
	exPars.push_back(-25.82);
	exPars.push_back(20.97);
	}

if (Z == 96){
	exPars.push_back(0.000483);
	exPars.push_back(-3.837e-07);
	exPars.push_back(0.04109);
	exPars.push_back(4.146);
	exPars.push_back(-0.005051);
	exPars.push_back(27.38);
	exPars.push_back(-1.258);
	exPars.push_back(-5.775);
	exPars.push_back(20.62);
	}

if (Z == 97){
	exPars.push_back(0.0004955);
	exPars.push_back(-3.87e-07);
	exPars.push_back(0.04162);
	exPars.push_back(4.113);
	exPars.push_back(0.005018);
	exPars.push_back(27.21);
	exPars.push_back(-0.9652);
	exPars.push_back(-10.74);
	exPars.push_back(10.96);
	}

if (Z == 98){
	exPars.push_back(0.000506);
	exPars.push_back(-3.953e-07);
	exPars.push_back(0.04219);
	exPars.push_back(4.076);
	exPars.push_back(0.003836);
	exPars.push_back(-0.6679);
	exPars.push_back(-0.9847);
	exPars.push_back(-11.9);
	exPars.push_back(6.179);
	}

if (Z == 99){
	exPars.push_back(0.0005228);
	exPars.push_back(-4.119e-07);
	exPars.push_back(0.04253);
	exPars.push_back(4.026);
	exPars.push_back(-0.01513);
	exPars.push_back(337.7);
	exPars.push_back(-1.274);
	exPars.push_back(-6.563);
	exPars.push_back(17.42);
	}

if (Z == 100){
	exPars.push_back(0.0005227);
	exPars.push_back(-4.085e-07);
	exPars.push_back(0.04244);
	exPars.push_back(3.952);
	exPars.push_back(-0.02118);
	exPars.push_back(339.9);
	exPars.push_back(-0.4189);
	exPars.push_back(-6.116);
	exPars.push_back(22.69);
	}

if (Z == 101){
	exPars.push_back(0.0005413);
	exPars.push_back(-4.158e-07);
	exPars.push_back(0.04369);
	exPars.push_back(3.947);
	exPars.push_back(-0.3653);
	exPars.push_back(898.7);
	exPars.push_back(0.4293);
	exPars.push_back(-7.326);
	exPars.push_back(-9.633);
	}

if (Z == 102){
	exPars.push_back(0.0005535);
	exPars.push_back(-4.21e-07);
	exPars.push_back(0.04421);
	exPars.push_back(3.903);
	exPars.push_back(-0.01233);
	exPars.push_back(9.025);
	exPars.push_back(0.05011);
	exPars.push_back(-30.66);
	exPars.push_back(-0.7396);
	}

if (Z == 103){
	exPars.push_back(0.00056);
	exPars.push_back(-4.312e-07);
	exPars.push_back(0.04463);
	exPars.push_back(3.848);
	exPars.push_back(-0.0126);
	exPars.push_back(8.837);
	exPars.push_back(0.0479);
	exPars.push_back(-32.09);
	exPars.push_back(-0.6863);
	}

if (Z == 104){
	exPars.push_back(0.0005743);
	exPars.push_back(-4.496e-07);
	exPars.push_back(0.04518);
	exPars.push_back(3.802);
	exPars.push_back(-0.005438);
	exPars.push_back(1.558);
	exPars.push_back(0.8858);
	exPars.push_back(-0.9817);
	exPars.push_back(-5.088);
	}

if (Z == 105){
	exPars.push_back(0.0008192);
	exPars.push_back(-6.238e-07);
	exPars.push_back(0.05879);
	exPars.push_back(4.53);
	exPars.push_back(-0.02097);
	exPars.push_back(342.6);
	exPars.push_back(-1.437);
	exPars.push_back(-8.942);
	exPars.push_back(17.32);
	}

if (Z == 106){
	exPars.push_back(0.0005902);
	exPars.push_back(-4.611e-07);
	exPars.push_back(0.04672);
	exPars.push_back(3.726);
	exPars.push_back(-0.005411);
	exPars.push_back(1.378);
	exPars.push_back(0.8965);
	exPars.push_back(-0.9379);
	exPars.push_back(-5.028);
	}

if (Z == 107){
	exPars.push_back(0.000599);
	exPars.push_back(-4.668e-07);
	exPars.push_back(0.04748);
	exPars.push_back(3.688);
	exPars.push_back(-0.005429);
	exPars.push_back(1.301);
	exPars.push_back(0.9034);
	exPars.push_back(-0.9097);
	exPars.push_back(-4.998);
	}

if (Z == 108){
	exPars.push_back(0.0006118);
	exPars.push_back(-4.778e-07);
	exPars.push_back(0.04805);
	exPars.push_back(3.637);
	exPars.push_back(-0.005413);
	exPars.push_back(1.228);
	exPars.push_back(0.9089);
	exPars.push_back(-0.8882);
	exPars.push_back(-4.967);
	}

if (Z == 109){
	exPars.push_back(0.0006237);
	exPars.push_back(-4.886e-07);
	exPars.push_back(0.04867);
	exPars.push_back(3.586);
	exPars.push_back(-0.0054);
	exPars.push_back(1.159);
	exPars.push_back(0.9137);
	exPars.push_back(-0.8689);
	exPars.push_back(-4.937);
	}

if (Z == 110){
	exPars.push_back(0.0006349);
	exPars.push_back(-5.008e-07);
	exPars.push_back(0.04943);
	exPars.push_back(3.545);
	exPars.push_back(-0.01493);
	exPars.push_back(8.146);
	exPars.push_back(0.04101);
	exPars.push_back(-37.54);
	exPars.push_back(-0.5238);
	}

if (Z == 111){
	exPars.push_back(0.0006395);
	exPars.push_back(-5.053e-07);
	exPars.push_back(0.05041);
	exPars.push_back(3.512);
	exPars.push_back(-0.004902);
	exPars.push_back(0.6968);
	exPars.push_back(0.3738);
	exPars.push_back(-4.036);
	exPars.push_back(-3.969);
	}

if (Z == 112){
	exPars.push_back(0.0006489);
	exPars.push_back(-5.126e-07);
	exPars.push_back(0.05124);
	exPars.push_back(3.469);
	exPars.push_back(-0.004952);
	exPars.push_back(0.5688);
	exPars.push_back(0.3487);
	exPars.push_back(-4.388);
	exPars.push_back(-3.87);
	}

if (Z == 113){
	exPars.push_back(0.0006642);
	exPars.push_back(-5.299e-07);
	exPars.push_back(0.05186);
	exPars.push_back(3.406);
	exPars.push_back(-0.004987);
	exPars.push_back(0.428);
	exPars.push_back(0.3242);
	exPars.push_back(-4.787);
	exPars.push_back(-3.77);
	}

if (Z == 114){
	exPars.push_back(0.0006738);
	exPars.push_back(-5.425e-07);
	exPars.push_back(0.05238);
	exPars.push_back(3.342);
	exPars.push_back(-0.005066);
	exPars.push_back(0.309);
	exPars.push_back(0.3048);
	exPars.push_back(-5.153);
	exPars.push_back(-3.691);
	}

if (Z == 115){
	exPars.push_back(0.0006908);
	exPars.push_back(-5.646e-07);
	exPars.push_back(0.05294);
	exPars.push_back(3.277);
	exPars.push_back(-0.0172);
	exPars.push_back(8.007);
	exPars.push_back(0.04223);
	exPars.push_back(-36.31);
	exPars.push_back(-0.5273);
	}

if (Z == 116){
	exPars.push_back(0.0007101);
	exPars.push_back(-5.853e-07);
	exPars.push_back(0.05382);
	exPars.push_back(3.224);
	exPars.push_back(-0.01767);
	exPars.push_back(8.011);
	exPars.push_back(0.04346);
	exPars.push_back(-35.2);
	exPars.push_back(-0.5449);
	}

if (Z == 117){
	exPars.push_back(0.0007153);
	exPars.push_back(-5.885e-07);
	exPars.push_back(0.05462);
	exPars.push_back(3.171);
	exPars.push_back(-0.01828);
	exPars.push_back(8.019);
	exPars.push_back(0.04447);
	exPars.push_back(-34.34);
	exPars.push_back(-0.5593);
	}

if (Z == 118){
	exPars.push_back(0.0007287);
	exPars.push_back(-6.006e-07);
	exPars.push_back(0.0554);
	exPars.push_back(3.116);
	exPars.push_back(-0.01895);
	exPars.push_back(7.953);
	exPars.push_back(0.04466);
	exPars.push_back(-34.19);
	exPars.push_back(-0.5555);
	}

if (Z == 119){
	exPars.push_back(0.0007381);
	exPars.push_back(-6.159e-07);
	exPars.push_back(0.0564);
	exPars.push_back(3.069);
	exPars.push_back(-0.02044);
	exPars.push_back(8.176);
	exPars.push_back(0.04809);
	exPars.push_back(-31.79);
	exPars.push_back(43.37);
	}

if (Z == 120){
	exPars.push_back(0.0007513);
	exPars.push_back(-6.362e-07);
	exPars.push_back(0.05728);
	exPars.push_back(3.013);
	exPars.push_back(-0.02097);
	exPars.push_back(8.159);
	exPars.push_back(0.05008);
	exPars.push_back(-30.44);
	exPars.push_back(-0.6434);
	}





}
inline void PotParam(int Zloc, std::vector<double> &Aby,
                     std::vector<double> &Bby) {
  std::vector<double> Aby_loc, Bby_loc;

  Aby.clear();
  Bby.clear();

  double b = 0.;
  if (Zloc < 0) Zloc = -Zloc;  // for Z<0 if beta + transition

  if (Zloc > 92)  // Moliere's potential
  {
    b = 0.88534 * std::pow(Zloc * 1., -1. / 3.);
    Aby_loc.push_back(0.1);
    Aby_loc.push_back(0.55);
    Aby_loc.push_back(0.35);
    Bby_loc.push_back(6.0 / b);
    Bby_loc.push_back(1.2 / b);
    Bby_loc.push_back(0.3 / b);
  }

  else {
    // Screening for H1
    if (Zloc == 1) {
      Aby_loc.push_back(-184.39);
      Aby_loc.push_back(185.39);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(2.0027);
      Bby_loc.push_back(1.9973);
      Bby_loc.push_back(0.0000);
    }

    // Screening for He2
    if (Zloc == 2) {
      Aby_loc.push_back(-0.2259);
      Aby_loc.push_back(1.2259);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(5.5272);
      Bby_loc.push_back(2.3992);
      Bby_loc.push_back(0.0000);
    }

    // Screening for Li3
    if (Zloc == 3) {
      Aby_loc.push_back(0.6045);
      Aby_loc.push_back(0.3955);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(2.8174);
      Bby_loc.push_back(0.6625);
      Bby_loc.push_back(0.0000);
    }

    // Screening for Be4
    if (Zloc == 4) {
      Aby_loc.push_back(0.3278);
      Aby_loc.push_back(0.6722);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(04.5430);
      Bby_loc.push_back(0.9852);
      Bby_loc.push_back(0.0000);
    }

    // Screening for B5
    if (Zloc == 5) {
      Aby_loc.push_back(0.2327);
      Aby_loc.push_back(0.7673);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(5.9900);
      Bby_loc.push_back(1.2135);
      Bby_loc.push_back(0.0000);
    }

    // Screening for C6
    if (Zloc == 6) {
      Aby_loc.push_back(0.1537);
      Aby_loc.push_back(0.8463);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(8.0404);
      Bby_loc.push_back(1.4913);
      Bby_loc.push_back(0.0000);
    }

    // Screening for N7
    if (Zloc == 7) {
      Aby_loc.push_back(0.0996);
      Aby_loc.push_back(0.9004);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(10.812);
      Bby_loc.push_back(1.7687);
      Bby_loc.push_back(0.0000);
    }

    // Screening for O8
    if (Zloc == 8) {
      Aby_loc.push_back(0.0625);
      Aby_loc.push_back(0.9375);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(14.823);
      Bby_loc.push_back(2.0403);
      Bby_loc.push_back(0.0000);
    }

    // Screening for F9
    if (Zloc == 9) {
      Aby_loc.push_back(0.0368);
      Aby_loc.push_back(0.9632);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(21.400);
      Bby_loc.push_back(2.3060);
      Bby_loc.push_back(0.0000);
    }

    // Screening for Ne10
    if (Zloc == 10) {
      Aby_loc.push_back(0.0188);
      Aby_loc.push_back(0.9812);
      Aby_loc.push_back(1 - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(34.999);
      Bby_loc.push_back(2.5662);
      Bby_loc.push_back(0.0000);
    }

    // Screening for Na11
    if (Zloc == 11) {
      Aby_loc.push_back(0.7444);
      Aby_loc.push_back(0.2556);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(4.1205);
      Bby_loc.push_back(0.8718);
      Bby_loc.push_back(0.0000);
    }

    // Screening for Mg12
    if (Zloc == 12) {
      Aby_loc.push_back(0.6423);
      Aby_loc.push_back(0.3577);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(4.7266);
      Bby_loc.push_back(1.0025);
      Bby_loc.push_back(0.0000);
    }

    // Screening for Al13
    if (Zloc == 13) {
      Aby_loc.push_back(0.6002);
      Aby_loc.push_back(0.3998);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(5.1405);
      Bby_loc.push_back(1.0153);
      Bby_loc.push_back(0.0000);
    }

    // Screening for Si14
    if (Zloc == 14) {
      Aby_loc.push_back(0.5160);
      Aby_loc.push_back(0.4840);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(5.8492);
      Bby_loc.push_back(1.1732);
      Bby_loc.push_back(0.0000);
    }

    // Screening for P15
    if (Zloc == 15) {
      Aby_loc.push_back(0.4387);
      Aby_loc.push_back(0.5613);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(6.6707);
      Bby_loc.push_back(1.3410);
      Bby_loc.push_back(0.0000);
    }

    // Screening for S16
    if (Zloc == 16) {
      Aby_loc.push_back(0.5459);
      Aby_loc.push_back(-0.5333);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(6.3703);
      Bby_loc.push_back(2.5517);
      Bby_loc.push_back(1.6753);
    }

    // Screening for Cl17
    if (Zloc == 17) {
      Aby_loc.push_back(0.7249);
      Aby_loc.push_back(-0.7548);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(6.2118);
      Bby_loc.push_back(3.3883);
      Bby_loc.push_back(1.8596);
    }

    // Screening for Ar18
    if (Zloc == 18) {
      Aby_loc.push_back(2.1912);
      Aby_loc.push_back(-2.2852);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(5.5470);
      Bby_loc.push_back(4.5687);
      Bby_loc.push_back(2.0446);
    }

    // Screening for K19
    if (Zloc == 19) {
      Aby_loc.push_back(0.0486);
      Aby_loc.push_back(0.7759);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(30.260);
      Bby_loc.push_back(3.1243);
      Bby_loc.push_back(0.7326);
    }

    // Screening for Ca20
    if (Zloc == 20) {
      Aby_loc.push_back(0.5800);
      Aby_loc.push_back(0.4200);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(6.3218);
      Bby_loc.push_back(1.0094);
      Bby_loc.push_back(0.0000);
    }

    // Screening for Sc21
    if (Zloc == 21) {
      Aby_loc.push_back(0.5543);
      Aby_loc.push_back(0.4457);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(6.6328);
      Bby_loc.push_back(1.1023);
      Bby_loc.push_back(0.0000);
    }

    // Screening for Ti22
    if (Zloc == 22) {
      Aby_loc.push_back(0.0112);
      Aby_loc.push_back(0.6832);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(99.757);
      Bby_loc.push_back(4.1286);
      Bby_loc.push_back(1.0090);
    }

    // Screening for V23
    if (Zloc == 23) {
      Aby_loc.push_back(0.0318);
      Aby_loc.push_back(0.6753);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(42.533);
      Bby_loc.push_back(3.9404);
      Bby_loc.push_back(1.0533);
    }

    // Screening for Cr24
    if (Zloc == 24) {
      Aby_loc.push_back(0.1075);
      Aby_loc.push_back(0.7162);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(18.959);
      Bby_loc.push_back(3.0638);
      Bby_loc.push_back(1.0014);
    }

    // Screening for Mn25
    if (Zloc == 25) {
      Aby_loc.push_back(0.0498);
      Aby_loc.push_back(0.6866);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(31.864);
      Bby_loc.push_back(3.7811);
      Bby_loc.push_back(1.1279);
    }

    // Screening for Fe26
    if (Zloc == 26) {
      Aby_loc.push_back(0.0512);
      Aby_loc.push_back(0.6995);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(31.825);
      Bby_loc.push_back(3.7716);
      Bby_loc.push_back(1.1606);
    }

    // Screening for Co27
    if (Zloc == 27) {
      Aby_loc.push_back(0.0500);
      Aby_loc.push_back(0.7142);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(32.915);
      Bby_loc.push_back(3.7908);
      Bby_loc.push_back(1.1915);
    }
    // Screening for Ni28
    if (Zloc == 28) {
      Aby_loc.push_back(0.0474);
      Aby_loc.push_back(0.7294);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(34.758);
      Bby_loc.push_back(3.8299);
      Bby_loc.push_back(1.2209);
    }

    // Screening for Cu29
    if (Zloc == 29) {
      Aby_loc.push_back(0.0771);
      Aby_loc.push_back(0.7951);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(25.326);
      Bby_loc.push_back(3.3928);
      Bby_loc.push_back(1.1426);
    }

    // Screening for Zlocn30
    if (Zloc == 30) {
      Aby_loc.push_back(0.0400);
      Aby_loc.push_back(0.7590);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(40.343);
      Bby_loc.push_back(3.9465);
      Bby_loc.push_back(1.2759);
    }
    // Screening for Ga31
    if (Zloc == 31) {
      Aby_loc.push_back(0.1083);
      Aby_loc.push_back(0.7489);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(20.192);
      Bby_loc.push_back(3.4733);
      Bby_loc.push_back(1.0064);
    }

    // Screening for Ge32
    if (Zloc == 32) {
      Aby_loc.push_back(0.0610);
      Aby_loc.push_back(0.7157);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(29.200);
      Bby_loc.push_back(4.1252);
      Bby_loc.push_back(1.1845);
    }

    // Screening for As33
    if (Zloc == 33) {
      Aby_loc.push_back(0.0212);
      Aby_loc.push_back(0.6709);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(62.487);
      Bby_loc.push_back(4.9502);
      Bby_loc.push_back(1.3582);
    }

    // Screening for Se34
    if (Zloc == 34) {
      Aby_loc.push_back(0.4836);
      Aby_loc.push_back(0.5164);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(8.7824);
      Bby_loc.push_back(1.6967);
      Bby_loc.push_back(0.0000);
    }

    // Screening for Br35
    if (Zloc == 35) {
      Aby_loc.push_back(0.4504);
      Aby_loc.push_back(0.5496);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(9.3348);
      Bby_loc.push_back(1.7900);
      Bby_loc.push_back(0.0000);
    }

    // Screening for Kr36
    if (Zloc == 36) {
      Aby_loc.push_back(0.4190);
      Aby_loc.push_back(0.5810);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(09.9142);
      Bby_loc.push_back(1.8835);
      Bby_loc.push_back(0.0000);
    }

    // Screening for Rb37
    if (Zloc == 37) {
      Aby_loc.push_back(0.1734);
      Aby_loc.push_back(0.7253);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(17.166);
      Bby_loc.push_back(3.1103);
      Bby_loc.push_back(0.7177);
    }

    // Screening for Sr38
    if (Zloc == 38) {
      Aby_loc.push_back(0.0336);
      Aby_loc.push_back(0.7816);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(55.208);
      Bby_loc.push_back(4.2842);
      Bby_loc.push_back(0.8578);
    }

    // Screening for Y39
    if (Zloc == 39) {
      Aby_loc.push_back(0.0689);
      Aby_loc.push_back(0.7202);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(31.366);
      Bby_loc.push_back(4.2412);
      Bby_loc.push_back(0.9472);
    }

    // Screening for Zlocr40
    if (Zloc == 40) {
      Aby_loc.push_back(0.1176);
      Aby_loc.push_back(0.6581);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(22.054);
      Bby_loc.push_back(4.0325);
      Bby_loc.push_back(1.0181);
    }

    // Screening for Nb41
    if (Zloc == 41) {
      Aby_loc.push_back(0.2257);
      Aby_loc.push_back(0.5821);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(14.240);
      Bby_loc.push_back(2.9702);
      Bby_loc.push_back(1.0170);
    }

    // Screening for Mo42
    if (Zloc == 42) {
      Aby_loc.push_back(0.2693);
      Aby_loc.push_back(0.5763);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(14.044);
      Bby_loc.push_back(2.8611);
      Bby_loc.push_back(1.0591);
    }

    // Screening for Tc43
    if (Zloc == 43) {
      Aby_loc.push_back(0.2201);
      Aby_loc.push_back(0.5618);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(15.918);
      Bby_loc.push_back(3.3672);
      Bby_loc.push_back(1.1548);
    }

    // Screening for Ru44
    if (Zloc == 44) {
      Aby_loc.push_back(0.2751);
      Aby_loc.push_back(0.5943);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(14.314);
      Bby_loc.push_back(2.7370);
      Bby_loc.push_back(1.1092);
    }

    // Screening for Rh45
    if (Zloc == 45) {
      Aby_loc.push_back(0.2711);
      Aby_loc.push_back(0.6119);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(14.654);
      Bby_loc.push_back(2.7183);
      Bby_loc.push_back(1.1234);
    }

    // Screening for Pd46
    if (Zloc == 46) {
      Aby_loc.push_back(0.2784);
      Aby_loc.push_back(0.6067);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(14.645);
      Bby_loc.push_back(2.6155);
      Bby_loc.push_back(1.4318);
    }

    // Screening for Ag47
    if (Zloc == 47) {
      Aby_loc.push_back(0.2562);
      Aby_loc.push_back(0.6505);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(15.5880);
      Bby_loc.push_back(2.7412);
      Bby_loc.push_back(1.1408);
    }
    // Screening for Cd48

    if (Zloc == 48) {
      Aby_loc.push_back(0.2271);
      Aby_loc.push_back(0.6155);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(16.914);
      Bby_loc.push_back(3.0841);
      Bby_loc.push_back(1.2619);
    }

    // Screening for In49
    if (Zloc == 49) {
      Aby_loc.push_back(0.2492);
      Aby_loc.push_back(0.6440);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(16.155);
      Bby_loc.push_back(2.8819);
      Bby_loc.push_back(0.9942);
    }

    // Screening for Sn50
    if (Zloc == 50) {
      Aby_loc.push_back(0.2153);
      Aby_loc.push_back(0.6115);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(17.7930);
      Bby_loc.push_back(3.2937);
      Bby_loc.push_back(1.1478);
    }

    // Screening for Sb51
    if (Zloc == 51) {
      Aby_loc.push_back(0.1806);
      Aby_loc.push_back(0.5767);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(19.875);
      Bby_loc.push_back(3.8092);
      Bby_loc.push_back(1.2829);
    }

    // Screening for Te52
    if (Zloc == 52) {
      Aby_loc.push_back(0.1308);
      Aby_loc.push_back(0.5504);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(24.154);
      Bby_loc.push_back(4.6119);
      Bby_loc.push_back(1.4195);
    }

    // Screening for I53
    if (Zloc == 53) {
      Aby_loc.push_back(0.0588);
      Aby_loc.push_back(0.5482);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(39.996);
      Bby_loc.push_back(5.9132);
      Bby_loc.push_back(1.5471);
    }

    // Screening for Xe54
    if (Zloc == 54) {
      Aby_loc.push_back(0.4451);
      Aby_loc.push_back(0.5549);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(11.8050);
      Bby_loc.push_back(1.7967);
      Bby_loc.push_back(0.0000);
    }

    // Screening for Cs55
    if (Zloc == 55) {
      Aby_loc.push_back(0.2708);
      Aby_loc.push_back(0.6524);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(16.591);
      Bby_loc.push_back(2.6964);
      Bby_loc.push_back(0.6814);
    }

    // Screening for Ba56
    if (Zloc == 56) {
      Aby_loc.push_back(0.1728);
      Aby_loc.push_back(0.6845);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(22.397);
      Bby_loc.push_back(3.4595);
      Bby_loc.push_back(0.8073);
    }

    // Screening for La57
    if (Zloc == 57) {
      Aby_loc.push_back(0.1947);
      Aby_loc.push_back(0.6384);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(20.764);
      Bby_loc.push_back(3.4657);
      Bby_loc.push_back(0.8911);
    }

    // Screening for Ce58
    if (Zloc == 58) {
      Aby_loc.push_back(0.1913);
      Aby_loc.push_back(0.6467);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(21.235);
      Bby_loc.push_back(3.4819);
      Bby_loc.push_back(0.9011);
    }

    // Screening for Pr59
    if (Zloc == 59) {
      Aby_loc.push_back(0.1868);
      Aby_loc.push_back(0.6558);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(21.803);
      Bby_loc.push_back(3.5098);
      Bby_loc.push_back(0.9106);
    }

    // Screening for Nd60
    if (Zloc == 60) {
      Aby_loc.push_back(0.1665);
      Aby_loc.push_back(0.7057);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(23.949);
      Bby_loc.push_back(3.5199);
      Bby_loc.push_back(0.8486);
    }

    // Screening for Pm61
    if (Zloc == 61) {
      Aby_loc.push_back(0.1624);
      Aby_loc.push_back(0.7133);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(24.598);
      Bby_loc.push_back(3.5560);
      Bby_loc.push_back(0.8569);
    }

    // Screening for Sm62
    if (Zloc == 62) {
      Aby_loc.push_back(0.1580);
      Aby_loc.push_back(0.7210);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(25.297);
      Bby_loc.push_back(3.5963);
      Bby_loc.push_back(0.8650);
    }

    // Screening for Eu63
    if (Zloc == 63) {
      Aby_loc.push_back(0.1538);
      Aby_loc.push_back(0.7284);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(26.017);
      Bby_loc.push_back(3.6383);
      Bby_loc.push_back(0.8731);
    }

    // Screening for Gd64
    if (Zloc == 64) {
      Aby_loc.push_back(0.1587);
      Aby_loc.push_back(0.7024);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(25.497);
      Bby_loc.push_back(3.7364);
      Bby_loc.push_back(0.9550);
    }

    // Screening for Tb65
    if (Zloc == 65) {
      Aby_loc.push_back(0.1453);
      Aby_loc.push_back(0.7426);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(27.547);
      Bby_loc.push_back(3.7288);
      Bby_loc.push_back(0.8890);
    }

    // Screening for Dy66
    if (Zloc == 66) {
      Aby_loc.push_back(0.1413);
      Aby_loc.push_back(0.7494);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(28.346);
      Bby_loc.push_back(3.7763);
      Bby_loc.push_back(0.8969);
    }

    // Screening for Ho67
    if (Zloc == 67) {
      Aby_loc.push_back(0.1374);
      Aby_loc.push_back(0.7558);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(29.160);
      Bby_loc.push_back(3.8244);
      Bby_loc.push_back(0.9048);
    }

    // Screening for Er68
    if (Zloc == 68) {
      Aby_loc.push_back(0.1336);
      Aby_loc.push_back(0.7619);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(29.990);
      Bby_loc.push_back(3.8734);
      Bby_loc.push_back(0.9128);
    }

    // Screening for Tm69
    if (Zloc == 69) {
      Aby_loc.push_back(0.1299);
      Aby_loc.push_back(0.7680);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(30.835);
      Bby_loc.push_back(3.9233);
      Bby_loc.push_back(0.9203);
    }

    // Screening for Yb70
    if (Zloc == 70) {
      Aby_loc.push_back(0.1267);
      Aby_loc.push_back(0.7734);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(31.681);
      Bby_loc.push_back(3.9727);
      Bby_loc.push_back(0.9288);
    }

    // Screening for Lu71
    if (Zloc == 71) {
      Aby_loc.push_back(0.1288);
      Aby_loc.push_back(0.7528);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(31.353);
      Bby_loc.push_back(4.0904);
      Bby_loc.push_back(1.0072);
    }

    // Screening for Hf72
    if (Zloc == 72) {
      Aby_loc.push_back(0.1303);
      Aby_loc.push_back(0.7324);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(31.217);
      Bby_loc.push_back(4.2049);
      Bby_loc.push_back(1.0946);
    }

    // Screening for Ta73
    if (Zloc == 73) {
      Aby_loc.push_back(0.1384);
      Aby_loc.push_back(0.7096);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(30.077);
      Bby_loc.push_back(4.2492);
      Bby_loc.push_back(1.1697);
    }

    // Screening for W74
    if (Zloc == 74) {
      Aby_loc.push_back(0.1500);
      Aby_loc.push_back(0.6871);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(28.630);
      Bby_loc.push_back(4.2426);
      Bby_loc.push_back(1.2340);
    }

    // Screening for Re75
    if (Zloc == 75) {
      Aby_loc.push_back(0.1608);
      Aby_loc.push_back(0.6659);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(27.568);
      Bby_loc.push_back(4.2341);
      Bby_loc.push_back(1.2970);
    }

    // Screening for Os76
    if (Zloc == 76) {
      Aby_loc.push_back(0.1722);
      Aby_loc.push_back(0.6468);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(26.586);
      Bby_loc.push_back(4.1999);
      Bby_loc.push_back(1.3535);
    }

    // Screening for Ir77
    if (Zloc == 77) {
      Aby_loc.push_back(0.1834);
      Aby_loc.push_back(0.6306);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(25.734);
      Bby_loc.push_back(4.1462);
      Bby_loc.push_back(1.4037);
    }

    // Screening for Pt78
    if (Zloc == 78) {
      Aby_loc.push_back(0.2230);
      Aby_loc.push_back(0.6176);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(22.994);
      Bby_loc.push_back(3.7346);
      Bby_loc.push_back(1.4428);
    }

    // Screening for Au79
    if (Zloc == 79) {
      Aby_loc.push_back(0.2289);
      Aby_loc.push_back(0.6114);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(22.864);
      Bby_loc.push_back(3.6914);
      Bby_loc.push_back(1.4886);
    }

    // Screening for Hg80
    if (Zloc == 80) {
      Aby_loc.push_back(0.2098);
      Aby_loc.push_back(0.6004);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(24.408);
      Bby_loc.push_back(3.9643);
      Bby_loc.push_back(1.5343);
    }

    // Screening for Tl81
    if (Zloc == 81) {
      Aby_loc.push_back(0.2708);
      Aby_loc.push_back(0.6428);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(20.941);
      Bby_loc.push_back(3.2456);
      Bby_loc.push_back(1.1121);
    }

    // Screening for Pb82
    if (Zloc == 82) {
      Aby_loc.push_back(0.2380);
      Aby_loc.push_back(0.6308);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(22.987);
      Bby_loc.push_back(3.6217);
      Bby_loc.push_back(1.2373);
    }

    // Screening for Bi83
    if (Zloc == 83) {
      Aby_loc.push_back(0.2288);
      Aby_loc.push_back(0.6220);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(23.792);
      Bby_loc.push_back(3.7796);
      Bby_loc.push_back(1.2534);
    }

    // Screening for Po84
    if (Zloc == 84) {
      Aby_loc.push_back(0.1941);
      Aby_loc.push_back(0.6105);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(26.695);
      Bby_loc.push_back(4.2582);
      Bby_loc.push_back(1.3577);
    }

    // Screening for At85
    if (Zloc == 85) {
      Aby_loc.push_back(0.1500);
      Aby_loc.push_back(0.6031);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(31.840);
      Bby_loc.push_back(4.9285);
      Bby_loc.push_back(1.4683);
    }

    // Screening for Rn86
    if (Zloc == 86) {
      Aby_loc.push_back(0.0955);
      Aby_loc.push_back(0.6060);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(43.489);
      Bby_loc.push_back(5.8520);
      Bby_loc.push_back(1.5736);
    }

    // Screening for Fr87
    if (Zloc == 87) {
      Aby_loc.push_back(0.3192);
      Aby_loc.push_back(0.6233);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(20.015);
      Bby_loc.push_back(2.9091);
      Bby_loc.push_back(0.7207);
    }

    // Screening for Ra88
    if (Zloc == 88) {
      Aby_loc.push_back(0.2404);
      Aby_loc.push_back(0.6567);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(24.501);
      Bby_loc.push_back(3.5524);
      Bby_loc.push_back(0.8376);
    }

    // Screening for Ac89
    if (Zloc == 89) {
      Aby_loc.push_back(0.2266);
      Aby_loc.push_back(0.6422);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(25.684);
      Bby_loc.push_back(3.7922);
      Bby_loc.push_back(0.9335);
    }

    // Screening for Th90
    if (Zloc == 90) {
      Aby_loc.push_back(0.2176);
      Aby_loc.push_back(0.6240);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(26.554);
      Bby_loc.push_back(4.0044);
      Bby_loc.push_back(1.0238);
    }

    // Screening for Pa91
    if (Zloc == 91) {
      Aby_loc.push_back(0.2413);
      Aby_loc.push_back(0.6304);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(25.193);
      Bby_loc.push_back(3.6780);
      Bby_loc.push_back(0.9699);
    }

    // Screening for U92
    if (Zloc == 92) {
      Aby_loc.push_back(0.2448);
      Aby_loc.push_back(0.6298);
      Aby_loc.push_back(1. - (Aby_loc[0] + Aby_loc[1]));
      Bby_loc.push_back(25.252);
      Bby_loc.push_back(3.6397);
      Bby_loc.push_back(0.9825);
    }
  }

  // Parameters are in atomic units, thus conversion in natural units
  for (int i = 0; i < (int)Bby_loc.size(); i++) Bby_loc[i] = Bby_loc[i] * FINESTRUCTURE;

  // Parameters are put in the global std::vectors for the required nucleus
  for (int i = 0; i < (int)Aby_loc.size(); i++) {
    Aby.push_back(Aby_loc[i]);
    Bby.push_back(Bby_loc[i]);
  }

  Aby_loc.clear();
  Bby_loc.clear();
}
}
}
#endif
