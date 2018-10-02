#!/usr/bin/env python3
"""
batch.py

Batch run of zod_light.py.
"""

import glob
import numpy as np

from datetime import date
from ipdb import set_trace as st
from zod_light import Observation


def extract_observation_properties(table):
    """Extract the RA, Dec and date information from the .tbl file."""

    def prep_label(table):
        """Extract a unique identifier."""
        label = table.split('/')[-1]

        return label

    def read_table(table):
        # Isolate the lines containing the date and coordinate information.
        with open(table) as f:
            for line in f:
                if line[:9] == '\\DATE_OBS':
                    date_str = line
                elif line[:9] == '\\CEN_SL1 ':
                    radec_str = line
                elif line[:9] == '\\CEN_SL2 ':
                    radec_str = line
                elif line[:9] == '\\CEN_SL3 ':
                    radec_str = line

        return date_str, radec_str

    def prep_date_str(date_str):
        """Process the date_str to number of days, year for zod-light."""

        # Extract the year, month, day of observation.
        ymd_str = date_str.split('\'')[1].split('T')[0]
        year, month, day = [int(x) for x in ymd_str.split('-')]

        # Compute the number of days since Jan. 1st of that year.
        obs_date = date(year, month, day)
        ref_date = date(year, 1, 1)
        days = (obs_date - ref_date).days

        return days, year

    def prep_radec_str(radec_str):
        """Extract the RA, Dec from radec_str in the right style."""

        # Isolate the part of the line that contains the info we need.
        sub_str = radec_str.split('\'')[1].split(' ')

        # Extract individual components.
        ra_hour = sub_str[0]
        ra_min = sub_str[1]
        ra_sec = sub_str[2]

        dec_deg = sub_str[4]
        dec_arcmin = sub_str[5]
        dec_arcsec = sub_str[6]

        # Combine in the desired format.
        ra = ra_hour + ':' + ra_min + ':' + ra_sec
        dec = dec_deg + ':' + dec_arcmin + ':' + dec_arcsec

        return ra, dec

    # Extract label.
    label = prep_label(table)

    # Extract the necessary lines.
    date_str, radec_str = read_table(table)

    # Parse the date information.
    days, year = prep_date_str(date_str)

    # Parse the RA, Dec information.
    ra, dec = prep_radec_str(radec_str)

    return label, ra, dec, days, year


def main():

    # Desired output spectrum parameters.
    wave_low = 5  # microns
    wave_high = 15  # microns
    wave_step = 0.1  # microns
    wave_arr = np.arange(wave_low, wave_high, wave_step)

    # Identify all input files.
    table_list = np.sort(glob.glob('../inputs/tbl/*_nd.tbl'))
    err_list = []

    # Create zodiacal estimate for all tables.
    for index, table in enumerate(table_list):

        # Extract the meta information.
        try:
            label, ra, dec, days, year = extract_observation_properties(table)
        except Exception as e:
            print(e)
            err_list.append(e)
            continue

        # Run!
        obs = Observation(label, ra, dec, days, year,
                          wave=wave_arr, verbose=False)
        obs.construct_spectrum(parallelize=True)
        obs.write_to_file()

    # Print the error list.
    print(err_list)

    st()


if __name__ == '__main__':
    main()
