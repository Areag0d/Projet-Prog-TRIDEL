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
