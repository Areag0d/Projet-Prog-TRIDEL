# -*- coding: utf-8 -*-
"""
Created on Wed Oct 20 15:57:44 2021

@author: Mike and Seif

Part 1 of TRIDEL simulation: data generation
"""

import numpy as np
import csv

"""We define our starting variables"""

# inhabitants per region:
# inputting the number of inhabitants
print("Estimate the current population of the canton of Vaud.")
habTotal = input("If you do not care.. press Enter: ")

if habTotal == "":
    habTotal = 815300
    print("No received value, we shall then take the value of 2020 which is 815300 person.\n")

habTotal = float(habTotal)
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


sigmaWasteAvg = 0.02 * wasteAvg
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

print("Running the part of the program computing the values...")

import os

os.startfile(r"Part_2_TRIDEL.exe")

