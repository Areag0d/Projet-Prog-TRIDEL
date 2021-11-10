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

//Creer une fonction qui calcule m moist et m inert pour chaque jour//

double mMoistEtmInert(moistProportion, inertProportion){

  double * mTable = wasteDayTable;
  double moistProportion = 0.15;
  double inertProportion = ;

  for(int i = 0; i < 366; i++){
  double * mMoist [i] = mTable  * moistProportion;
  double * mInert [i] = mTable * inertProportion;
  }
  return mMoist, mInert;
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
