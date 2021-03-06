from __future__ import division, print_function
import sys, os, re
import numpy as np

import pint
ureg = pint.UnitRegistry()
import pandas as pd # import data tables from NIST and store with pandas

# --- Function to find nearest value in array and return index and element --- #

def find_nearest(array, value):
    #array = np.asarray(array)
    assert(value < array.max() and value >= array.min())
    idx = (np.abs(array - value)).argmin()
    return idx, array[idx]

# --- Build unit system --- #

Kelvin = ureg.kelvin
atm = ureg.atm
gram = ureg.g
L = ureg.L
Joule = ureg.J
mol = ureg.mol
psi = ureg.psi

mg = ureg.mg    # milligram
mL = ureg.mL    # milliliter
kJ = ureg.kJ     # kiloJoule

# --- Command line arguments --- #

pLow = sys.argv[1]
pHigh = sys.argv[2]
pInc = sys.argv[3]
Temperature = sys.argv[4]

MM = 16.043 * gram/mol     # molar mass of Methane
NA = 6.02214e23 * 1/mol   # Avogadro's constant

# --- Read NIST tables using Pandas --- #

html_web = "https://webbook.nist.gov/cgi/fluid.cgi?Action=Load&ID=C74828&"
html_arg = "Type=IsoTherm&Digits=12&PLow=%s&PHigh=%s&PInc=%s&T=%s" % (pLow, pHigh, pInc,Temperature)
html_units = "&RefState=DEF&TUnit=K&PUnit=atm&DUnit=g%2Fml&HUnit=kJ%2Fmol&WUnit=m%2Fs&VisUnit=uPa*s&STUnit=N%2Fm"
tables = pd.read_html(html_web + html_arg + html_units)

# Convert from pandas to numpy arrays (index starts at 0)

T = tables[0][0].values[1:].astype(np.float64)     # Temperature
T = T*Kelvin
p = tables[0][1].values[1:].astype(np.float64)     # Pressure
p = p*atm
rho = tables[0][2].values[1:].astype(np.float64)   # Density
rho = rho*gram/(mL)

V = tables[0][3].values[1:].astype(np.float64)     # Volume
V = V*mL/gram
U = tables[0][4].values[1:].astype(np.float64)     # Internal Energy
U = U*kJ/mol
H = tables[0][5].values[1:].astype(np.float64)     # Enthalpy
H = H*kJ/mol
S = tables[0][6].values[1:].astype(np.float64)     # Entropy
S = S*Joule/mol/Kelvin
Cv = tables[0][7].values[1:].astype(np.float64)    # Heat Capacity
Cv = Cv*Joule/mol/Kelvin

G = H - T*S
F = U - T*S

Gads = 8*kJ/mol # the chemical potential of adsorption (in kJ/mol)

# --- Calculations --- #

# Final Pressure and Fixed Volume
Fp = 1*atm + 500*psi
print('filled pressure', Fp)
HOC = -882.0 *kJ/mol # - if enthalpy (endothermic) and + if exothermic

# Initial and final pressures
p_i = p[0]
idx,p_f = find_nearest(p,Fp)
p_f = p_f
print('actually filled pressure', p_f)

# Initial and final number density of methane gas
n_i_gas = rho[0] / MM * NA
n_f_gas = rho[idx] / MM * NA

idx_i,G_i = find_nearest(G, G[0] + Gads)
idx_f,G_f = find_nearest(G, G[idx] + Gads)
print('G_f vs expected:', G_f, G[idx] + Gads)
print('G min/max', G[0], G[-1])
n_i = rho[idx_i] / MM
n_f = rho[idx_f] / MM

print('n_i', n_i, 'n_f', n_f)
print("ratio of species in final to initial: ",n_f/n_i)

# Energy stored per volume (ESV)
print('mass density stored', rho[idx_f] - rho[idx_i])
ESV = HOC*(n_i - n_f)
#- Cv[0]*(0) - mu*(n_i-n_f)+H[0]*n_i-H[int(idx)]*n_f # Need help on this???
print('ESV', ESV)

print('energy wasted while compressing/vol', (F[idx] - F[0])*n_f)
