"""
plotem.py

Take a look at the zody spectra produced from the SST bgmodel.
"""

import numpy as np
import matplotlib.pyplot as plt


def doit(theprefix, rangemax, color='k'):
    for i in np.arange(1, rangemax, 1):
        filein = theprefix + '_' + str(i) + '.txt'
        wavein, fluxin = np.loadtxt(filein, delimiter=',').T

        if i == 1 and theprefix=='C32':
            plt.plot(wavein, fluxin, color=color, label='C32 and C35')
        elif i == 1 and theprefix=='OGLE':
            plt.plot(wavein, fluxin, color=color, label=theprefix)
        else:
            plt.plot(wavein, fluxin, color=color)
    return


def doit2(theprefix, rangemax):
    for i in np.arange(1, rangemax, 1):
        filein = theprefix + '_' + str(i) + '.txt'
        wavein, fluxin = np.loadtxt(filein, delimiter=',').T
        if np.nanmax(fluxin) > 40:
            print(i)


        # if i == 1:
        plt.plot(wavein, fluxin, label=filein)
        # else:
        #     plt.plot(wavein, fluxin, color=color)
    return


def plot_ngc6522():
    for i in [1, 2, 5, 8, 11, 12, 13, 17, 19]:
        filein = 'NGC6522' + '_' + str(i) + '.txt'
        wavein, fluxin = np.loadtxt(filein, delimiter=',').T
        if i == 1:
            plt.plot(wavein, fluxin, color='r', label='NGC6522 (a)')
        else:
            plt.plot(wavein, fluxin, color='r')

    for i in [3, 4, 6, 7, 9, 10, 14, 15, 16, 18]:
        filein = 'NGC6522' + '_' + str(i) + '.txt'
        wavein, fluxin = np.loadtxt(filein, delimiter=',').T
        if i == 3:
            plt.plot(wavein, fluxin, color='k', label='NGC6522 (b) and (c)')
        else:
            plt.plot(wavein, fluxin, color='k')

    return            


def simpleplot(filein, color='k'):
    wavein, fluxin = np.loadtxt(filein, delimiter=',').T
    plt.plot(wavein, fluxin, color=color, label=filein)
    return


doit('OGLE', 8, color='blue')
doit('C32', 17, color='green')
doit('C35', 6, color='green')
# doit('NGC6522', 20, color='r')
# doit2('NGC6522', 20)
plot_ngc6522()

simpleplot('NEP_C32C35OGLE.txt', color='orange')
simpleplot('NEP_NGC6522_a.txt', color='yellow')
simpleplot('NEP_NGC6522_bc.txt', color='gray')



plt.xlabel(r'Wavelength ($\mu$m)')
plt.ylabel('Surface brightness (MJy/sr)')

plt.legend(loc=0)
plt.show()
plt.close()
