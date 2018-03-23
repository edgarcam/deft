from __future__ import division
import numpy as np
import matplotlib.pyplot as plt
import sys, glob
import colors

from matplotlib.colors import LightSource

densitycolormap = plt.cm.jet
densityinterpolation = 'bilinear'
densityshadedflag = True
densitybarflag = True
gridflag = True

filename = sys.argv[1]
Tmin = float(sys.argv[2])


for sad in glob.glob("data/gamma/%s/sad*.dat" % filename):
    data = np.loadtxt(sad)
    energies_found = data[:,0]
    t = data[:,1]
    ehi = data[:,2]
    elo = data[:,3]
    ts = np.exp(np.linspace(0, np.log(max(t)*5), 1000))
    gamma = np.zeros_like(ts)
    
    for j in range(len(t)):
        for i in range(len(gamma)):
            if ts[i] > t[j]:
                gamma[i] = energies_found[j]*(elo[j]-ehi[j])/ts[i]/3/Tmin
    sadname = sad.split('/')[-1].split('.')[0]
    colors.loglog(ts, gamma,sadname)
    plt.xlabel('Moves')
    plt.ylabel('$\gamma$')
    plt.title('Gamma versus Time')
    colors.legend()

def gamma_sa(t,t0):
    return t0/np.maximum(t, t0)

t0s = [1e3,1e4,1e5,1e6]
for t0 in t0s:
    colors.loglog(ts,gamma_sa(ts, t0),'samc-%g' %t0)
    colors.legend()


plt.savefig('figs/gamma-%s.pdf' % filename.replace('.','_'))
if 'noshow' not in sys.argv:
    plt.show()
