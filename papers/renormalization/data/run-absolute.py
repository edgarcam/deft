#!/usr/bin/python

from __future__ import division
from math import pi       # REALLY don't need all of math
import os, numpy as np, sys

assert(not os.system("fac ../../../free-energy-monte-carlo ../../../free-energy-monte-carlo-infinite-case"))


R=1

i = eval(sys.argv[1])
#RG recursion level

ww = float(sys.argv[2])
#arg ww = [1.3, 1.5, 2.0, 3.0]

L = float((sys.argv[3]))
#arg L = [5.0]

L_i = L*(2**i)
#incorperation of RG parameter i

Ns = eval(sys.argv[4])
#arg Ns = [range(2,10)]

Overwrite = False
if '-O' in sys.argv:  # Check for overwrite flag in arguments
    Overwrite = True
    
print("\n----------------------FREE ENERGY MONTE CARLO------------------------------\n\n")

      
def free_energy_over_kT_to_ff(free_energy): # Find an ff that corresponds to given free energy
    # bisection
    n = 0
    low,high = 0,1
    resolution = 1E-8
    while n < 500: #better value?
        ff_guess = (low+high)/2
        free_energy_next =  (4*ff_guess - 3*ff_guess**2)/(1-ff_guess)**2
        if abs(free_energy_next - free_energy) < resolution:
            return ff_guess
        elif free_energy_next > free_energy:
            high = (high+low)/2
        else:
            low = (high +low)/2
        n+=1
    return ff_guess

have_srun = '--srun' in sys.argv
max_non_srun_jobs = 5
jobs_submitted = 0


for N in Ns:
    # The maximum free energy is near the point where
    # Carnahan-Starling predict a filling fraction of 0.7, which is
    # not reasonable to simulate.
    approximate_free_energies = np.arange(np.log(2), 3000*N, np.log(2))/N
    ffs = np.zeros_like(approximate_free_energies)
    for k in xrange(len(ffs)):
        ffs[k] = free_energy_over_kT_to_ff(approximate_free_energies[k])

    dirname = 'scrunched-ww%4.2f-L%04.2f/i%01d/N%03d/absolute' % (ww,L,i,N)
    if Overwrite:
        os.system('rm -rf '+dirname)
    print('mkdir -p '+dirname)
    os.system('mkdir -p '+dirname)

    sim_runs = 1000000
    sc_period = int(max(10, 1*N*N/10))

    ff_goal = (4*np.pi/3*R**3)*N/(L_i)**3 # this is the density

    for j in xrange(len(ffs)-2):
        filename = '%05d' % (j)
        ff = ffs[j]
        ff_next = ffs[j+1]
        if ffs[j+2] > ff_goal:
            ff_next = ff_goal

        output_file_path = dirname+'/'+filename+'.dat'
        if not os.path.isfile(output_file_path):
            print "Was checking for", output_file_path
            if have_srun:
                cmd = 'srun -J L%04.2f/i%01d/N%03d/abs/%s' % (L,i,N, filename)
            else:
                cmd = ''

            cmd += ' nice -19' # don't hog the CPU
            if j==0:
                cmd += ' ../../../free-energy-monte-carlo-infinite-case'
                cmd += ' --ff_small %g' % ff_next
                # do infinite case for first step always
                cmd += ' --counts %d' % sim_runs
            else:
                cmd += ' ../../../free-energy-monte-carlo'
                cmd += ' --ff_small %g' % ff_next
                cmd += ' --ff %g' % ff
                cmd += ' --sc_period %d' % sc_period
                cmd += ' --runs %d' % sim_runs
            cmd += ' --N %d' % N
            cmd += ' --filename %s' % filename
            cmd += ' --data_dir %s' % dirname
            cmd += ' > %s/%s.out 2>&1 &' % (dirname, filename)
            print("Running with command: %s" % cmd)
            os.system(cmd)
            jobs_submitted += 1
            if not have_srun and jobs_submitted >= max_non_srun_jobs:
                print("I think this is enough run-absolute processes for now.  No fork bombs!!!\n")
                exit(0)
        else:
            print("You're trying to overwrite:\n %s \n Use the flag -O in order to do so.\n" % output_file_path)

        if ffs[j+2] > ff_goal:
            # We are all done now!
            # This automagically handles when ff > ff_goal!
            break

if jobs_submitted == 0:
    print "Nothing needs to be done from run-absolute!"
elif jobs_submitted == 1:
    print "I submitted 1 run-absolute job.\n"
else:
    print "I submitted %d run-absolute jobs.\n" % jobs_submitted
