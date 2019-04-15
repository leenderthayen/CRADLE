# -*- coding: utf-8 -*-
"""
Created on Mon Jan  4 15:34:48 2016

@author: leendert
"""

import numpy as np
import matplotlib.pyplot as plt

def DalitzPlot(name1, name2, name3):
    range1 = [i for i in range(len(names)) if names[i] == name1]
    range2 = [i for i in range(len(names)) if names[i] == name2]
    range3 = [i for i in range(len(names)) if names[i] == name3]
    r = set(range1) ^ set(range2)
    r = set(r) ^ set(range3)
    data1 = data[list(r),3:]
    data2 = data[list(r),3:]
    data3 = data[list(r),3:]
    
    s12 = data1 + data2
    s13 = data1 + data3
    s12 = [i[0]**2.0-sum(i[1:]**2.0) for i in s12]
    s13 = [i[0]**2.0-sum(i[1:]**2.0) for i in s13]
    
    plt.figure()
    plt.hist2d(s12, s13, bins=50)

def histParam(name):
    plt.figure()
    data1 = data[[i for i in range(len(names)) if names[i] == name],3]
    plt.hist(data1, bins=100)
    plt.xlabel('Kinetic Energy (keV)')
    plt.ylabel('Counts')
    plt.title(name)
    plt.figure()
    data2 = data[[i for i in range(len(names)) if names[i] == name],1]
    plt.hist(data2, bins=100)
    plt.xlabel('Time (s)')
    plt.ylabel('Counts')
    plt.title(name)
    print(name + " Total # events: " +  str(len(data1)))
    
def hist2DParam(name1, name2):
    plt.figure()
    range1 = [i for i in range(len(names)) if names[i] == name1]
    range2 = [i for i in range(len(names)) if names[i] == name2]
    r = set(range1) ^ set(range2)
    data1 = data[list(r),3]
    data2 = data[list(r),3]
    plt.hist2d(data1, data2, bins=5000)
    plt.xlabel(name1 + ' Kinetic Energy (keV)')
    plt.ylabel(name2 + ' Kinetic Energy (keV)')
    plt.title(name1 + ' & ' + name2)

plt.close('all')
plt.ion()

fileName = '../build/output_66Mn.txt'

data = np.genfromtxt(fileName, dtype=None)
names = [i[2] for i in data]
data = np.array([list(i)[:2] + list(i)[3:] for i in data])
data[np.isnan(data)]=0.

'''for i in range(4):

    firstEvent = data[data[:,0] == i]
    U,V,W = zip(*firstEvent[:,5:])
    X = np.zeros(len(U))
    Y = np.zeros(len(U))
    Z = np.zeros(len(U))
    
    plt.figure()
    ax = plt.gca()
    ax.quiver(X,Y,U,V,angles='xy',scale_units='xy',scale=1)
    ax.set_xlim([min(U)*1.1, max(U)*1.1])
    ax.set_ylim([min(V)*1.1, max(V)*1.1])
    
    print(sum(X))
    print(sum(Y))
    print(sum(Z))'''
    
for i in set(names): 
    histParam(i)

'''fileName2 = 'build/32Ar_scalar.txt'

data2 = np.genfromtxt(fileName2, dtype=None)
names2 = [i[2] for i in data2]
data2 = np.array([list(i)[:2] + list(i)[3:] for i in data2])
data2[np.isnan(data2)]=0.

plt.figure()
dataV = data[[i for i in range(len(names)) if names[i] == 'p'],3]
plt.hist(dataV, bins=100, histtype='step', label='Vector')
dataS = data2[[i for i in range(len(names2)) if names2[i] == 'p'],3]
plt.hist(dataS, bins=100, histtype='step', label='Scalar')
plt.xlabel('Kinetic Energy (keV)')
plt.ylabel('Counts')
plt.title('Comparison of emitted proton spectrum')
plt.legend()'''


