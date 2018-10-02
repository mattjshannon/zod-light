#!/usr/bin/env python3
"""
compare.py

Overplot the Spitzer spectra with the zodiacal estimates.
"""

import glob
import matplotlib.pyplot as plt
import numpy as np

from astropy.table import Table
from ipdb import set_trace as st


def load_zody(zod_file):
    """Load the zodiacal estimate."""
    wave, zody = np.loadtxt(zod_file, delimiter=',').T

    return wave, zody


def load_spec(tbl_file):
    """Load the IRS spectrum."""

    def create_dict(wavespec, flux, fluxerr, orders, order_num=0):

        rdx = np.where(orders == order_num)

        SL_dict = {
            'wavespec': wavespec[rdx],
            'flux': flux[rdx],
            'fluxer': fluxerr[rdx]
        }

        return SL_dict

    # Load table.
    # table = atpy.Table(tbl_file)
    table = Table.read(tbl_file, format='ascii')

    # Identify vectors.
    wavespec = np.array(table['WAVELENGTH'])
    flux = np.array(table['FLUX'])
    fluxerr = np.array(table['FLUX_UNCERTAINTY'])
    orders = np.array(table['ORDER'])

    # Split and save to dicts.
    SL1 = create_dict(wavespec, flux, fluxerr, orders, order_num=1)
    SL2 = create_dict(wavespec, flux, fluxerr, orders, order_num=2)
    SL3 = create_dict(wavespec, flux, fluxerr, orders, order_num=3)

    return SL1, SL2, SL3


def create_figure(base_str, wave, zody, SL1, SL2, SL3):
    """Plot the spectra and save to a PDF."""

    fig, ax = plt.subplots()

    ax.plot(wave, zody, label='Zodiacal estimate')
    ax.plot(SL1['wavespec'], SL1['flux'], '-', label='SL1')
    ax.plot(SL2['wavespec'], SL2['flux'], '-', label='SL2')
    ax.plot(SL3['wavespec'], SL3['flux'], '-', label='SL3')
    ax.set_title(base_str, fontsize=12)
    ax.set_xlabel(r'Wavelength ($\mu$m)')
    ax.set_ylabel('Surface brightness (MJy/sr)')
    ax.minorticks_on()
    ax.axhline(y=0, ls='--')
    ax.legend(loc=0)

    savename = 'plots/' + base_str + '.pdf'
    fig.savefig(savename, format='pdf', bbox_inches='tight')

    plt.close()
    fig.clear()

    return


zod_list = np.sort(glob.glob('tbl_zodiacal/*_nd.tbl.zod'))

# Iterate over the list of all zodiacal estimate files.
for index, zod_file in enumerate(zod_list):

    # Keep track of how long this is gonna take...
    if index % 5 == 0:
        print(index, ' / ', len(zod_list) + 1)

    # Isolate raw string.
    tbl_file = zod_file.split('.zod')[0].replace('_zodiacal', '')
    base_str = tbl_file.split('/')[-1]

    # Read in zodiacal estimate.
    wave, zody = load_zody(zod_file)

    # Load spectrum associatd with the zod file.
    SL1, SL2, SL3 = load_spec(tbl_file)

    # Plot them and save a pdf.
    create_figure(base_str, wave, zody, SL1, SL2, SL3)





    


