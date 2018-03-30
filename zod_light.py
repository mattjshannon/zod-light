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
from ipdb import set_trace as st


class Observation(object):
    """Create an object to hold a Spitzer observation.

    e.g., C32-1.
    """

    def __init__(self, label, ra, dec, days, year, wave=None):
        """Initialize an instance of class Observation.

        Note:
            Assumes observation was taken with the Spitzer Space Telescope.

        Args:
            ra (str): HMS string of J2000 right ascension.
            dec (str): HMS string of J2000 declination.
            days (int): Number of days since Jan 1st of corresponding year.
            year (int): Year of observation.
            wave (numpy.ndarray), optional: Desired output spectrum wavelengths.
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
            self.wave_array = np.range(5, 40, 0.05) # default

        self._run_info()
        self.zody_array = self.construct_spectrum()            
        self.write_to_file(self.label + '.txt', directory='output_spectra/')

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
        os.chdir("model")

        run_list = ['./BackgroundModel',
                    str(self.ra_decimal), str(self.dec_decimal),
                    str(wave), str(self.days), str(self.year), 'sirtf']

        result = subprocess.run(run_list, stdout=subprocess.PIPE)

        # CD BACK???
        os.chdir("..")

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
            parallelize (bool), optional: Whether to parallelize the computation.

        Returns:
            wave_array (np.ndarray): Wavelengths.
            zody_array (np.ndarray): Zodiacal estimates in MJy/sr.
        """

        wave_array = self.wave_array

        if not parallelize:
            zody_array = [self.estimate_zodiacal_light(x) for x in wave_array]

        else:
            pool = ProcessPoolExecutor(max_workers=4)
            zody_array = list(pool.map(self.estimate_zodiacal_light, wave_array))

        return zody_array

    def write_to_file(self, filename, directory=''):
        """Write wave_array and zody_array to a file!

        Args:
            directory (str, optional): Defaults to current working directory.
            filename (str): Name of file to save to disk.

        Returns:
            True if successful, False otherwise.
        """
        save_array = np.vstack((self.wave_array, self.zody_array)).T

        my_header = 'wavelength (micron), zodiaical light estimate (MJy/sr)'
        np.savetxt(directory + filename, save_array, delimiter=',', header=my_header)
        print("Spectrum saved: ", directory + filename)

        return True


def main():
    print("Starting timer...")
    start_all = time.time()


    # Desired output spectrum parameters.
    wave_low = 5  # microns
    wave_high = 40  # microns
    wave_step = 0.1  # microns

    wave_arr = np.arange(wave_low, wave_high, wave_step)

    Observation("test_observation", "17:41:17.50", "-28:29:57.5", 109, 2005,
                wave=wave_arr)#, plot=True)

    # Observation("C32_1", "17:41:17.50", "-28:29:57.5", 109, 2005)
    # Observation("C32_2", "17:41:22.70", "-28:31:47.00", 109, 2005)
    # Observation("C32_3", "17:41:23.56", "-28:27:24.20", 109, 2005)
    # Observation("C32_4", "17:41:26.60", "-28:27:02.20", 109, 2005)
    # Observation("C32_5", "17:41:27.26", "-28:28:52.10", 109, 2005)
    # Observation("C32_6", "17:41:27.88", "-28:28:17.10", 109, 2005)
    # Observation("C32_7", "17:41:28.51", "-28:27:33.80", 109, 2005)
    # Observation("C32_8", "17:41:30.15", "-28:28:01.30", 109, 2005)
    # Observation("C32_9", "17:41:34.60", "-28:24:31.40", 109, 2005)
    # Observation("C32_10", "17:41:39.48", "-28:24:28.20", 109, 2005)
    # Observation("C32_11", "17:41:39.94", "-28:25:21.20", 109, 2005)
    # Observation("C32_12", "17:41:55.27", "-28:16:38.70", 109, 2005)
    # Observation("C32_13", "17:41:57.53", "-28:22:37.70", 109, 2005)
    # Observation("C32_14", "17:41:58.73", "-28:18:49.20", 109, 2005)
    # Observation("C32_15", "17:42:03.69", "-28:17:29.90", 109, 2005)
    # Observation("C32_16", "17:42:06.86", "-28:18:32.40", 109, 2005)

    # Observation("C35_1", "17:49:16.96", "-29:35:02.70", 109, 2005)
    # Observation("C35_2", "17:49:23.99", "-29:35:22.20", 109, 2005)
    # Observation("C35_3", "17:49:43.65", "-29:21:54.50", 109, 2005)
    # Observation("C35_4", "17:49:48.05", "-29:21:04.80", 109, 2005)
    # Observation("C35_5", "17:49:51.65", "-29:21:08.70", 109, 2005)

    # Observation("OGLE_1", "17:54:31.94", "-29:53:26.50", 109, 2005)
    # Observation("OGLE_2", "17:54:56.80", "-29:41:57.40", 109, 2005)
    # Observation("OGLE_3", "17:54:58.98", "-29:47:01.40", 109, 2005)
    # Observation("OGLE_4", "17:55:11.90", "-29:40:27.80", 109, 2005)
    # Observation("OGLE_5", "17:55:15.41", "-29:41:22.80", 109, 2005)
    # Observation("OGLE_6", "17:55:16.97", "-29:41:31.90", 109, 2005)
    # Observation("OGLE_7", "17:55:21.70", "-29:39:13.00", 109, 2005)

    # Observation("NGC6522_1", "18:02:34.78", "-29:59:58.90", 104, 2005)
    # Observation("NGC6522_2", "18:02:38.72", "-29:59:54.60", 104, 2005)
    # Observation("NGC6522_3", "18:02:48.90", "-29:54:31.00", 249, 2005)
    # Observation("NGC6522_4", "18:02:49.44", "-29:58:53.40", 249, 2005)
    # Observation("NGC6522_5", "18:02:59.51", "-30:02:54.30", 104, 2005)
    # Observation("NGC6522_6", "18:03:01.60", "-30:00:01.10", 249, 2005)
    # Observation("NGC6522_7", "18:03:04.80", "-29:52:59.30", 249, 2005)
    # Observation("NGC6522_8", "18:03:05.25", "-29:55:15.90", 104, 2005)
    # Observation("NGC6522_9", "18:03:05.33", "-29:55:27.80", 249, 2005)
    # Observation("NGC6522_10", "18:03:08.11", "-29:57:48.00", 249, 2005)
    # Observation("NGC6522_11", "18:03:08.52", "-30:05:26.50", 104, 2005)
    # Observation("NGC6522_12", "18:03:08.69", "-29:52:20.40", 104, 2005)
    # Observation("NGC6522_13", "18:03:11.47", "-29:57:47.20", 104, 2005)
    # Observation("NGC6522_14", "18:03:13.88", "-29:56:20.90", 249, 2005)
    # Observation("NGC6522_15", "18:03:15.99", "-29:55:38.30", 249, 2005)
    # Observation("NGC6522_16", "18:03:23.84", "-29:54:10.70", 249, 2005)
    # Observation("NGC6522_17", "18:03:28.36", "-29:55:45.40", 104, 2005)
    # Observation("NGC6522_18", "18:03:33.26", "-29:59:11.50", 249, 2005)
    # Observation("NGC6522_19", "18:03:34.07", "-29:59:58.80", 104, 2005)

    # Observation("NEP_C32C35OGLE", "18:00:00.00", "+66:33:38.55", 109, 2005)
    # Observation("NEP_NGC6522_a", "18:00:00.00", "+66:33:38.55", 104, 2005)
    # Observation("NEP_NGC6522_bc", "18:00:00.00", "+66:33:38.55", 249, 2005)

    elapsed = (time.time() - start_all)
    print("Elapsed time (s): ", elapsed)


if __name__ == '__main__':
    main()















