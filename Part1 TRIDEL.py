# -*- coding: utf-8 -*-
"""
Created on Wed Oct 20 15:57:44 2021

@author: Mike and Seif

Part 1 of TRIDEL modelisation: data generation
"""

import numpy as np
import csv

"""on définit nos variables de départ"""

#habitants par région:
#inputing the number of inhabitants
print("Donnez le nombre d'habitants actuels du canton de Vaud.")
habTotal = input("Si vous ne savez pas, appuyez sur Enter: ")

if habTotal == "":
    habTotal = 815300
    print("Pas d'input reçu, la valeur de 2020 qui est de 815300 habitants.\n")

habTotal = float(habTotal)
#on a le pourcentage que représentent les habitants
#d'une région vis-à-vis du canton

prophabLausanne = 0.25
prophabOuest = 0.25
prophabLaCote = 0.14
prophabNord = 0.10

"""On calcule simplement la quantité d'habitants pris en echarge
par TRIDEL par région grâce aux données précédentes
ATTENTION: on pondère la quantité d'habitants par un facteur 1/2
pour les habitants des régions Nord et La Côte"""

habLausanne  = habTotal * prophabLausanne
habOuest = habTotal * prophabOuest
habLaCote = habTotal * 0.5 * prophabLaCote
habNord = habTotal * 0.5 * prophabNord
habChargTot = habLausanne + habOuest + habLaCote + habNord
#print(f"habitants total pris en charge par TRIDEL = {habChargTot}")
habChargLst = np.array([habLausanne, habOuest, habLaCote, habNord])

#déchets par habitant par région [kg/année] : wastehabRegion

wastehabLausanne = 185.1
wastehabNord = 151.4
wastehabOuest = 130.8
wastehabLaCote = 147.3
wasteHabLst = np.array([wastehabLausanne, wastehabNord, wastehabOuest,  wastehabLaCote])

"""On peut calculer la moyenne des déchets par habitants
pondérée par le nombre d'habitants de chaque ville par annee [kg/annee]"""

wastePondLst = (wasteHabLst * (habChargLst / habChargTot))
wasteAvg = np.sum(wastePondLst)
#print(wastePondLst, wasteAvg)

"""On calcule la quantite totale de dechets par jour,
pour cela nous estimons un sigma de 0.2% a wasteAvg,
et generons aleatoirement(mais selon une distribution normale)
une valeur des dechets pour chaque jour [kg/jour]"""

sigmaWasteAvg = 0.02 * wasteAvg
wasteYearArray = np.random.normal(wasteAvg, sigmaWasteAvg, size=(365))
wasteDayArray = (wasteYearArray / 365) * habChargTot

#converting the array to a list

wasteDayLst = []

for i in wasteDayArray:
    wasteDayLst.append([i])


"""On crée maintenant fichier csv à partir du jeu de données que nous avons"""

print("Creating a CSV file for the generated values of waste per day...\n")
with open("wasteDayLst.csv", "w", newline= "") as file:
    writer = csv.writer(file)
    writer.writerows(wasteDayLst)


"""On lance la deuxième partie du programme, en C."""

print("Running the part of the program computing the values...")
import os

os.startfile(r"Part_2_TRIDEL.exe")

