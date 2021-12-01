import numpy as np
import matplotlib.pyplot as plt
import csv
import math as ma

"""Part 1 CSV file creation""" 
# 1st we need to import the Csv file into a table 

PowerTable = np.genfromtxt('PowerTable.csv', delimiter = ',', dtype = 'float64') #[MW]
PowerTable *= 1000000 #[W]

wasteDayLst = np.genfromtxt('wasteDayLst.csv', delimiter = ',', dtype = 'float64') #[Kg]
#wasteDayLst /= 1000 #[tonnes]

TridelValues = np.array([4942.84, 4909.77, 4959.43, 5041.34, 5876.92, 1396.43, 7550.33, 8926.98, 8242.48, 6819.23, 5744.39, 5439.18]) #[MWh / month]
TridelValues = TridelValues / (30 * 24) #[MW]

# rendre ces valeurs par jours: copier chaque valeur le nombre de jours
# dans le mois correspondant

def valueRepeater(List, repNumber):
    for repetition in repNumber:
        for value in range(list):



"""Part 2 energy output plot"""

x = np.array([day + 1 for day in range(365)])

y = np.array(wasteDayLst)
z = PowerTable

#plt.title('Daily Waste Weight')
plt.plot(x, y, label = 'Daily Waste weight [Kg]')
plt.plot(x, z, label = 'Daily Power production [W]')
plt.legend()
plt.show()

d = [month for month in range(12)]
e = TridelValues
plt.plot(d, e, label = 'Tridel monthly Power production [MW]')
plt.legend()
plt.show()

