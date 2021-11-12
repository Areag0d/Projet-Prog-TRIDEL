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

//This function takes in argument the massic proportions of the compostion of
//machefer. It is assumed to be composed of SiO2, Al2O3, CaO, Fe2O3, C and Cl.
//Knowing relative proportions of components of the inert part of waste,
//we can calculate its specific heat, which is essential for downstream calculus
//We neglect the contribution of trace elements, as their proportion is insignificant.
double Cm_Inert(double propSiO2, double propAl2O3, double propCaO, double propFe2O3, double propC, double propCl){
  //Except for SiO2, which is glass, Carbon and Chlorine, all the other components
  //are in their oxidized form, which means there were burnt.
  //therefore to calculate their proportions in incoming waste,
  //we need to calculate their proportions before oxidation (metallic)

  //for one gram of Machefer
  //Glass (SiO2)
  double mSiO2 = propSiO2 * 1; //[g]

  //Carbon
  double mC = propC * 1; //[g]

  //Chlorine
  double mCl = propCl * 1; //[g]

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
  double mInitialMix = mSiO2 + mAl + mFe + mCa + mC + mCl;

  //now we can determine the relative proportions of metals
  //in the inert part of waste by normalizing each value

  double massTable [] = {mSiO2, mAl, mFe, mCa, mC, mCl};
  for (int i = 0; i < 6; i ++){
    massTable[i] = massTable[i] / mInitialMix;
  }

  //Now that we have relative proportions, we can finally approximate
  //its specific heat value by calculating the average of each heat capacity
  //weighted by its importance (proportion) in the inert part

  //we give specific heat values (tabulated) of each component [J/(g*K)]:

  double  CmTable [] = {0.84, 0.894, 0.412, 0.63, 0.710, 0.48};

  //weighted average

  double CmInert = 0;
  for (int i = 0; i < 6; i ++){
    CmInert += massTable[i] * CmTable[i];
  }

  return CmInert;
}

int main(int argc, char * argv[]) {

double a = Cm_Inert(0.56, 0.10, 0.14, 7.5, 1.8, 1.5);

printf("%f", a);

  return 0;
}

//Need to either compute propSiO2 and the others
//either just make the masses directly saving 6 lines of code
//From line 123 to 142 a lot better given the addition of the 
//Carbon and Chlorine to the system..

