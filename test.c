#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

double OriginalMass(double massOxy, double MWOxy, double MWPure, double StoichCoefficient){
    //we first calculate the final number of moles
    double nbMolOxy  = massOxy / MWOxy;
    //we calculate the inital number of moles, according to the oxidation reaction
    double nbMolPure = StoichCoefficient  * nbMolOxy;
    //we get the initial mass of metal, before oxidation
    double PureMass = nbMolPure * MWPure;
    return PureMass;
}



double Cm_Inert(double propSiO2, double propAl2O3, double propCaO, double propFe2O3){
  //we group all trac elements under an only category: others.
  //we obtain the proportion of trace elements with a substraction
  double propOthers = 100 - (propSiO2 + propAl2O3 + propCaO + propFe2O3);

  //Except for SiO2, which is glass, and trace elements, all the other components
  //are in their oxidized form, which means there were burnt.
  //therefore to calculate their proportions in incoming waste,
  //we need to calculate their proportions before oxidation (metallic)

  //for one gram of Machefer
  //Glass (SiO2)
  double mSiO2 = propSiO2 * 1; //[g]

  //Others
  double mOthers = propOthers + 1; //[g]

  //Aluminium
  //Tabulated Molar Weight of Al2O3 = 102 [g/mol], Al = 27 [g/mol]
  //Oxidation reaction: 2Al + 3O --> Al2O3
  double mAl2O3 = propAl2O3 * 1; //[g]
  double mAl = OriginalMass(mAl2O3, 102, 27, 2);

  //Iron
  //Tabulated Molar Weight of Fe2O3 = 159.6 [g/mol],  Fe = 55.8 [g/mol]
  //Oxidation reaction: 2Fe + 3O --> Fe2O3
  double mFe2O3 = propFe2O3 * 1; //[g]
  double mFe = OriginalMass(mFe2O3, 159.6, 55.8, 2);

  //Calcium
  //Tabulated Molar Weight of CaO = 56 [g/mol], Ca = 40 [g/mol]
  //Oxidation reaction: Ca + O --> CaO
  double mCaO = propCaO * 1; //[g]
  double mCa = OriginalMass(mCaO, 56, 40, 1);

  //initial mass of mixed metals that produced Machefer during combustion
  double mInitialMix = mSiO2 + mAl + mFe + mCa + mOthers;

  //now we can determine the relative proportions of metals
  //in the inert part of waste by normalizing each value
  double inipropSiO2 = mSiO2 / mInitialMix;
  double inipropAl = mAl / mInitialMix;
  double inipropFe = mFe / mInitialMix;
  double inipropCa = mCa / mInitialMix;
  double inipropOthers = mOthers / mInitialMix;

  //Now that we have relative proportions, we can finally approximate
  //its specific heat value by calculating the average of each heat capacity
  //weighted by its importance (proportion) in the inert part

  //we give specific heat values (tabulated) of each component [J/(g*K)]:
  double CmSiO2 = 0.84;
  double CmAl = 0.894;
  double CmFe = 0.412;
  double CmCa = 0.63;
  double CmOthers = (0.710 + 0.48) / 2; //The average between the specific heat for Carbon, and that for Chlorine

  //weighted average
  double CmInert = (CmSiO2 * inipropSiO2) + (CmAl * inipropAl) + (CmFe * inipropFe) + (CmCa * inipropCa) + (CmOthers * inipropOthers);

  return CmInert;
}

int main(int argc, char * argv[]) {

double a = Cm_Inert(0.56, 0.10, 0.14, 7.5);

printf("%f", a);

  return 0;
}
