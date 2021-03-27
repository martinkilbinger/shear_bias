# Shear bias

[![arXiv](https://img.shields.io/badge/arXiv-1806.10537-red.svg)](https://arxiv.org/abs/1806.10537)

`shear_bias` is a package that contains tools and scripts for shear bias
estimation for weak gravitational lensing analysis.

## Information

### Authors
  - Martin Kilbinger `martin.kilbinger@cea.fr`
  - Arnau Pujol `arnaupv@gmail.com`

Version: 0.4

Date: March 2021

Documentation: TBD

## Installation

Download the code from the `github` repository.

```bash

git clone https://github.com/cosmostat/shear_bias
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

## Content

  - `shear_bias`
    Python scripts with the shear_bias module functions and classes.
  - `notebooks`
    Jupyter notebooks
    - `shear_bias_example.ipynb`
      Example notebook using galsim


## Reference

Pujol, Kilbinger, Sureau & Bobin (2018),
[arXiv:1806.10537](https://arxiv.org/abs/1806.10537),
[ads](http://cdsads.u-strasbg.fr/abs/2018arXiv180610537P)
