# -*- coding: utf-8 -*-
"""
Created on Fri Dec 10 11:18:50 2021

@author: mimag
"""
import numpy as np
import matplotlib.pyplot as plt

"""Part 1: CSV file reading"""
# 1st we need to import the CSV file of Power outputs into a table

PowerTable = np.genfromtxt('PowerTable.csv', delimiter = ',', dtype = 'float64') # [MW]

varPowerTable = np.genfromtxt('varPowerTable.csv', delimiter = ',', dtype = 'float64') # [MW]

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
varPwr = varPowerTable

Fueltest = np.zeros(365)
fig, axs = plt.subplots(2, 2)

axs[0, 0].plot(days, Pwr, 'tab:orange', label="Calculated Power")
axs[0, 0].plot(days, TridelDay, 'tab:red', label="Official Power production")
axs[0, 0].set_title('Calc. and true Power output')
axs[0, 0].set(ylabel="[MW]")
axs[0, 0].set_xticks([])

axs[1, 0].plot(days, wstDay)
axs[1, 0].set_title('Waste weight')
axs[1, 0].set(ylabel="[tons]")
axs[1, 0].set(xlabel="[days]")

axs[0, 1].plot(days, varPwr, 'tab:green', label="Calc. Power with variance")
axs[0, 1].plot(days,TridelDay, 'tab:red', label="Official Power production")
axs[0, 1].set_title('Calc. variance and true output')
axs[0, 1].set_xticks([])

axs[1, 1].plot(days, Fueltest, 'tab:brown')
axs[1, 1].set_title('Minimal fuel needed')
axs[1, 1].set(xlabel="[days]")

