# -*- coding: utf-8 -*-
"""
Created on Wed Dec  1 19:12:39 2021

@author: Mikael & Seif

"""

import numpy as np
import csv

"""We define our starting variables"""

# inhabitants per region:
# inputting the number of inhabitants
print("\nEstimate the current population of the canton of Vaud.")
habTotalEstim = input("\nIf you do not care... press Enter: ")
habTotal = 815300

# making sure that the population value input is relevant
# (that it is an integer and that it is not too far from the current population of the canton):

if habTotalEstim == "":
    print("\nNo received value, we shall then take the value of 2020 which is 815300 person.\n")
    habTotalEstim = habTotal

else:
    try:
        habTotalEstim = int(habTotalEstim)

    except ValueError:
        print("\nReceived value is not an integer, we shall take the value of 2020 which is 815300 person.\n")
        habTotalEstim = habTotal

    if habTotalEstim < 0:
        print("\nReceived value is negative, we shall take the value of 2020 which is 815300 person.\n")
        habTotalEstim = habTotal

if habTotalEstim != habTotal:

    if np.abs(habTotalEstim - habTotal) > 100000:
        print("\nReceived value is too far from official values, we shall take the value of 2020 which is 815300 person.\n")
        habTotalEstim = habTotal

    else:
        print("\nReceived value is within a reasonable range from official values, we shall then base all our calculations on this value.\n")



# we have the inhabitance percentage of the inhabitants
# of a region with respect to the canton
prophabLausanne = 0.25
prophabOuest = 0.25
prophabLaCote = 0.14
prophabNord = 0.10

"""We can easily calculate the number of people from which
TRIDEL collects waste from the previous data.
WARNING: we weigh the number of inhabitants by a factor of 1/2
for the inhabitants of Nord and La Côte regions"""


habLausanne  = habTotal * prophabLausanne
habOuest = habTotal * prophabOuest
habLaCote = habTotal * 0.5 * prophabLaCote
habNord = habTotal * 0.5 * prophabNord
habChargTot = habLausanne + habOuest + habLaCote + habNord

habChargLst = np.array([habLausanne, habOuest, habLaCote, habNord])

#waste per person per region [kg/year] : wastehabRegion

wastehabLausanne = 185.1
wastehabNord = 151.4
wastehabOuest = 130.8
wastehabLaCote = 147.3
wasteHabLst = np.array([wastehabLausanne, wastehabNord, wastehabOuest,  wastehabLaCote])

"""we can calculate the inhabitants' waste average,
weighted by the number of inhabitants of each city per year [kg/year]"""


wastePondLst = (wasteHabLst * (habChargLst / habChargTot))
wasteAvg = np.sum(wastePondLst)
#print(wastePondLst, wasteAvg)

"""We compute the total waste quantity per day,
for that, we estimate a sigma of 0.02% times wasteAvg,
and randomly (but following a normal distribution) generate
a value of the total waste quantity each day [kg/day]"""


sigmaWasteAvg = 0.2 * wasteAvg
wasteYearArray = np.random.normal(wasteAvg, sigmaWasteAvg, size=(365))
wasteDayArray = (wasteYearArray / 365) * habChargTot

#converting the array to a list

wasteDayLst = []

for i in wasteDayArray:
    wasteDayLst.append([i])


"""We now create a CSV file containing the data we have generated"""

print("Creating a CSV file for the generated values of waste per day...\n")
with open("wasteDayLst.csv", "w", newline= "") as file:
    writer = csv.writer(file)
    writer.writerows(wasteDayLst)


"""We run the second part of the code, written in C."""

print("Running the program computing the values...\n")

import os

# testing whether the executable file is at the right place
# The program then waits for the user to execute part 2 to resume

while not os.path.exists("Part2TRIDEL.exe"):
   print("\nPart2TRIDEL.exe does not exist or isn't in the same folder as the program.\n")
   print("Please, compile Part2TRIDEL.c or add the .exe file in the same folder,\n")
   input("Then, come back and press enter to continue...")
   print("\n")

os.startfile(r"Part2TRIDEL.exe")

print("Creating a CSV file of calculated Power output...\n")
print("Creating a CSV file of Variance calculated Power output...\n")
print("Creating a CSV file of the needed fuel if the output is negative...\n")