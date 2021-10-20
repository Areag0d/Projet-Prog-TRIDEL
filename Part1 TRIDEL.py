# -*- coding: utf-8 -*-
"""
Created on Wed Oct 20 15:57:44 2021

@author: Mike and Seif

Part 1 of TRIDEL modelisation: data generation
"""

import math
import numpy as np

"""on définit nos variables de départ"""

#habitants par région:
#on a les habitants totaux du canton de Vaud
habTotal = 

#on a le pourcentage que représentent les habitants
#d'une région vis-à-vis du canton
prophabLausanne =
prophabNord =
prophabOuest = 
prophabLaCote =

#On calcule simplement la quantité d'habitants pris en echarge
#par TRIDEL par région grâce aux données précédentes
#ATTENTION: on pondère la quantité d'habitants par un facteur 1/2
#pour les habitants des régions Nord et La Côte
habLausanne  = habTotal * prophabLausanne
habOuest = habTotal * prophabOuest
habLaCote = 0.5 * habTotal * prophabLaCote
habNord = 0.5 * habTotal * prophabNord

#déchets par habitant par région : wastehabRegion
wastehabLausanne = 
wastehabNord =
wastehabOuest =  
wastehabLaCote = 

"""On peut calculer la moyenne des déchets par habitants
pondérée par le nombre d'habitants"""
