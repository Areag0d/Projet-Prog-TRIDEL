import numpy as np
import matplotlib.pyplot as plt


# waiting for the PowerOutput CSV file to be written
# before running the last part

import time
time.sleep(0.5)

###########################################################################
###########################################################################

"""Interpreting the results"""

import matplotlib.pyplot as plt

"""Part 1: CSV file reading"""
# 1st we need to import the CSV file of Power outputs into a table

PowerTable = np.genfromtxt('PowerTable.csv', delimiter = ',', dtype = 'float64') # [MW]

wasteDayLst = np.genfromtxt('wasteDayLst.csv', delimiter = ',', dtype = 'float64') # [Kg]
wasteDayLst /= 1000 # conversion from [kg] to [tonnes]

varPowerTable = np.genfromtxt('varPowerTable.csv', delimiter = ',', dtype = 'float64') # [MW]

FuelNeededDay = np.genfromtxt('FuelNeededDay.csv', delimiter = ',', dtype = 'float64') # [Tons]

"""Part 2: initializing reference values"""
# These values are given in [MWh] per month

TridelValues = np.array([4942.84, 4909.77, 4959.43, 5041.34, 5876.92, 1396.43, 7550.33, 8926.98, 8242.48, 6819.23, 5744.39, 5439.18]) #[MWh / month]
TridelValues = TridelValues / (30.4* 24) # converting [MWh] to [MW]

# Modifying the dimension of the arrays, in order for it to be possible
# to plot official and calculated values together

daysInMonths = [31, 28, 30, 31, 30, 31, 31, 31, 30, 31, 30, 31]
DayTridelVal = []

for i in range(len(TridelValues)):
    repeatedValue = np.repeat(TridelValues[i], daysInMonths[i])
    DayTridelVal = np.concatenate((DayTridelVal, repeatedValue))


"""Part 3 energy output plot"""
# We divide our plot into four subplots:

days = np.array([day + 1 for day in range(365)])

wasteDay = np.array(wasteDayLst)

Fueltest = np.zeros(365)
fig, axs = plt.subplots(2, 2)
# Plotting calculated output values against true values
axs[0, 0].plot(days, PowerTable, 'tab:orange', label="Calculated Power")
axs[0, 0].plot(days, DayTridelVal, 'tab:red', label="Official Power production")
axs[0, 0].set_title('Calc. and true Power output')
axs[0, 0].set(ylabel="[MW]")

# Plotting mass of waste per day
axs[1, 0].plot(days, wasteDay)
axs[1, 0].set_title('Waste weight')
axs[1, 0].set(ylabel="[tons]")
axs[1, 0].set(xlabel="[days]")

# Plotting calculated output values with variance against true values
axs[0, 1].plot(days, varPowerTable, 'tab:orange', label="Calc. Power with variance")
axs[0, 1].plot(days,DayTridelVal, 'tab:red', label="Official Power production")
axs[0, 1].set_title('Calc. variance and true output')

# Plotting minimal fuel mass in order to conduct combustion
# with our added variance model
axs[1, 1].plot(days, FuelNeededDay, 'tab:brown')
axs[1, 1].set_title('Minimal fuel needed')
axs[1, 1].set(xlabel="[days]")

plt.tight_layout()
plt.show()

print("\nTHE END\n")