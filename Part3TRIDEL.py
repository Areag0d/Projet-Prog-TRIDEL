import numpy as np
import matplotlib.pyplot as plt
import csv
import math as ma

"""Part 1 CSV file creation""" 
# 1st we need to import the Csv file into a table 

#PowerTable = np.genfromtxt('PowerTable.csv', delimiter = ',', dtype = 'float64')
wasteDayLst = np.genfromtxt('wasteDayLst.csv', delimiter = ',', dtype = 'float64') #[Kg]
#wasteDayLst /= 1000 #[tonnes]
#if 
TridelValues = np.array([4942.84, 4909.77, 4959.43, 5041.34, 5876.92, 1396.43, 7550.33, 8926.98, 8242.48, 6819.23, 5744.39, 5439.18]) #[MWh / month]
TridelValues = TridelValues / (30 * 24) #[MW]



"""Part 2 energy output plot"""

x = np.array([day + 1 for day in range(365)])

#y = np.array(PowerTable)
y = np.array(wasteDayLst)

plt.xlabel('Day')
plt.ylabel('WasteDay [Kg]')
plt.zlabel('')
plt.title('Daily Waste Weight')
plt.plot(x, y)

plt.show()

d = [i for i in range(13)]
z = TridelValues