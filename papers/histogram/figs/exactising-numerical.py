#!/usr/bin/env python

from __future__ import division
import sys, os
import numpy as np
import math as mh
from scipy.special import comb

from fractions import Fraction
import matplotlib.pyplot as plt
import matplotlib
import readnew

filename = sys.argv[1]
n = int(sys.argv[2])
m = int(sys.argv[3])

#------------------------------------------------------#
#------- COMPUTE THE EXACT REFERENCE FROM BEALE -------#

#LINK --> https://spot.colorado.edu/~beale/IsingExactMathematica.html

# # define a Temperature Range
# dT = 0.0001
# T = np.arange(0.1,100,dT)
#
# # define parameter 'x' in terms of the Temperature
# x = np.exp(-2/T)
# print(x)

x = np.poly1d(np.array([1, 0], dtype=Fraction))

# define Beta (not to be confused with 1/kBT) as per Paul D. Beale
b = 2*x - 2*x*x*x

def CalculateCoefSum(n, m, k):
    # define a[k_] as per Paul D. Beale
    #print('ccs of',n,m,k)
    a_coef = (1 + x*x)**2 - b*Fraction(np.cos(np.pi*k/n))
    #print('a_coef', a_coef)
    frac = Fraction(np.cos(np.pi*k/n))
    c2_coef_sum = np.poly1d(np.array([Fraction(0.5)]))
    for j in range(0, m+2, 2): # should include endpoint so m --> m+2
        c2_coef_sum += (a_coef**2 - b*b)**(j//2) * a_coef**(m - j)*comb(m, j, exact=True)
    print('c2_coef_sum', type(c2_coef_sum.c[0]), type(c2_coef_sum.c[-1]))
    c2_coef_sum -= np.poly1d(np.array([Fraction(0.5)]))
    return c2_coef_sum

def CalculateRef(n, m):
    if (n % 2 == 0):

        # define coefficients (x = exp(-2K) = exp(-2J/kbT).
        # the coefficients c0,s0,cn,sn are just numbers contrary
        # to what the notation would have you believe.
        # dividing by 2**(m/2) is different than paper?
        two = Fraction(2)
        c0 = ((1-x)**m + (x*(1+x))**m) / two**(m//2)
        s0 = ((1-x)**m - (x*(1+x))**m) / two**(m//2)
        cn = ((1+x)**m + (x*(1-x))**m) / two**(m//2)
        sn = ((1+x)**m - (x*(1-x))**m) / two**(m//2)

        c2_coef = 1
        s2_coef = 1
        for k in range(1, n, 2): # should include endpoint so n - 1 --> n
            c2_coef_sum = CalculateCoefSum(n, m, k)
            # define c2[k_] and s2[k_] as per Paul D. Beale
            c2_coef *= two**(1 - 2*m)*((c2_coef_sum) + b**m)
            s2_coef *= two**(1 - 2*m)*((c2_coef_sum) - b**m)
            print('odd', k, '/', n, type(c2_coef.c[-1]), type(s2_coef.c[-1]))

        z1 = two**(m*n//2 - 1)*c2_coef
        z2 = two**(m*n//2 - 1)*s2_coef
        print('z1', type(z1.c[-1]))
        print('z2', type(z2.c[-1]))

        c2_coef = 1
        s2_coef = 1
        for k in range(2, n, 2): # should include endpoint so n - 2 --> n
            c2_coef_sum = CalculateCoefSum(n, m, k)
            # define c2[k_] and s2[k_] as per Paul D. Beale
            c2_coef *= two**(1 - 2*m)*((c2_coef_sum) + b**m)
            s2_coef *= two**(1 - 2*m)*((c2_coef_sum) - b**m)
            print('even', k, '/', n, type(c2_coef.c[-1]), type(s2_coef.c[-1]))

        z3 = two**(m*n//2 - 1)*c0*cn*c2_coef
        z4 = two**(m*n//2 - 1)*s0*sn*s2_coef
        print('z3', type(z3.c[-1]))
        print('z4', type(z4.c[-1]))


    else:
        print('Cannot determine the Parition Function for (ODD) spin systems!')
        assert(False)
    return z1 + z2 +z3 +z4

# The general expression for the entropy is given (via Helmholtz Energy):
# F := E - TS = - kB T ln(Z) --> S(E) = kBln(Z(Beta)) + E/T
# E = - d/d(Beta) ln(Z) or equivalently E = kB T^2 d/dT ln(Z)

# def EvaluatePartitionFunction(n,m,Z,Temp,dT):
#     # convert Z to lnZ and take a derivative
#     # K = J/kBT
#     lnZ = 2*n*m/Temp + np.ln(Z.astype(float))
#     #Z = np.exp(2*n*m/Temp)*Z
#
#     E = Temp*Temp*np.gradient(lnZ,dT)
#     S =  lnZ + E/Temp
#
#     return S,E

def OutputFile(SaveName, max, min):
    dirname = 'data/%s-reference-lndos.dat' % (SaveName)
    print('saving to', dirname)
    np.savetxt(dirname,
          #np.c_[-Evals, Sinterp],
          np.c_[E, S],
          #np.c_[E[E>=-2*n*m],S[E>=-2*n*m]],
          fmt = ('%.16g'),
          delimiter = '\t',
          header = 'comparison reference file\t(generated with python %s \n max_entropy_state: %i \n min_important_energy: %i \n energy\t lndos\t\t ' % (' '.join(sys.argv), max, min))

Z_long = CalculateRef(n, m)
print('sum          ', Z_long.c.sum(), 2**(n*m))
print('sum even only', Z_long.c[0::2].sum(), 2**(n*m))
exit(1)
print((Z_long.c[0:None:2]))

# print('zeros below')
# print(Z_long.c[1:None:2])
# print 'the above should be zeros'
print('type of Z_long[-1]', type(Z_long.c[-1]))
S = np.array(np.log(Z_long.c[0:None:2].astype(float)))

#print((np.array(Z_long.c[0:None:2])).tolist())
print((len(S)))
E = np.arange(-2*n*m, -2*n*m + len(S)*4, 4)
print((len(E)))
#S,E = EvaluatePartitionFunction(n,m,Z_long,T,dT)
# remove the first element (due to derivative?)
#S = np.delete(S,0)
#E = np.delete(E,0)
print((S, E))
# restrict to energy range of interest for interpolation
#S,E = S[E>=-2*n*m],E[E>=-2*n*m]

# interpolate
#Evals = np.arange(0,-2*n*m-.1,-4)
#Sinterp = np.interp(Evals,E,S)

max_entropy_state = 0
min_important_energy = 1

OutputFile(filename, max_entropy_state, min_important_energy)

plt.plot(E, S, '.-')

#plt.plot(Evals,Sinterp,'o')
plt.plot([-2*n*m, -2*n*m+8, -2*n*m+12, -2*n*m+16, -2*n*m+20],
          np.log([2, 2*n*m, 4*n*m, n**2*m**2+9*n*m, 4*n**2*m**2+24*n*m]), 'x')

# compare with some data
ref = '/home/jordan/deft/papers/histogram/data/ising-sad-32-reference-lndos.dat'
try:
    eref, lndosref, Nrt_ref = readnew.e_lndos_ps(ref)
except:
    eref, lndosref = readnew.e_lndos(ref)
plt.plot(eref, lndosref - lndosref[-1] + np.log(2), 'o', markersize=3, alpha=.2)

plt.xlabel('Energy')
plt.ylabel('ln(S(E))')
#plt.ylim(0,750)
#plt.xlim(-2200,50)
plt.show()
