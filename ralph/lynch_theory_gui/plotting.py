# -*- coding: utf-8 -*-
"""
Created on Tue Apr 21 15:07:02 2015

@author: ralph
"""

import numpy as np
import matplotlib.pyplot as plt
import pylab as p
import os


def diameter(xmin,xmax, lam, f, T,SDD, filename):
    x = 0
    
    DFI = np.zeros(len(np.linspace(2*(10**-10),5*(10**-10),num=1000)))
    xx = np.zeros(len(np.linspace(2*(10**-10),5*(10**-10),num=1000)))
    m = np.zeros(len(np.linspace(2*(10**-10),5*(10**-10),num=1000)))
    

    for D in np.linspace(xmin, xmax, num=1000):

        xi = (np.sqrt(1-(lam*lam*2.58999e13)))-(np.sqrt(1-(lam*lam*2.78215e11)))
        dd = (lam*SDD)/(4*10**-6)

        DD = D/dd

        
        if D>dd:
            klammer = DD-(np.sqrt((DD*DD)-1)*(1+(1/(2*DD*DD))))+(((1/DD)-1/(4*DD*DD*DD))*(np.log((DD+np.sqrt(DD*DD-1))/(DD-np.sqrt(DD*DD-1)))))
            mu = ((3*np.pi**2)/lam**2)*f*(np.abs(xi)**2)*dd*klammer

        else:
            mu = ((3*np.pi**2)/lam**2)*f*((np.abs(xi))**2)*dd*DD

        
        DFI[x] = np.exp(-mu*T)
        m[x] = mu
        xx[x] = D
        x = x+1
    
    #plt.plot(xx,DFI)
    plt.xlabel('Particle Diameter')
    plt.ylabel('DFI signal')
    directory = os.getcwd()
    if filename != '':
        file = open(directory+'/'+filename + '.jpg', 'w+')
        p.savefig(directory+'/'+filename + '.jpg')
        file.close()
    #plt.show()
    return xx, DFI
    

def wavelength(xmin,xmax, D, f, T,SDD, filename):
    x = 0
    
    DFI = np.zeros(len(np.linspace(2*(10**-10),5*(10**-10),num=1000)))
    xx = np.zeros(len(np.linspace(2*(10**-10),5*(10**-10),num=1000)))
    m = np.zeros(len(np.linspace(2*(10**-10),5*(10**-10),num=1000)))
    
    for lam in np.linspace(xmin,xmax,num=1000):
                          # concentration
        xi = (np.sqrt(1-(lam*lam*2.58999e13)))-(np.sqrt(1-(lam*lam*2.78215e11)))
        dd = (lam*SDD)/(4*10**-6)
                    
        DD = D/dd

        
        if D>dd:
            klammer = DD-(np.sqrt((DD*DD)-1)*(1+(1/(2*DD*DD))))+(((1/DD)-1/(4*DD*DD*DD))*(np.log((DD+np.sqrt(DD*DD-1))/(DD-np.sqrt(DD*DD-1)))))
            mu = ((3*np.pi**2)/lam**2)*f*(np.abs(xi)**2)*dd*klammer

        else:
            mu = ((3*np.pi**2)/lam**2)*f*((np.abs(xi))**2)*dd*DD

        
        DFI[x] = np.exp(-mu*T)
        m[x] = mu
        xx[x] = lam
        x = x+1 
    plt.plot(xx,DFI)
    plt.xlabel('Wavelength')
    plt.ylabel('DFI signal')
    directory = os.getcwd()
    if filename != '':
        file = open(directory+'/'+filename + '.jpg', 'w+')
        p.savefig(directory+'/'+filename + '.jpg')
        file.close()
    #plt.show()
    return xx, DFI

def concentration(xmin,xmax, D, lam, T,SDD, filename):
    x = 0
    
    DFI = np.zeros(len(np.linspace(2*(10**-10),5*(10**-10),num=1000)))
    xx = np.zeros(len(np.linspace(2*(10**-10),5*(10**-10),num=1000)))
    m = np.zeros(len(np.linspace(2*(10**-10),5*(10**-10),num=1000)))
    
    for f in np.linspace(xmin,xmax,num=1000):
        xi = (np.sqrt(1-(lam*lam*2.58999e13)))-(np.sqrt(1-(lam*lam*2.78215e11)))
        dd = (lam*SDD)/(4*10**-6)
        DD = D/dd
        
        
        if D>dd:
            klammer = DD-(np.sqrt((DD*DD)-1)*(1+(1/(2*DD*DD))))+(((1/DD)-1/(4*DD*DD*DD))*(np.log((DD+np.sqrt(DD*DD-1))/(DD-np.sqrt(DD*DD-1)))))
            mu = ((3*np.pi**2)/lam**2)*f*(np.abs(xi)**2)*dd*klammer
        else:
            mu = ((3*np.pi**2)/lam**2)*f*((np.abs(xi))**2)*dd*DD

        
        DFI[x] = np.exp(-mu*T)
        m[x] = mu
        xx[x] = f
        x = x+1 
    plt.plot(xx,DFI)
    plt.xlabel('Concentration')
    plt.ylabel('DFI signal')
    directory = os.getcwd()
    if filename != '':
        file = open(directory+'/'+filename + '.jpg', 'w+')
        p.savefig(directory+'/'+filename + '.jpg')
        file.close()
    return xx, DFI
    
def thickness(xmin,xmax, D, lam, f,SDD, filename):
    x = 0
    
    DFI = np.zeros(len(np.linspace(2*(10**-10),5*(10**-10),num=1000)))
    xx = np.zeros(len(np.linspace(2*(10**-10),5*(10**-10),num=1000)))
    m = np.zeros(len(np.linspace(2*(10**-10),5*(10**-10),num=1000)))
    
    for T in np.linspace(xmin,xmax,num=1000):
        xi = (np.sqrt(1-(lam*lam*2.58999e13)))-(np.sqrt(1-(lam*lam*2.78215e11)))
        dd = (lam*SDD)/(4*10**-6)
        DD = D/dd
        
        
        if D>dd:
            klammer = DD-(np.sqrt((DD*DD)-1)*(1+(1/(2*DD*DD))))+(((1/DD)-1/(4*DD*DD*DD))*(np.log((DD+np.sqrt(DD*DD-1))/(DD-np.sqrt(DD*DD-1)))))
            mu = ((3*np.pi**2)/lam**2)*f*(np.abs(xi)**2)*dd*klammer
        else:
            mu = ((3*np.pi**2)/lam**2)*f*((np.abs(xi))**2)*dd*DD

        
        DFI[x] = np.exp(-mu*T)
        m[x] = mu
        xx[x] = T
        x = x+1 
    plt.plot(xx,DFI)
    plt.xlabel('Thickness')
    plt.ylabel('DFI signal')
    directory = os.getcwd()
    if filename != '':
        file = open(directory+'/'+filename + '.jpg', 'w+')
        p.savefig(directory+'/'+filename + '.jpg')
        file.close()
    return xx, DFI
    
def SDD(xmin,xmax, D, lam, f,T, filename):
    x = 0
    
    DFI = np.zeros(len(np.linspace(2*(10**-10),5*(10**-10),num=1000)))
    xx = np.zeros(len(np.linspace(2*(10**-10),5*(10**-10),num=1000)))
    m = np.zeros(len(np.linspace(2*(10**-10),5*(10**-10),num=1000)))
    
    for SDD in np.linspace(xmin,xmax,num=1000):
        xi = (np.sqrt(1-(lam*lam*2.58999e13)))-(np.sqrt(1-(lam*lam*2.78215e11)))
        dd = (lam*SDD)/(4*10**-6)
        DD = D/dd
        
        
        if D>dd:
            klammer = DD-(np.sqrt((DD*DD)-1)*(1+(1/(2*DD*DD))))+(((1/DD)-1/(4*DD*DD*DD))*(np.log((DD+np.sqrt(DD*DD-1))/(DD-np.sqrt(DD*DD-1)))))
            mu = ((3*np.pi**2)/lam**2)*f*(np.abs(xi)**2)*dd*klammer
        else:
            mu = ((3*np.pi**2)/lam**2)*f*((np.abs(xi))**2)*dd*DD

        
        DFI[x] = np.exp(-mu*T)
        m[x] = mu
        xx[x] = SDD
        x = x+1 
    plt.plot(xx,DFI)
    plt.xlabel('SDD')
    plt.ylabel('DFI signal')
    directory = os.getcwd()
    if filename != '':
        file = open(directory+'/'+filename + '.jpg', 'w+')
        p.savefig(directory+'/'+filename + '.jpg')
        file.close()
    return xx, DFI
#np.savetxt("foo.csv", DFI, delimiter=",")


#concentration(0.01,0.2,2e-6,4.1e-10,0.005,0.025)