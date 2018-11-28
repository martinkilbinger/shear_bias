# Shear bias

`shear_bias` is a package that contains tools and scripts for shear bias
estimation for weak gravitational lensing analysis.

## Information

### Authors
  - Martin Kilbinger `martin.kilbinger@cea.fr`
  - Arnau Pujol `arnaupv@gmail.com`

Version: 0.1

Date: November 2019

Documentation: TBD

## Installation

Download the code from the `github` repository.

```bash

git clone https://github.com/martinkilbinger/shear_bias
```

A directory `shear_bias` is created. There, call the setup script to install the
package.

```bash
cd shear_bias
python setup.py install
```

You might have to use `sudo` in front of the setup command, if you have the root password.
Alternatively, you can install the package from within a virtual environment, or use
the option `--prefix DIR` to install it in a directory `DIR` of your choice.

### External programs and libraries

The following packages should be installed:
  - `galsim` program
  - `shapelens` library
    Its installation requires the `cfitsio' and `tmv` libraries

## Content

  - `shear_bias`
    Python scripts with the shear_bias module functions and classes.
  - `notebooks`
    Jupyter notebooks and auxilliary files
    - `shear_bias_example.ipynb`
      Example notebook using galsim and KSB-shapelens
    - `config/galsim`
      Galsim configuration files (.yaml, to use with galsim on the command line)
    - `src`
      Contains `get_shapes.cc`, the source file for the KSB-shapelens executable.


## Reference

Pujol, Kilbinger, Sureau & Bobin (2018),
https://arxiv.org/abs/1806.10537,
http://cdsads.u-strasbg.fr/abs/2018arXiv180610537P
