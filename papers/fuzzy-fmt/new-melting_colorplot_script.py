#!/usr/bin/python2
#NOTE: Run this plot script from directory deft/papers/fuzzy-fmt with command
# ./new-melting_anyplot_script.py [directory where data stored] --f[] [temp] --x[] --y[]
#to create plots from plot.dat files already in the data directory

import os
import argparse
import numpy as np
import matplotlib.pyplot as plt
import sys
import scipy.interpolate

parser = argparse.ArgumentParser(description='Creates a plot.', epilog="stuff...")
groupf = parser.add_mutually_exclusive_group(required=True)
groupx = parser.add_mutually_exclusive_group(required=True)
groupy = parser.add_mutually_exclusive_group(required=True)

parser.add_argument('directory', metavar='exisiting_data_directory', type=str,
                    help='exisiting directory/filename for data file') 
groupf.add_argument('--ftemp', action="store_true",
                    help='use plot.dat file with this fixed temperature')
groupf.add_argument('--fdensity', action="store_true",
                    help='use plot.dat file with this fixed temperature') 
parser.add_argument('value', metavar='value_of_fixed_quantity', type=str,
                    help='use plot.dat file with this fixed value')
parser.add_argument('--xlab', metavar='label for x-axis', type=str,
                    help='label for x-axis. use with --xcol.') 
parser.add_argument('--ylab', metavar='label for y-axis', type=str,
                    help='label for y-axis. use with --ycol.') 
                    
                     
groupx.add_argument('--xtemp', action="store_true",
                    help='temperature on x-axis') 
groupx.add_argument('--xdensity', action="store_true",
                    help='density on x-axis') 
groupx.add_argument('--xcfe', action="store_true",
                    help='crystal free energy/atom on x-axis') 
groupx.add_argument('--xhfe', action="store_true",
                    help='homogeneous free energy/atom on x-axis') 
groupx.add_argument('--xdiff', action="store_true",
                    help='diff in free energy on x-axis')   
groupx.add_argument('--xcfev', action="store_true",
                    help='crystal energy/volume on x-axis') 
groupx.add_argument('--xfv', action="store_true",
                    help='fraction of vacancies (fv) on x-axis') 
groupx.add_argument('--xgw', action="store_true",
                    help='Gaussian width on x-axis')
groupx.add_argument('--xcol', metavar='column for x-axis data', type=int,
                    help='column for x-axis data') 

groupy.add_argument('--ytemp', action="store_true",
                    help='temperature on y-axis') 
groupy.add_argument('--ydensity', action="store_true",
                    help='density on y-axis') 
groupy.add_argument('--ycfe', action="store_true",
                    help='crystal free energy/atom on y-axis') 
groupy.add_argument('--yhfe', action="store_true",
                    help='homogeneous free energy/atom on y-axis') 
groupy.add_argument('--ydiff', action="store_true",
                    help='diff in free energy on y-axis')   
groupy.add_argument('--ycfev', action="store_true",
                    help='crystal energy/volume on y-axis') 
groupy.add_argument('--yfv', action="store_true",
                    help='fraction of vacancies (fv) on y-axis') 
groupy.add_argument('--ygw', action="store_true",
                    help='Gaussian width on y-axis')
groupy.add_argument('--ycol', metavar='column for y-axis data', type=int,
                    help='column for y-axis data') 

args=parser.parse_args()
#print
#print args

data_directory=args.directory

if args.ftemp:
    fixed_quantity="kT"
elif args.fdensity:
    fixed_quantity="n"
    
fixed_value=args.value

#data_file=data_directory+"/plot_alldat".dat"
data_file=data_directory
thisdata = np.loadtxt(data_file)
print
print "Using data from file:"+data_file
print

if args.xtemp:  
    x_axis=thisdata[:,0]    
    x_label="Temperature (kT)"
    x_plot="kT"
elif args.xdensity:  
    x_axis=thisdata[:,1]     
    x_label="Reduced Density (n)"
    x_plot="n"
elif args.xfv:  
    x_axis=thisdata[:,2]     
    x_label="Fraction of vacancies (fv)"
    x_plot="fv"
elif args.xgw:   
    x_axis=thisdata[:,3]     
    x_label="Width of Gaussian (gwidth)"
    x_plot="gw"
elif args.xhfe:  
    x_axis=thisdata[:,4]     
    x_label="Homogeneous Free Energy/atom" 
    x_plot="hFE"
elif args.xcfe:  
    x_axis=thisdata[:,5]     
    x_label="Crystal Free Energy/atom"
    x_plot="cFE"
elif args.xdiff:  
    x_axis=thisdata[:,6]     
    x_label="Diff=(CrystalFE-HomogeneousFE)/atom" 
    x_plot="DiffFE"
elif args.xcfev:  
    x_axis=thisdata[:,10]     
    x_label="Crystal Free Energy/volume"
    x_plot="cFEv"
elif args.xcol:   
    x_axis=thisdata[:,args.xcol]     
    x_label=args.xlab
    x_plot=args.xlab
    
if args.ytemp:  
    y_axis=thisdata[:,0]     
    y_label="Temperature (kT)"
    y_plot="kT"
elif args.ydensity:  
    y_axis=thisdata[:,1]     
    y_label="Reduced Density (n)"
    y_plot="n"
elif args.yfv:  
    y_axis=thisdata[:,2]     
    y_label="Fraction of vacancies (fv)"
    y_plot="fv"
elif args.ygw:   
    y_axis=thisdata[:,3]
    y_label="Width of Gaussian (gwidth)"
    y_plot="gw"
elif args.yhfe:  
    y_axis=thisdata[:,4]    
    y_label="Homogeneous Free Energy/atom"
    y_plot="hFE" 
elif args.ycfe:  
    y_axis=thisdata[:,5]     
    y_label="Crystal Free Energy/atom"
    y_plot="cFE"
elif args.ydiff:  
    y_axis=thisdata[:,6]  
    y_label="Diff=(CrystalFE-HomogeneousFE)/atom" 
    y_plot="DiffFE"
elif args.ycfev:  
    y_axis=thisdata[:,10]    
    y_label="Crystal Free Energy/volume"
    y_plot="cFEv"
elif args.ycol:   
    y_axis=thisdata[:,args.ycol]     
    y_label=args.ylab
    y_plot=args.ylab
    
 
z_axis=thisdata[:,6]      #plots energy diff/atom
#z_label="Crystal Free Energy/atom"
#z_plot="cFE"

plot_name=data_directory+"/plot_color_"+y_plot+"vs"+x_plot+"_"+fixed_quantity+fixed_value+".png"
plot_title=y_label+" vs "+x_label+" at Fixed "+fixed_quantity+"="+fixed_value

#Plot x-axis vs y-axis with z values in color
#plt.plot(x_axis, y_axis, color="purple")

#x,y = np.mgrid[0:0.99:0.01, 0.01:1.41:0.1]
#xi=np.linspace(0, 0.99, 0.01)
#yi=np.linspace(0.01, 1.41, 0.1)
xi = x_axis
yi= y_axis
xi, yi = np.meshgrid(xi, yi)
x=x_axis
y=y_axis
z=z_axis
print "length of xi", len(xi)
print x.min()
print "length of x", len(x)
print "length of y", len(y)
print "length of z", len(z)
print "z_axis", z_axis

zi = scipy.interpolate.griddata((x, y), z, (xi, yi), method='nearest')

print "zi", zi
print len(zi)

#Debug ----------------------
for i in range(0, (len(x)-1)):   
    print x[i], y[i], z[i] 
#put this into a data file!
#----------------------------

print "z min", z.min()
print "z max", z.max()

#plt.imshow(zi, vmin=zi.min(), vmax=zi.max(), origin='lower', extent=[x.min(), x.max(), y.min(), y.max()]) 

#plt.colorbar()

#plt.title(plot_title)
#plt.xlabel(x_label)
#plt.ylabel(y_label)
#plt.savefig(plot_name)

#plt.show()
#fig, ax = plt.subplots()
#cmap = mp1.colors.ListedColormap(['red', 'blue'])
#bounds = [-2, 0, 2]
#norm =mp1.colors.BoundaryNorm(bounds, cmap.N)
#cb2 = mp1.colorbar.ColorbarBase(ax, cmap=cmap, norm=norm, boundaries=[0] + bounds + [13], extend='both', ticks=bounds, spacing='proportional', orientation='vertical')
#cb2.set_label('DiffFE')
#fig.show
##contour=plt.contour(x,y,zi, levels, colors='k')
##plt.clabel(contour, colors = 'k', fmt = '%2.1f', fontsize= 12)
#contour_filled = plt.contourf(x,y,zi, levels)
#plt.colorbar()

#plt.show()
plt.title("Crystal Free Energy, kT=2, n=0.92")
plt.xlabel("fv")
plt.ylabel("gw")
levels = [-20, -10, -5, 0, 99,100]
plt.scatter(x,y,c=z)
plt.colorbar()
plt.show()
