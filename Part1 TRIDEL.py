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
#on a le nombre total  d'habitants du canton de Vaud
habTotal = 815300

#on a le pourcentage que représentent les habitants
#d'une région vis-à-vis du canton
prophabLausanne = 0.25
prophabOuest = 0.25
prophabLaCote = 0.14
prophabNord = 0.10

#On calcule simplement la quantité d'habitants par région
#grâce aux données précédentes
habLausanne  = habTotal * prophabLausanne
habNord = habTotal *
habOuest =
habLaCote = 

#déchets par habitant par région [kg/année] : wastehabRegion
wastehabLausanne = 185.1
wastehabNord = 151.4
wastehabOuest = 130.8
wastehabLaCote = 147.3

