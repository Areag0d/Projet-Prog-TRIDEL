import numpy as np
import matplotlib.pyplot as plt
import csv
import math as ma

"""Part 1 CSV file creation""" 
# 1st we need to import the Csv file into a table 

#PowerTable = np.genfromtxt('PowerTable.csv', delimiter = ',', dtype = 'float64')
wasteDayLst = np.genfromtxt('wasteDayLst.csv', delimiter = ',', dtype = 'float64')
#if 


"""Part 2 energy output plot"""

x = np.array([day + 1 for day in range(365)])

#y = np.array(PowerTable)
y = np.array(wasteDayLst)

plt.plot(x, y)
plt.show()
