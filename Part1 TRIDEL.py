# -*- coding: utf-8 -*-
"""
Created on Wed Oct 20 15:57:44 2021

@author: Mike and Seif

Part 1 of TRIDEL modelisation: data generation
"""

import math
import numpy as np
import random as rdm
import csv

"""on définit nos variables de départ"""

#habitants par région:
#on a le nombre total  d'habitants du canton de Vaud

habTotal = 815300

#on a le pourcentage que représentent les habitants
#d'une région vis-à-vis du canton

prophabLausanne = 0.25
prophabOuest = 0.25
prophabLaCote = 0.14
prophabNord = 0.10

#On calcule simplement la quantité d'habitants pris en echarge
#par TRIDEL par région grâce aux données précédentes
#ATTENTION: on pondère la quantité d'habitants par un facteur 1/2
#pour les habitants des régions Nord et La Côte

habLausanne  = habTotal * prophabLausanne
habOuest = habTotal * prophabOuest
habLaCote = habTotal * 0.5 * prophabLaCote
habNord = habTotal * 0.5 * prophabNord
habChargTot = habLausanne + habOuest + habLaCote + habNord
#print(f"habitants total pris en charge par TRIDEL = {habChargTot}")
habChargLst = np.array([habLausanne, habOuest, habLaCote, habNord])
n = len(habChargLst)

#déchets par habitant par région [kg/année] : wastehabRegion

wastehabLausanne = 185.1
wastehabNord = 151.4
wastehabOuest = 130.8
wastehabLaCote = 147.3
wasteHabLst = np.array([wastehabLausanne, wastehabNord, wastehabOuest,  wastehabLaCote])

"""On peut calculer la moyenne des déchets par habitants
pondérée par le nombre d'habitants de chaque ville par annee [kg/annee]"""

wastePondLst = []

for i in range (n):
    wastePondLst.append(wasteHabLst[i] * (habChargLst[i] / habChargTot))

wastePondLst = np.array([wastePondLst])
wasteAvg = np.sum(wastePondLst)
#print(wastePondLst, wasteAvg) 

"""On calcule la quantite totale de dechets par jour,
pour cela nous estimons un sigma de 25% a wasteAvg, 
et generons aleatoirement(mais selon une distribution normale)
une valeur des dechets pour chaque jour [kg/jour]"""

sigmaWasteAvg = 0.2 * wasteAvg
wasteYearLst = np.random.normal(wasteAvg, sigmaWasteAvg, size=(365))
wasteDayLst = wasteYearLst / 365

#print(wasteDayLst)

