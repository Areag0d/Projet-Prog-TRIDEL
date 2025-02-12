#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/* This is the prog project about the incineration station: TRIDEL
In this part we're going to read the CSV file produced by Part 1
and process it according to incineration equations:
that way we obtain an energy output*/


/* In our main function we will put the general backbone
of the functions along with a commentary of the different
steps that will lead us to the final output calculation,
and each function that goes along with each of these steps
*/

// So we start by defining all the functions before the main

//Importing the WasteDay table created in python to start
void read_csv(char * filename, double * table);

//Part 1
double OriginalMass(double mass1, double MW1, double MW2, double StoichCoefficient);
struct Masses;
struct Masses massesStructBuilder(double propSiO2, double propAl2O3, double propCaO, double propFe2O3, double propC, double propCl);
double CpInert(double propSiO2, double propAl2O3, double propCaO, double propFe2O3, double propC, double propCl);

//Part 2
double Qcalculator(double m, double Cp, double Tfinal, double Tinitial);
double Qignition(double mC2H4, double mMoist, double mInert);

//Part 3
double QMetalOxi(double mInert, double propSiO2, double propAl2O3, double propCaO, double propFe2O3, double propC, double propCl);
double TfinalCalculator(double mC2H4, double mMoist, double mInert, double massMoyC2H4);

//Part 4
double QdotCalculator(double mC2H4, double mMoist, double mInert, double massMoyC2H4);

//Part 5
double WdotCalculator(double mC2H4, double mMoist, double mInert, double massMoyC2H4);

//(Part 6: in the main)

// Part 7: creating a new table adding variance and needed Fuel if the energetic output is negative
void stochastiser(double value, double * PowerVarTable, double * negativeOutputSum);

// Part 8: creating a CSV writer
void write_csv(char * filename, double * table);



int main(int argc, char * argv[]) {

  // importing data from the csv file we create in python into a table
  // creating a recieving table for data of dimension 365 * years
  // int years = 1;
  double wasteDayTable[365];

  // calling the csv reader to produce wasteDayTable
  read_csv("wasteDayLst.csv", wasteDayTable);

  // Part 1: waste composition

  // From the table of waste mass per day we have imported from the CSV file,
  // we calculate the proportions of combustible, moisture and inert parts in the waste.
  // Hence we create a table for each component as it is the most convenient

  double moistProportion = 0.195; // 19.5 %, average of the tabulated interval.
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

  // Part 2: energy required to heat up waste to ignition

  // This is implemented in Qignition function, which is used
  // in the next part

  // Part 3: Final air temperature

  // We implemented the function TfinalCalculator which calculates
  // the heat released by the combustion, substracts from it the heat
  // needed to heat up the waste, and outputs the final temperature
  // 3.1 : heat released by PE combustion
  // 3.2 : finding composition of final flue gas
  // 3.3 : final temperature

  // Part 4: Heat exchanger

  // We implement QdotCalculator to model the heat exchanger: the energy flow
  // going from the combustion flue gas and the water(steam) that will
  // generate energy afterwards

  // Part 5: Energy harvesting

  // To determine the work applied on the turbine, we model the heat engine
  // by a Rankine cycle, which is common for electricity generation from
  // steam turbines. To do so, we implemented WdotCalculator function.

  // Part 6: final energy output

  // We iterate our final function (WdotCalculator) on all the entries
  // of our table and fill up our energetic output table

  double massMoyC2H4 = 0;
  for (int day = 0; day < 365; day++) massMoyC2H4 += mC2H4Table[day];
  massMoyC2H4 /= 365;

  // We convert our kJ per day in kW:
  // W = J/s --> kJ / s = kW. So we divide again by 1000 to get MegaWatts:
  // PowerOutput = W / (3600 * 1000) [MW]
  double PowerTable[365];
  double WorkOutput[365];
  for (int day = 0; day < 365; day++){

    WorkOutput[day] = WdotCalculator(mC2H4Table[day], mMoistTable[day], mInertTable[day], massMoyC2H4);
    PowerTable[day] = WorkOutput[day] / (3600*24*1000); // [MW]
  }

  // Part 7

  //Implementing a new table of values given by calculated outputs
  // added with variance.
  // Because our mean (calculated) value is close to 1 MW, and our choice of
  // sigma = 1.25, (based on official values), we obtain negative energy outputs
  //from our model with the added variance.
  // These negatives values are deficits in energy production,
  // hence we calculate how much minimum fuel is needed in order to
  // conduct complete combustion and hence, achieve a net energy ouput of zero.

  double VarPowerTable[365];
  double FuelNeededDay[365];

  for (int day = 0; day < 365; day++){
    stochastiser(PowerTable[day], &VarPowerTable[day], &FuelNeededDay[day]);
  }

  // Part 8: Outputing CSV files

  // CSV file for PowerTable
  write_csv("PowerTable.csv", PowerTable);
  // CSV file for varPowerTable
  write_csv("VarPowerTable.csv", VarPowerTable);
  // CSV file for negative outputs
  write_csv("FuelNeededDay.csv", FuelNeededDay);
  return 0;
}



// CSV file lecture
void read_csv(char * filename, double * table) {
  FILE * file = fopen(filename, "r");

  // Verification
  if (file == NULL) {
    printf("Data file does not exist (or isn't accessible).\n");
    }
  else {
  // read line by line
  char line[100]; // maybe we need to make it bigger if we add columns
  int i = 0;
  while(fgets(line, 100, file) != NULL) {
    double value = atof(line);
    table[i] = value;
    i += 1;

    }
  }
  fclose(file);
}


// Part 1: waste composition

/*
This function calculates the relative mass of an element before a reaction.
  So from the tabulated proportions of oxidized metals in Machefer, we
  We use this function for the calculation of the oxidation of metals and
  the combustion of Polyethylene.
*/
double OriginalMass(double mass1, double MW1, double MW2, double StoichCoefficient){

    // we first calculate the final number of moles
    double nbMol1  = mass1 / MW1;
    // we calculate the inital number of moles, according to the stoichiometric
    // coefficients
    double nbMol2 = StoichCoefficient  * nbMol1;

    // we get the initial mass of metal/gas, before oxidation/combustion
    double Mass2 = nbMol2 * MW2;
    return Mass2;
}
// Except for SiO2, which is glass, Carbon and Chlorine, all the other components
// are in their oxidized form, which means there were burnt.
// therefore to calculate their proportions in incoming waste,
// we need to calculate their proportions before oxidation (metallic)

// to mecanise repetitive tasks, we use a structure

struct Masses{
  double mSiO2;
  double mC;
  double mCl;
  double mAl;
  double mFe;
  double mCa;
} ; // [g]

  // This function puts all the initial relative proportions of metals
  // in a structure, for it to be used in two different functions downstream
struct Masses massesStructBuilder(double propSiO2, double propAl2O3, double propCaO, double propFe2O3, double propC, double propCl){

  // for one gram of Machefer
  // Glass (SiO2)
  double mSiO2 =  propSiO2 * 1;// [g]

  // Carbon
  double mC =  propC * 1;// [g]

  // Chlorine
  double mCl =  propCl * 1; // [g]

  // Aluminium
  // Tabulated Molar Weight of Al2O3 = 102 [g/mol], Al = 27 [g/mol]
  // Oxidation reaction: 2Al + 3O --> Al2O3
  double mAl2O3 = propAl2O3 * 1; // [g]
  double mAl = OriginalMass(mAl2O3, 102, 27, 2); // [g]

  // Iron
  // Tabulated Molar Weight of Fe2O3 = 159.6 [g/mol],  Fe = 55.8 [g/mol]
  // Oxidation reaction: 2Fe + 3O --> Fe2O3
  double mFe2O3 = propFe2O3 * 1; // [g]
  double mFe = OriginalMass(mFe2O3, 159.6, 55.8, 2); // [g]

  // Calcium
  // Tabulated Molar Weight of CaO = 56 [g/mol], Ca = 40 [g/mol]
  // Oxidation reaction: Ca + O --> CaO
  double mCaO = propCaO * 1; // [g]
  double mCa = OriginalMass(mCaO, 56, 40, 1); // [g]

  // initial mass of mixed metals that produced Machefer during combustion
  struct Masses masses = {mSiO2, mC, mCl, mAl, mFe, mCa};

  return masses;
}


/*
  This function takes as arguments the mass proportions of the composition of
  machefer. It is assumed to be composed of SiO2, Al2O3, CaO, Fe2O3, C and Cl.
  Knowing relative proportions of components of the inert part of the waste,
  we can calculate it's specific heat, which is essential for downstream calculations.
  We neglect the contribution of trace elements, as their proportion is insignificant.

  These proportions of metals in machefer are taken from tables! see line 340
*/
double CpInert(double propSiO2, double propAl2O3, double propCaO, double propFe2O3, double propC, double propCl){

  // initial mass of mixed metals that produced Machefer during combustion
  struct Masses masses =  massesStructBuilder(propSiO2, propAl2O3, propCaO, propFe2O3, propC, propCl);// [g]

  double mInitialMix = masses.mSiO2 + masses.mC + masses.mCl + masses.mAl + masses.mFe + masses.mCa;

  // now we can determine the relative proportions of metals
  // in the inert part of waste by normalizing each value

  double relmassTable [] = {masses.mSiO2, masses.mC, masses.mCl, masses.mAl, masses.mFe, masses.mCa};
  for (int i = 0; i < 6; i ++) {
    relmassTable[i] /= mInitialMix;

  }

  // Now that we have relative proportions, we can finally calculate
  // its specific heat value by taking the average of each heat capacity
  // weighted by its importance (proportion) in the inert part.

  // we give specific heat values (tabulated) of each component:
  // SiO2, Al, Fe, Ca, C, Cl respectively [J/(g*K)]:

  double  CpTable [] = {0.84, 0.894, 0.412, 0.63, 0.710, 0.48};

  // weighted average given by:

  double CpInert = 0;

  for (int i = 0; i < 6; i ++){
    CpInert += relmassTable[i] * CpTable[i];
  }

  return CpInert; // [J/(g*K)]
}


// Part 2: energy required to heat up waste to ignition

double Qcalculator(double m, double Cp, double Tfinal, double Tinitial) {

  double deltaT = Tfinal - Tinitial;
  double Qh = m * Cp * deltaT;

  return Qh;
}

/*
  we define Tignition for PE to be 350°C
  we define it as a global variable, because we use it in different
  functions multiple times
*/
double Tignition = 350;


double Qignition(double mC2H4, double mMoist, double mInert){

  // Starting from the global equation that gives the total heat required
  // to evaporate moisture and heat up waste:
  // Qignition = Qwaste + Qeva + Qsteam = (QC2H4 + Qinert + Qmoist) + Qeva + Qsteam
  // And noting that Q = m * Cp * ΔT, unless it is for latent heat where Q = m * Cp,
  // we have the following equations:

  // QC2H4
  double CpC2H4x = 2.25; // [KJ/Kg/K]
  double Tinitial = 20; // T°C at which waste enters combustion room
  double QC2H4T = Qcalculator(mC2H4, CpC2H4x, Tignition, Tinitial);

  double Cpfus = 230;// [kJ/Kg]
  double QC2H4Fusion =  Cpfus * mC2H4;
  double QC2H4 = QC2H4T + QC2H4Fusion;

  // Qinert
  // Values used below are taken from tables
  double Cpinert = CpInert(0.56, 0.10, 0.14, 0.076, 0.018, 0.015);
  double Qinert = Qcalculator(mInert, Cpinert, Tignition, Tinitial);

  // Qmoist
  double CpWater = 4.184; // [kJ/kg]
  double Tboiling = 100;
  double Qmoist = Qcalculator(mMoist, CpWater, Tboiling, Tinitial);

  double Qwaste = QC2H4 + Qinert + Qmoist;

  // Qeva
  double Cvap = 2257; // [kJ/kg]
  double Qeva =  Cvap * mMoist;

  // Qsteam
  double Csteam = 2; // [kJ/kg]
  double Qsteam = Csteam * mMoist;

  double Qignition = Qwaste + Qeva + Qsteam;
  // Qignition is the total energy input used to start the combustion reaction

  return Qignition;
}


// Part 3: heat released by waste combustion

/* First, we calculate the heat released by metal oxidation
   We do this in an analogous way to the CpInert function */
double QMetalOxi(double mInert, double propSiO2, double propAl2O3, double propCaO, double propFe2O3, double propC, double propCl){

  struct Masses masses =  massesStructBuilder(propSiO2, propAl2O3, propCaO, propFe2O3, propC, propCl);// [g]
  double mInitialMix = masses.mSiO2 + masses.mC + masses.mCl + masses.mAl + masses.mFe + masses.mCa;

  // we get the initial relative proportions
  // and at the same time, the mass of each metal for a given mInert
  double metalmassTable [] = {masses.mSiO2, masses.mC, masses.mCl, masses.mAl, masses.mFe, masses.mCa};

  for (int i = 0; i < 6; i ++) {
    metalmassTable[i] /= mInitialMix;
    metalmassTable[i] *= mInert;
  }

  // We know the enthalpy of oxidation for a given number of moles
  // for each metal. Hence, we convert mass to moles and get the energy produced
  // by the said metal's combustion.
  // Enthalpy is given for the product of the combustion reaction, so we need to add stoichio coefficient
  // We multiply by 1000 because mInert is given in kg, and we want to convert
  // it to grams to calculate the number of moles more easily.
  // Energy produced per mole is tabulated
  // Only Al, Fe and Ca are oxidised
  // n = m / MW

  // Aluminium : MW = 27 [g/mol]
  double nAl = 1000 * metalmassTable[1] / 27;
  double QAl = nAl * 1.669 / 2; // [kJ]

  // Iron : MW = 55.8 [g/mol]
  double nFe = 1000 * metalmassTable[2] / 55.8;
  double QFe = nFe * 824 / 2; // [kJ]

  // Calcium : MW = 40 [g/mol]
  double nCa = 1000 * metalmassTable[3] / 40;
  double QCa = nCa * 635; // [kJ]

  // We finally get the total energy produced by all the metals' combustion by adding up
  // all the individual combustion energies
  double QMetalOxi = QAl + QFe + QCa;

  return QMetalOxi;
}


double TfinalCalculator(double mC2H4, double mMoist, double mInert, double massMoyC2H4){

  // 3.1 : heat released by PE combustion and metal oxidation

  double propTable [] = {0.56, 0.10, 0.14, 0.076, 0.018, 0.015};

  // We assume the combustible part of waste is Polyethylene (PE)
  double QcC2H4x = 47000; // [kJ/kg] tabulated value
  double Qheat = QcC2H4x * mC2H4 + QMetalOxi(mInert, propTable[0], propTable[1], propTable[2], propTable[3], propTable[4], propTable[5]); // [KJ]

  double Qignit = Qignition(mC2H4, mMoist, mInert); // mCombustible = mC2H4
  double Qnet = Qheat - Qignit;

  // To get Tfinal, we use the equation Qnet = Cp * Mtot * (Tf - Ti)

  // 3.2 Calculations of Mtot = mflue + mprim

  // mflue = mass of flue gases
  double MWC2H4 = 28;

  // mC2H4 is given in main
  double mCO2 = OriginalMass(1000*mC2H4, MWC2H4, 44, 2)/1000; // [kg]
  double mH2O = OriginalMass(1000*mC2H4, MWC2H4, 18, 2)/1000; // [kg]

  double mflue = mCO2 + mH2O; // [kg]

  // finding mprim = mass of primary air
  // from the comubstion equation: 3O2 + C2H4x --> 2CO2 + 2H2O
  // we have to multiply massMoyC2H4 by 1000 because it is given in [kg]
  double nC2H4Moy = 1000 * massMoyC2H4 / MWC2H4; // [mol]
  double nO2 = 3 * nC2H4Moy; // [mol]

  // to ensure a good combustion, we'll overshoot the moles of primary air needed
  // by 50%, as it is suggested in litterature
  // in the end, 50% of primary air's ideal molarity will find itself
  // in combustion chamber, as it hasn't been burnt
  double nO2prim = 0.5 * nO2; // [mol]

  // We know air is not only composed by oxygen, but also N2
  // we neglect all other trace elements
  // we know volumetric proportions of air: 21% O2, 79%N2
  // So from the Ideal gas law : V = n * R * T / P

  double R = 8.2057; // [m3*atm/mol*K]
  double P = 1;
  double VO2prim = nO2prim * R * Tignition / P;

  // Because by Ideal Gas Law all gases take up the same volume for
  // a given number of moles, we find VN2 with a "règle de trois":
  double VN2 = (VO2prim/21) * 79;

  // we calculate the mass of each part of the air:
  // mass of O2 is simply mO2prim = nO2prim * MWO2
  double MWO2 = 32; //g/mol
  // we divide by 1000 because we do molar calculations in [g],
  // and we want the values in [kg]
  double mO2prim = nO2prim * MWO2 / 1000; // [kg]

  // to get mass of N2, we first calculate nN2 using the Ideal Gas law
  // nN2 = P * V / (R * Tignition)
  double nN2 = P * VN2 / (R * Tignition); // [mol]

  // then we get the mass as usual in grams from mN2 = nN2 * MW and dividing by 1000
  double MWN2 = 28;
  double mN2 = nN2 *  MWN2 /1000; //[Kg]

  // we can finally get mprim:
  double mprim = mO2prim + mN2;

  // We can add mflue and mprim to get total air: Mtot
  double Mtot = mflue + mprim;

  // 3.3 : final temperature

  // To get Tfinal, we use the equation Qnet = Cp * Mtot * (Tf - Ti)
  // we need to find Cp of our mixture:
  // to do so, we calculate the average of Cp of our components
  // weighted by their mass proportions:
  // tables made for O2, N2, CO2, H2O
  double massTable [] = {mO2prim, mN2, mCO2, mH2O};
  double CpTable [] = {0.919, 1.04, 0.844, 1.93};

  double Cptot = 0;
  for (int i = 0; i < 4; i++) {
    Cptot += massTable[i] * CpTable[i];
    Cptot /= Mtot;
  }

  // We solve for Tf : Tf = Qnet/(Cp * Mtot) + Tignition
  double Tfinal = Qnet / (Cptot * Mtot) + Tignition;

  return Tfinal;
}


// Part 4 : Heat exchanger

/*
  We're calculating the energy flow in the heat exchanger according to this equation:
  Qflow = k * A * LMTD
  Where Q = Energy flow that goes through the heat exchanger, k = heat transfer coefficient,
  A = heat transfer area, LMTD = logarithmic Mean Temperature Difference
*/
double QdotCalculator(double mC2H4, double mMoist, double mInert, double massMoyC2H4){

  double lambda = 45; // λ = thermal conductivity, [W/(mK)] (=45 W/(mK) making the assumption that
                      //the heat exchanger wall is only made of steel
  double thickness = 0.00833; // plate thickness of the heat exchanger [m]
  double alphaHot = 3500; // average of the tabulated values
  double alphaCold = 120; // average of the tabulated values
  double k = 1/ ((1/alphaHot) + (thickness/lambda) + (1/alphaCold)); // heat transfer coefficient

  // LMTD: Logarithmic Mean Temperature Difference
  double Tfinal = TfinalCalculator(mC2H4, mMoist, mInert, massMoyC2H4);
  double ThotIn = Tfinal;
  double ThotOut = 0.9 * ThotIn;
  double TcoldIn = 30;
  double TcoldOut = 450;

  double dA = ThotIn - TcoldOut;
  double dB = ThotOut - TcoldIn;

  double LMTD = (dA - dB)/ log(dA / dB);

  double A = 11490; // [m^2]

  // Energy flow
  double Qdot = k * A * LMTD;

  return Qdot;
}


// Part 5 : Energy harvesting

double WdotCalculator(double mC2H4, double mMoist, double mInert, double massMoyC2H4){

  // we know that Wdot = mdot * deltaH
  // we can calculte mdot with mdot = Qdot / (CpSteam * dT)
  double Qdot = QdotCalculator(mC2H4, mMoist, mInert, massMoyC2H4); // [J/s]
  double CpSteam = 2000; // [J/kg/K]
  double dT = 450 - 30; // [K]
  double mdot = Qdot / (CpSteam * dT) ; // [kg/s]

  // delaH = Hi - Hf, which are given in tables for our specific temperatures
  // The specific tempeartures were chosen upon what is common in litterture
  // for steam engines.
  // At 450°C and 50 bars, Hi = 3316 [kJ/kg]
  // At 77°C and 2 bars, Hf = 314 [kJ/kg]
  double Hf = 3316, Hi = 314;
  double deltaH = Hf - Hi;
  double Wdot = mdot * deltaH;

  return Wdot; // [kJ/day]
}

//Part 6: in the main

//Part 7: creating a new energy table adding variance
// and a NeededFuel table if the result of adding the variance is a negartive energy output

void stochastiser(double mu, double *PowerVarTable, double *NeededPetrol){

  // We create normally distributed daily energetic outputs based on our daily energetic output
  double TAU = 8 * atan(1);
  double max = 0, sigma=1.25, randomiser;

  randomiser = sqrt(-2*log(rand()/(RAND_MAX+1.0))) * cos(TAU*rand()/(RAND_MAX+1.0));
  double stochastEnergyVal = randomiser * sigma + mu;

  // Placing these varied energetic values in a table
  *PowerVarTable = stochastEnergyVal;

  // If randomised enrgetic values are under 0, it means that we need fuel to fully conduct the combustion.
  // As there is still waste that wasn't completely burnt due to insufficient energy.
  // To quantify this, we create a new table which we call FuelTable. (see main)
  // And to do so, we first need to do some calculations to adjust the units.

  if (stochastEnergyVal < 0){
    *NeededPetrol = fabs(stochastEnergyVal);
    *NeededPetrol = *NeededPetrol / 46; //[Kg/s]
    *NeededPetrol = *NeededPetrol * (3600 * 24); //[Kg/j]
    *NeededPetrol = *NeededPetrol / 1000; //[T/j]
  }

  else{
    *NeededPetrol = 0;
  }

}


// Part 8: creating a CSV writer to create all the CSV tables based on the tables we created above.

void write_csv(char * filename, double * table) {
  printf("\n Creating a %s file", filename);
  FILE * file = fopen(filename, "w");

  // iterating on the whole table
  for (int i = 0; i < 365; i++) {
    fprintf(file, "%f\n", table[i]);
  }

  fclose(file);
}
