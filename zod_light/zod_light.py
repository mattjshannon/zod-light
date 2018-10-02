#!/usr/bin/env python
"""
automate.py

Calculate the zodiacal light estimate automatically for my SST observations.
"""

import numpy as np
import os
import subprocess
import time

from astropy import units as u
from astropy.coordinates import SkyCoord
from concurrent.futures import ProcessPoolExecutor


class Observation(object):
    """Create an object to hold a Spitzer observation.

    e.g., C32-1.
    """

    def __init__(self, label, ra, dec, days, year, wave=None, verbose=False):
        """Initialize an instance of class Observation.

        Note:
            Assumes observation was taken with the Spitzer Space Telescope.

        Args:
            ra (str): HMS string of J2000 right ascension.
            dec (str): HMS string of J2000 declination.
            days (int): Number of days since Jan 1st of corresponding year.
            year (int): Year of observation.
            wave (numpy.ndarray), optional: Desired output spectrum wavelength.
        """
        self.label = label
        self.ra = ra
        self.dec = dec
        self.ra_decimal, self.dec_decimal = self.to_decimal()
        self.days = days
        self.year = year

        if wave is not None:
            self.wave_array = wave
        else:
            self.wave_array = np.range(5, 40, 0.05)  # default

        if verbose:
            self._run_info()

        # self.zody_array = self.construct_spectrum()
        # self.write_to_file(self.label + '.txt', directory='../outputs/')

    def _run_info(self):
        print()
        print('Observation name: ', self.label)
        print('RA: ', self.ra)
        print('Dec: ', self.dec)
        print('Date: ', self.days, ' days into ', self.year)
        return

    def to_decimal(self):
        """Convert the RA/DEC into decimal form from HMS.

        Returns:
            ra_decimal (str): J2000 RA in decimal form.
            dec_decimal (str): J2000 declination in decimal form.
        """
        # convert ra and dec strings into decimals here.

        coords = SkyCoord(self.ra + " " + self.dec, unit=(u.hourangle, u.deg))
        return coords.ra.degree, coords.dec.degree

    def estimate_zodiacal_light(self, wave):
        """Estimate the zodiacal light in MJy/sr.

        Args:
            wave (float): Wavelength for estimate.

        Returns:
            zody (float): Zodiacal light estimate.
        """
        stdout = self._query_bash(wave)
        zody = self._parse_bash_output(stdout)
        return zody

    def _query_bash(self, wave):
        """Run zodiacal light model and capture output.

        Args:
            wave (float): Wavelength for estimate.

        Returns:
            zody (float): Zodiacal light estimate in MJy/sr.
        """

        # CD ???
        os.chdir("../model/")

        run_list = ['./BackgroundModel',
                    str(self.ra_decimal), str(self.dec_decimal),
                    str(wave), str(self.days), str(self.year), 'sirtf']

        result = subprocess.run(run_list, stdout=subprocess.PIPE)

        # CD BACK???
        os.chdir("../zod_light/")

        return result.stdout

    def _parse_bash_output(self, parse_string):
        """Parse output from _query_bash to determine the total zodiacal light.

        Args:
            bash_stdout (str): Output from _query_bash with BackgroundModel.

        Return:
            zody_mjysr (float): Background emission in MJy/sr.
        """
        zody_str = str(parse_string).split('zody = ')[1].split('\\nism')[0]
        zody_mjysr = float(zody_str)
        return zody_mjysr

    def construct_spectrum(self, parallelize=False):
        """Construct the specturm by iterating over wavelengths.

        Args:
            parallelize (bool), optional: Whether to parallelize the
                computation.

        Returns:
            wave_array (np.ndarray): Wavelengths.
            zody_array (np.ndarray): Zodiacal estimates in MJy/sr.
        """

        wave_array = self.wave_array

        if not parallelize:
            zody_array = [self.estimate_zodiacal_light(x) for x in wave_array]

        else:
            pool = ProcessPoolExecutor(max_workers=4)
            zody_array = \
                list(pool.map(self.estimate_zodiacal_light, wave_array))

        self.zody_array = np.array(zody_array)

        return

    def write_to_file(self, filename=None, directory=''):
        """Write wave_array and zody_array to a file!

        Args:
            directory (str, optional): Defaults to current working directory.
            filename (str): Name of file to save to disk.

        Returns:
            True if successful, False otherwise.
        """
        save_array = np.vstack((self.wave_array, self.zody_array)).T

        directory = '../outputs/'
        filename = self.label + '.zod'

        my_header = 'Wavelength (micron), Zodiacal light estimate (MJy/sr)'
        np.savetxt(directory + filename, save_array, delimiter=',',
                   header=my_header)
        print("Spectrum saved: ", directory + filename)

        return True


def main():
    print("Starting timer...")
    start_all = time.time()

    # Desired output spectrum parameters.
    wave_low = 5  # microns
    wave_high = 15  # microns
    wave_step = 0.1  # microns

    wave_arr = np.arange(wave_low, wave_high, wave_step)

    Observation("test_observation", "17:41:17.50", "-28:29:57.5", 109, 2005,
                wave=wave_arr)  # , plot=True)

    elapsed = (time.time() - start_all)
    print("Elapsed time (s): ", elapsed)


if __name__ == '__main__':
    main()















