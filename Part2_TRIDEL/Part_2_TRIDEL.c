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
//This function takes in argument proportions of the compostion of the machefer
//Machefer is assumed to be composed of SiO2, Al2O3, CaO, Fe2O3.
//these are massic proportions
double Cm_Inert(double propSiO2, double propAl2O3, double propCaO, double propFe2O3){
  //the given proportions do not sum up to 100%,
  //because we neglect trace elements
  //therefore, we proceed to a conditionnal compensation
  double accountedsum = propSiO2 + propAl2O3 + propCaO + propFe2O3;
  double v = 100 - accountedsum;
  //the Verbesserung is given to the measures proportionally to their weight
  //their weight is defined by their relative importance
  double compSiO2 = propSiO2 + (v * propSiO2/accountedsum);
  double compAl2O3 = propAl2O3 + (v * propAl2O3/accountedsum);
  double compCaO = propCaO + (v * propCaO/accountedsum);
  double compFe2O3 = propFe2O3 + (v * propFe2O3/accountedsum);
  //Except for SiO2, which is glass, all the other components are
  //in their oxidized form, which means there were burnt.
  //therefore we calculate
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

  //creer une fonction qui calcule les differents Q//
  double QcombT = Qcalculator( , 1.9 , , );
  double QcombFusion = Qcalculator( , 1.9 , , );
  double Qcomb = QcombT + QcombFusion;
  double Qinert = Qcalculator( , , , );
  double Qmoist = Qcalculator( , 4184, 100 , 20);

  double Qair = Qcomb + Qinert + Qmoist;
  return 0;
}
