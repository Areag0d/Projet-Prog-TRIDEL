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


//Part 1: waste composition

//This function calculates the relative mass of an element before a reaction,
//in our case, an oxidation reaction (which occurs during the burning of waste)
//So from the tabulated proportions of oxidized metals in Machefer, we
//calculate the relative proportions of pure metals in the inert part of waste,
//we proceed similarly in the calculation of CO2 and H2O produced from burning polyethylene in the waste.
double OriginalMass(double mass1, double MW1, double MW2, double StoichCoefficient){

    //we first calculate the final number of moles
    double nbMol1  = mass1 / MW1;
    //we calculate the inital number of moles, according to the oxidation reaction

    double nbMol2 = StoichCoefficient  * nbMol1;
    //we get the initial mass of metal/gas, before oxidation/combustion
    double Mass2 = nbMol2 * MW2;

    return Mass2;
}




//This function takes in argument the massic proportions of the compostion of
//machefer. It is assumed to be composed of SiO2, Al2O3, CaO, Fe2O3, C and Cl.
//Knowing relative proportions of components of the inert part of waste,
//we can calculate its specific heat, which is essential for downstream calculus
//We neglect the contribution of trace elements, as their proportion is insignificant.
double CmInert(double propSiO2, double propAl2O3, double propCaO, double propFe2O3, double propC, double propCl){
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


  double relmassTable [] = {mSiO2, mAl, mFe, mCa, mC, mCl};
  for (int i = 0; i < 6; i ++) relmassTable[i] /= mInitialMix;

  //Now that we have relative proportions, we can finally approximate
  //its specific heat value by calculating the average of each heat capacity
  //weighted by its importance (proportion) in the inert part

  //we give specific heat values (tabulated) of each component:
  //SiO2, Al, Fe, Ca, C, Cl respectively [J/(g*K)]:

  double  CmTable [] = {0.84, 0.894, 0.412, 0.63, 0.710, 0.48};

  //weighted average given in

  double CmInert = 0;
  for (int i = 0; i < 6; i ++) CmInert += relmassTable[i] * CmTable[i];

  return CmInert; //[J/(g*K)]

}


//Part 2: energy required to heat up waste to ignition

double Qcalculator(double m, double Cm, double Tfinal, double Tinitial) {

  double deltaT = Tfinal -Tinitial;
  double Qh = m * Cm * deltaT;

  return Qh;
}
 //we define Tignition for PE to be 350°C
 //we define it as a global variable, because we use it in different
 //functions multiple times
double Tignition = 350;

double Qignition(double mC2H4, double mMoist, double mInert){
  //Starting from the global equation that gives the total heat required to evaporate moisture and heat up waste
  //Qignition = Qwaste + Qeva + Qsteam = (QC2H4 + Qinert + Qmoist) + Qeva + Qsteam
  //And noting that Q = m * Cm * ΔT, unless it is for latent heat where Q = m * Cm,
  //we have the following equations:

  //QC2H4
  double CmC2H4x = 2.25; //[KJ/Kg/K]
  double Tinitial = 20; // T°C at which waste enters combustion room
  double QC2H4T = Qcalculator(mC2H4, CmC2H4x, Tignition, Tinitial);

  double Cmfus = 230;//[kJ/Kg]
  double QC2H4Fusion =  Cmfus * mC2H4;
  double QC2H4 = QC2H4T + QC2H4Fusion;

  //Qinert
  double Cminert = CmInert(0.56, 0.10, 0.14, 7.5, 1.8, 1.5);
  double Qinert = Qcalculator(mInert, Cminert, Tignition, Tinitial);

  //Qmoist
  double CmWater = 4.184; //[kJ/kg]
  double Tboiling = 100;
  double Qmoist = Qcalculator(mMoist, CmWater, Tboiling, Tinitial);

  double Qwaste = QC2H4 + Qinert + Qmoist;

  //Qeva
  double Cvap = 2257; //[kJ/kg]
  double Qeva =  Cvap * mMoist;

  //Qsteam
  double Csteam = 2; //[kJ/kg]
  double Qsteam = Csteam * mMoist;

  double Qignition = Qwaste + Qeva + Qsteam;
  //Qair is the total energy input used to start the combustion reaction

  return Qignition;
}


//Part 3: heat released by waste combustion

double TfinalCalculator(double mC2H4, double mMoist, double mInert, double massMoyC2H4){

  //We assume the combustible part of waste is Polyethylene (PE)
  double QcC2H4x = 47000; //[kJ/kg] tabulated value
  double Qheat = QcC2H4x * mC2H4; //[KJ]

  double Qignit = Qignition(mC2H4, mMoist, mInert);//mCombustible = mC2H4
  double Qnet = Qheat - Qignit;

  //double mCO2 = OriginalMass(mC2H4, 28, 44, 2);//massC2H4 est apporte dans le tableau dans le main
  //double mH2O = OriginalMass(mC2H4, 28, 18, 2);



  //double CpCO2 = 0.849;	//[kJ/kgK], tabulated value
  //double CpH2O = 1.996; //[kJ/kgK ], tabulated


  double MWC2H4 = 28;
  double nC2H4 = mC2H4 / MWC2H4;
  double nC2H4Moy = massMoyC2H4 / MWC2H4;
  double R = 8.314;
  double P = 1;


  double MWC2H4 = 28;
  double nC2H4 = mC2H4 / MWC2H4;
  double nC2H4Moy = massMoyC2H4 / MWC2H4;
  double R = 8.314; // J / (K * mol)
  double P = 1;

  double MWN2 = 28;
  double MWO2 = 32;


  return Tfinal;


}


//Part 4 : energy harvesting

//We're calculating the energy flow according to this equation:
  // Qflow = k * A * LMTD
  //Where Q = Energy flow, k = heat transfer coefficient,
  //A = heat transfer area, LMTD = logarithmic Mean Temperature Difference

double QdotCalculator(double mC2H4, double mMoist, double mInert, double massMoyC2H4){

  double lambda = 45; //λ = thermal conductivity, [W/(mK)] (=45 W/(mK) making the assumption that it is only made of steel
  double thickness = 0.00833;//plate thickness of the heat exchanger [m]
  double alphaHot = 3500; //mean of the tabulated values
  double alphaCold = 120; //mean of the tabulated values
  double k = 1/ ((1/alphaHot) + (thickness/lambda) + (1/alphaCold)); //heat transfer coefficient
  //printf("%F\n", k);
  //LMTD: Logarithmic Mean Temperature Difference
  double Tfinal = TfinalCalculator(mC2H4, mMoist, mInert, massMoyC2H4);
  double ThotIn = Tfinal;
  double ThotOut = 0.7 * ThotIn;
  double TcoldIn = 30;
  double TcoldOut = 570;

  double dA = ThotIn - TcoldIn;
  double dB = ThotOut - TcoldOut;

  double LMTD = (dA - dB)/ log(dA / dB);
  printf("%F\n", LMTD);
  double A = 11490; //[m^2]

  //Energy flow
  double Qdot = k * A * LMTD;

  return Qdot;
}


double WdotCalculator(double mC2H4, double mMoist, double mInert, double massMoyC2H4){
  //we know that Wdot = mdot * deltaH
  //we can calculte mdot with mdot = Qdot / (CmSteam * dT)
  double Qdot = QdotCalculator(mC2H4, mMoist, mInert, massMoyC2H4); //[J/s]
  double CmSteam = 2000; //[J/kg/K]
  double dT = 570 - 30; //[K]
  double mdot = Qdot / (CmSteam * dT); //[kg/s]
  //delaH = Hi - Hf, which are given in tables for our specific temperatures
  //The specific tempeartures were chosen upon what is common in litterture
  //for steam engines.
  //At 570°C and 10 bars, Hi = 3654 [kJ/kg]
  //At 77°C and 2 bars, Hf = 293 [kJ/kg]
  double Hf = 3654, Hi = 314;
  double deltaH = Hf - Hi;
  double Wdot = mdot * deltaH;

  return Wdot;
}




int main(int argc, char * argv[]) {
  //importing data from csv file into a table
  // create a recieving table for data of dimension 365 * years
  //int years = 1;
  double wasteDayTable[365];

  // calling the csv reader to produce wasteDayTable
  read_csv("wasteDayLst.csv", wasteDayTable);
  /*// printing the table to visualise
  for (int i = 0; i < 365; i++) {
    printf("line %d : %0.2f kg\n", i, wasteDayTable[i]);
  }*/
  //Part 1: waste composition

  //From the table of waste mass per day we have imported from CSV,
  //we calculate the proportions of combustible, moisture and inert parts
  //Hence we create a table for each component as it is the most convenient

  double moistProportion = 0.195; //19.5 %, average of the  two bounds of the interval
  double inertProportion = 0.15;
  double C2H4Proportion = 1 - moistProportion - inertProportion;

  double mC2H4Table[365];
  double mMoistTable[365];
  double mInertTable[365];

  for (int day = 0; day < 365; day++) {
    mC2H4Table[day] = C2H4Proportion * wasteDayTable[day];
    mMoistTable[day] = moistProportion * wasteDayTable[day];
    mInertTable[day] = inertProportion * wasteDayTable[day];
  }

  //Part 2: energy required to heat up waste to ignition

  //This is implemented in Qignition function, which is used
  //in the next part

  //Part 3: heat released by waste combustion and final air temperature

  //We implemented the function TfinalCalculator which calculates
  //the heat released by the combustion, substracts to it the heat
  //needed to heat up the waste, and outputs the final temperature

  //Part 4: energy transformation

  //We implement QdotCalculator to model the heat exchanger: the energy flow
  //going from the combustion flue gas and the water(steam) that will
  //generate energy afterwards

  //Part 5: energy harvesting
  //To determine the work applied on the turbine, we model the heat engine
  //by a Rankine cycle, which is common for electricity generation from
  //steam turbines. To do so, we implemented WdotCalculator function.

  //Part 6: final energy output
  //We iterate our final function (WdotCalculator) on all the entries
  //of our table and fill up our output table

  double massMoyC2H4 = 0;
  for (int day = 0; day < 365; day++) massMoyC2H4 += mC2H4Table[day];
  massMoyC2H4 /= 365;


  double WdotTable[365];
   for (int day = 0; day < 365; day++){
    WdotTable[day] = WdotCalculator(mC2H4Table[day], mMoistTable[day], mInertTable[day], massMoyC2H4);
    double Tfinal = TfinalCalculator(mC2H4Table[day], mMoistTable[day], mInertTable[day], massMoyC2H4);
    double Qdot = QdotCalculator(mC2H4Table[day], mMoistTable[day], mInertTable[day], massMoyC2H4);
    //printf("%f\n", Tfinal);
    //printf("%f\n", Qdot);
  }
  //debugging





  return 0;
}
