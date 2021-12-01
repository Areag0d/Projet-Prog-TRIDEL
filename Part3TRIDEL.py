import numpy as np
import matplotlib.pyplot as plt

"""Part 1: CSV file reading""" 
# 1st we need to import the CSV file of Power outputs into a table 

PowerTable = np.genfromtxt('PowerTable.csv', delimiter = ',', dtype = 'float64') # [MW]

wasteDayLst = np.genfromtxt('wasteDayLst.csv', delimiter = ',', dtype = 'float64') # [Kg]
wasteDayLst /= 1000 # conversion from [kg] to [tonnes]

"""Part 2: initializing reference values"""
# These values are given in [MWh] for per month
TridelValues = np.array([4942.84, 4909.77, 4959.43, 5041.34, 5876.92, 1396.43, 7550.33, 8926.98, 8242.48, 6819.23, 5744.39, 5439.18]) #[MWh / month]
TridelValues = TridelValues / (30.4 * 24) # converting [MWh] to [MW]

# Modifying the dimension of the arrays, in order for it to be possible
# to plot official and calculated values together

daysInMonths = [31, 28, 30, 31, 30, 31, 31, 31, 30, 31, 30, 31]
DayTridelVal = []

for i in range(len(TridelValues)):
    repeatedValue = np.repeat(TridelValues[i], daysInMonths[i])
    DayTridelVal = np.concatenate((DayTridelVal, repeatedValue))


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

