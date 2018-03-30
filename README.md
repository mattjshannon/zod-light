[![Documentation Status](https://readthedocs.org/projects/zod-light/badge/?version=latest)](http://zod-light.readthedocs.io/en/latest/?badge=latest)

# zod-light
A tool for estimating Galactic zodiacal light emission for Spitzer Space Telescope observations.

# Background
This is a Python wrapper for querying the Zodiacal Light Model (written in C) by W. Reach, J. Bennett, & B. Hartley (SSC): see
https://irsa.ipac.caltech.edu/data/SPITZER/docs/dataanalysistools/tools/contributed/general/zodiacallight/.

Please note that the C code has been modified to account for a couple of bugs, hence why the C code itself is included here (all rights of the C model remain with the authors as mentioned above).

# Typical use case
A common use for this package is to determine the zodiacal light towards a given position (RA, Dec) during a Spitzer observation. For instance, in [Shannon et al. (2018)][http://adsabs.harvard.edu/abs/2018ApJ...855...32S] we examined diffuse background emission from the Galactic bulge region. Since we were only interested in the background itself (and not the on-star observations), no background-subtraction was performed. Depending on how close to the Galactic center one looks, the zodiacal light can be an important if not dominant component of the mid-infrared continuum.

![Example use of zodiacal estimate](/docs/images/fig3.pdf)

# Installation
1. First, download this repository. Your best bet is to just clone it. From the terminal:
   - ``git clone https://github.com/mattjshannon/zod-light.git``

2. Next, you need to ensure that you have the **ephem** data, i.e.,
``model/ephem/de.bsp``
and
``model/ephem/sirtf.bsp``.
   - Note that these files are quite large (140MB and 225MB, respectively). They are linked here (in this repository) through the git-lfs (Large File Storage) service. However, if you do not have git-lfs installed (or if the bandwidth amount has been exceeded):
   - Extract them from the ``model/ephem/`` folder of the Spitzer-hosted (old) Zodiacal Light Model, which you can download from https://irsa.ipac.caltech.edu/data/SPITZER/docs/dataanalysistools/tools/contributed/general/zodiacallight/. Just dump them into the ``model/ephem/`` folder of this repository.

3. Next, you need to *make* the ZL Model. To do so:
   - ``cd model``
   - And followed by the appropriate command (choose as appropriate):
     - ``make mac``
     - ``make linux``
     - ``make solaris``


# Running an example


# Inputs

# How to format your queries

# Outputs





