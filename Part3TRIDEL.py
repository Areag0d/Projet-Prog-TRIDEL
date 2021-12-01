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
TridelValues = TridelValues / (30 * 24) #[MW] 

# rendre ces valeurs par jours: copier chaque valeur le nombre de jours
# dans le mois correspondant

daysInMonths = [31, 28, 30, 31, 30, 31, 31, 31, 30, 31, 30, 31]
DayTridelVal = []

for i in range(len(TridelValues)):
    repeatedValue = np.repeat(TridelValues[i], daysInMonths[i])
    DayTridelVal = np.concatenate((DayTridelVal, repeatedValue))

#print(DayTridelVal)


"""Part 3 energy output plot"""

#We divide our plot into two subplots:
#we start by plotting our energy output vs the official energy output of Tridel

days = np.array([day + 1 for day in range(365)])

wstDay = np.array(wasteDayLst)
Pwr = PowerTable
TridelDay = DayTridelVal

plt.figure()

plt.subplot(2, 1, 1)
plt.plot(days, TridelDay, label = 'Official daily Power production')
plt.plot(days, Pwr, label = 'Daily calculated Power production')
plt.ylabel("[MW]")
plt.legend()

#We then plot the total waste taken up by TRIDEL as a function of
#the number of people chosen by the user in part 1.

plt.subplot(2, 1, 2)

plt.plot(days, wstDay, label = 'Daily Waste weight [tons]')
plt.legend()
plt.xlabel("Days")
plt.ylabel("[tons]")
plt.show()

