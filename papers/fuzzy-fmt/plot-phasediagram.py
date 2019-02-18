#!/usr/bin/python2

#This program produces temperature vs density, and pressure vs temperature phase diagrams
#from data stored in phasenew.dat (or phasenewtensor.dat)

#NOTE: Run this plot script from directory deft/papers/fuzzy-fmt 
#with comand ./plot-phasediagram.py phasenew.dat (or phasenewtensor.dat)

from __future__ import print_function, division

import numpy as np
import matplotlib.mlab as mlab
import matplotlib.pyplot as plt
import os, glob
import argparse
import sys

parser = argparse.ArgumentParser()
parser.add_argument('--tensor', action='store_true',
                    help='use tensor weight')
args=parser.parse_args()

p_at_freezing = []  #pressure at freezing (intersection point between homogeneous and crystal plots)
n_homogeneous_at_freezing =[]
n_crystal_at_freezing = []

for kT in np.arange(0.05, 1.25, 0.05):

   n = []
   invn = []
   hfe = []
   cfe = []

   if args.tensor :
     files = sorted(list(glob.glob('crystallization/kT%.3f_n*_best_tensor.dat' % kT)))
   else :
      files = sorted(list(glob.glob('crystallization/kT%.3f_n*_best.dat' % kT)))

   for f in files:
      data = np.loadtxt(f)
      n.append(data[1])     #density
      invn.append(1/data[1])
      hfe.append(data[4])   #homogeneous free energy/atom
      cfe.append(data[5])   #crystal free energy/atom
   hfe = np.array(hfe)
   cfe = np.array(cfe)
   invn = np.array(invn)
   n = np.array(n)


   functions = np.vstack((np.ones_like(invn),
                       invn**-1,
                       invn**-2,
                       invn**-3,
                       invn**-4,
                       invn**-5,
                       invn**-6)).T
   pressure_functions = np.vstack((np.zeros_like(invn),
                                invn**-2,
                                2*invn**-3,
                                3*invn**-4,
                                4*invn**-5,
                                5*invn**-6,
                                6*invn**-7)).T
   A = np.linalg.lstsq(functions, cfe)
   coeff = A[0]
   #print('residuals', A[1])
   #print('coeff', coeff)
   fit_cfe = np.dot(functions, coeff)


   dhfe=np.diff(hfe)  #Caution: depends on order of data files!
   dcfe=np.diff(cfe)  #Caution: depends on order of data files!
   dinvn=np.diff(invn)  #Caution: depends on order of data files!
   mid_invn=invn[0:len(invn)-1]+dinvn/2
   hpressure = -(dhfe/dinvn) #for fixed N and Te   
   cpressure = -(dcfe/dinvn) #for fixed N and Te  

   fit_p = np.dot(pressure_functions, coeff)

   mid_hfe = 0.5*(hfe[1:] + hfe[:-1])
   mid_cfe = 0.5*(cfe[1:] + cfe[:-1])

   mid_h_gibbs = mid_hfe + mid_invn*hpressure
   mid_c_gibbs = mid_cfe + mid_invn*cpressure
   fit_c_gibbs = fit_cfe + invn*fit_p


   #Find pressure at point of intersection
   def find_first_intersection(p1, g1, p2, g2): 
      for i in range(1,len(g1)-1):
         m1=(g1[i+1]-g1[i])/(p1[i+1]-p1[i])
         for j in range(1,len(g2)-1):
               m2=(g2[j+1]-g2[j])/(p2[j+1]-p2[j])
               if m1!=m2 :
                  P_inter=(g2[j] - m2*p2[j] -g1[i] + m1*p1[i])/(m1-m2)
                  if p1[i] < P_inter < p1[i+1] and p2[j] < P_inter < p2[j+1]:
                     g_inter=m1*P_inter+g1[i]-m1*p1[i]
                     if g1[i] < g_inter < g1[i+1] and g2[j] < g_inter < g2[j+1] :
                           return P_inter, g_inter

   p_inter, g_inter = find_first_intersection(hpressure, mid_h_gibbs, cpressure, mid_c_gibbs)
   pf_inter, gf_inter = find_first_intersection(hpressure, mid_h_gibbs, fit_p, fit_c_gibbs)


   #Find homogeneous and crystal densities at p_inter
   def find_densities(p_inter, pressure, invn):
      for i in range(1,len(pressure)-1): 
         if pressure[i] > p_inter :
               pressureabove=pressure[i]
               invnabove=invn[i]
               pressurebelow=pressure[i-1]
               invnbelow=invn[i-1]
               m=(pressureabove-pressurebelow)/(invnabove-invnbelow)
               invn_inter=invnabove-((pressureabove-p_inter)/m)
               return invn_inter
   invnh=find_densities(p_inter, hpressure, mid_invn)
   invnc=find_densities(p_inter, cpressure, mid_invn)
   
   p_at_freezing.append=p_inter
   n_homogeneous_at_freezing.append=1/invnh
   n_crystal_at_freezing.append=1/invnc

   print (kT, p_inter, n_homogeneous_at_freezing, n_crystal_at_freezing)   #This print out is sent to >> phasenew.dat (or phasenewtensor.dat) by phasediagram_data.py

#---------------------------------------
#------
p_at_freezing = np.array(p_at_freezing)
n_homogeneous_at_freezing = np.array(n_homogeneous_at_freezing)
n_crystal_at_freezing = np.array(n_crystal_at_freezing)

kT_freeze= []
for kT in np.arange(0.05, 1.25, 0.05):    
   kT_freeze.append
kT_freeze = np.array(kT_freeze)
#------

#Temperature vs Density Phase Diagram
plt.plot(n_homogeneous_at_freezing, kT_freeze, label='liquid', color='red')
plt.plot(n_crystal_at_freezing, kT_freeze, label='solid', color='blue')
plt.fill_betweenx(kT_freeze, 0, n_homogeneous_at_freezing, color='red')  
plt.fill_betweenx(kT_freeze, n_homogeneous_at_freezing, n_crystal_at_freezing, color='gray') 
plt.fill_betweenx(kT_freeze, n_crystal_at_freezing, 1.1, color='blue')  
plt.title("Temperature vs Density")
plt.legend(loc='best')
plt.xlabel('Density')
plt.ylabel('kT')

plt.figure()

#Pressure vs Temperature Phase Diagram
plt.fill_between(kT_freeze, 0*p_at_freezing, p_at_freezing, color='red')
plt.fill_between(kT_freeze, p_at_freezing, p_at_freezing+100, color='blue')
plt.plot(kT_freeze, p_at_freezing, color='black')
plt.ylim(0, 40)
plt.xlim(kT_freeze.min(), kT_freeze.max())
plt.title("Pressure vs Temperature")
plt.xlabel('kT')
plt.ylabel('Pressure')

plt.show()
