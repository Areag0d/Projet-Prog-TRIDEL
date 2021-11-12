#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
/* This is the prog project about the incineration station: TRIDEL
In this part we're going to read the csv file produced by Part 1
and process it according to incineration equations:
that way we obtain an energy (and maybe other products) output*/

// CSV file lecture
/* No use of read_line at the moment
//read a line of only one column (must modify for several lines)
// use strchr for several columns: check GPS exercise for template
void read_line(char * line) {

}
*/
//read whole file
void read_csv(char * filename, double * table) {
  FILE * file = fopen(filename, "r");

  // verification
  if (file == NULL) {
    printf("Data file does not exist (or isn't accessible).\n");
    }

  // read line by line
  char line[100]; // maybe we need to make it bigger if we add columns
  int i = 0;
  while(fgets(line, 100, file) != NULL) {
    double value = atof(line);
    table[i] = value;
    i += 1;
    //printf("value : %0.2f", table [i]);
    }
  fclose(file);
}

//Le point 2: creer une fonction qui calcule les differents Q//

double Qcalculator(double m, double Cm, double T2, double T1) {

  double deltaT = T2 - T1;
  double Q = m * Cm * deltaT;

  return Q;
}

//Calculating the mass of the moisture in the waste//

double mMoist(double moistProportion, double waste, double inertProportion){

  double mMoist = waste * moistProportion;
  return mMoist;

}

//Calculating the mass of the inert part of the waste //

double mInert(double inertproportion, double waste, double inertProportion){

  double mInert  = waste * inertProportion;
  return mInert;

}

//This function calculates the relative mass of an element before a reaction,
//in our case, an oxidation reaction (which occurs during the burning of waste)
//So from the tabulated proportions of oxidized metals in Machefer, we
//calculate the relative proportions of pure metals in the inert part of waste.
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
//machefer. It is assumed to be composed of SiO2, Al2O3, CaO, Fe2O3 and trace elements.
//Knowing relative proportions of components of the inert part of waste,
//we can calculate its specific heat, which is essential for downstream calculus
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
  double mFe2O3 = compFe2O3 * 1; //[g]
  double mFe = OriginalMass(mFe2O3, 159.6, 55.8, 2);

  //Calcium
  //Tabulated Molar Weight of CaO = 56 [g/mol], Ca = 40 [g/mol]
  //Oxidation reaction: Ca + O --> CaO
  double mCaO = compCaO * 1; //[g]
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
  //importing data from csv file into a table
  // create a recieving table for data of dimension 365 * years
  //int years = 1;
  double wasteDayTable[365 + 1];

  // calling the csv reader to produce wasteDayTable
  read_csv("wasteDayLst.csv", wasteDayTable);
  /*// printing the table to visualise
  for (int i = 0; i < 365; i++) {
    printf("line %d : %0.2f kg\n", i, wasteDayTable[i]);
  }*/

 //Point 2 du rapport: Q comb//

  //Starting from the global equation that gives the total heat required to evaporate moisture and heat up waste
  //Qair = Qwaste + Qeva + Qsteam = (Qcomb + Qinert + Qmoist) + Qeva + Qsteam
  //And noting that Q = m * Cm * ΔT, unless it is for latent heat where Q = m * Cm, we have the following equations:

  double mcomb = mwaste * comb_proportion;
  double CmC2H4x = 1.9;
  double ΔTignition = 350 - 20;
  double QcombT = mcomb * CmC2H4x * ΔTignition;
 //double QcombT = Qcalculator() so can use this function to implement the diofferent Q's

  double Cmfus = 230;
  double QcombFusion =  Cmfus * mcomb;
  double Qcomb = QcombT + QcombFusion;



  double Qinert =  minert * Cminert * ΔTignition;


  double mmoist = mwaste * moist_proportion;
  double Cmwater = 4184;
  double Qmoist =  mmoist * Cmwater * (100-20);

  double Qwaste = Qcomb + Qinert + Qmoist;


  double Cvap = 2257;
  double Qeva =  Cvap * mmoist;

  double Csteam = 2000;
  double Qsteam = Csteam * mmoist;

  double Qair = Qwaste + Qeva + Qsteam;
  //Qair is the total energy input used to start the combustion reaction








  return 0;
}
