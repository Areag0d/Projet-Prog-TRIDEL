import numpy as np
import matplotlib.pyplot as plt
import csv
import math as ma

"""Part 1 CSV file creation""" 
# 1st we need to import the Csv file into a table 

PowerTable = np.genfromtxt('PowerTable.csv', delimiter = ',', dtype = 'float64') #[MW]
#PowerTable *= 1000000 #[W]

wasteDayLst = np.genfromtxt('wasteDayLst.csv', delimiter = ',', dtype = 'float64') #[Kg]
wasteDayLst /= 1000 #[tonnes]

TridelValues = np.array([4942.84, 4909.77, 4959.43, 5041.34, 5876.92, 1396.43, 7550.33, 8926.98, 8242.48, 6819.23, 5744.39, 5439.18]) #[MWh / month]
TridelValues = TridelValues / (30 * 24) #[MW] ecrire une boucle pour le diviser par 1000.

# rendre ces valeurs par jours: copier chaque valeur le nombre de jours
# dans le mois correspondant

daysInMonths = [31, 28, 30, 31, 30, 31, 31, 31, 30, 31, 30, 31]
DayTridelVal = []

for i in range(len(TridelValues)):
    repeatedValue = np.repeat(TridelValues[i], daysInMonths[i])
    DayTridelVal = np.concatenate((DayTridelVal, repeatedValue))

#print(DayTridelVal)


"""Part 2 energy output plot"""

x = np.array([day + 1 for day in range(365)])

y = np.array(wasteDayLst)
z = PowerTable

#plt.title('Daily Waste Weight')
plt.figure()
e = DayTridelVal
plt.subplot(2, 1, 1)
plt.plot(x, e, label = 'Daily given Power production')
plt.plot(x, z, label = 'Daily calculated Power production')
#plt.xlabel("Days")
plt.ylabel("[MW]")
plt.legend()

plt.subplot(2, 1, 2)

plt.plot(x, y, label = 'Daily Waste weight [tons]')
plt.legend()
plt.xlabel("Days")
plt.ylabel("[tons]")
plt.show()

