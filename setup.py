#!/usr/bin/env python
# -*- coding: utf-8 -*-

from setuptools import setup, Extension, Command
from setuptools.command.build_ext import build_ext

#from distutils.core import setup
from distutils.version import LooseVersion
from distutils.core import Extension

import os
import subprocess
import platform
import sys

exec(open('shear_bias/info.py').read())


def readme():
    with open('README.md') as f:
        return f.read()


class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=''):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):
    def run(self):
        try:
            out = subprocess.check_output(['cmake', '--version'])
        except OSError:
            raise RuntimeError(
                "CMake must be installed to build the following extensions: " +
                ", ".join(e.name for e in self.extensions))

        if platform.system() == "Windows":
            cmake_version = LooseVersion(re.search(r'version\s*([\d.]+)',
                                         out.decode()).group(1))
            if cmake_version < '3.1.0':
                raise RuntimeError("CMake >= 3.1.0 is required on Windows")

        for ext in self.extensions:
            #self.build_extension(ext)
            pass

        # MKDEBUG added
        #subprocess.call(["make", "install"])


    def build_extension(self, ext):
        extdir = os.path.abspath(
            os.path.dirname(self.get_ext_fullpath(ext.name)))
        cmake_args = ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + extdir,
                      '-DPYTHON_EXECUTABLE=' + sys.executable]

        cfg = 'Debug' if self.debug else 'Release'
        build_args = ['--config', cfg]

        if platform.system() == "Windows":
            cmake_args += ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{}={}'.format(
                cfg.upper(),
                extdir)]
            if sys.maxsize > 2**32:
                cmake_args += ['-A', 'x64']
            build_args += ['--', '/m']
        else:
            cmake_args += ['-DCMAKE_BUILD_TYPE=' + cfg]
            build_args += ['--', '-j2']

            if 'CMAKE_INSTALL_PREFIX' in os.environ: #os.environ['CMAKE_INSTALL_PREFIX']:
                cmake_args += ['-DCMAKE_INSTALL_PREFIX={}'.format(os.environ['CMAKE_INSTALL_PREFIX'])]

        env = os.environ.copy()
        env['CXXFLAGS'] = '{} -DVERSION_INFO=\\"{}\\"'.format(
            env.get('CXXFLAGS', ''),
            self.distribution.get_version())
        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)
        subprocess.check_call(['cmake', ext.sourcedir] + cmake_args,
                              cwd=self.build_temp, env=env)
        subprocess.check_call(['cmake', '--build', '.'] + build_args,
                              cwd=self.build_temp)

        print('')  # Add an empty line for cleaner output

home = os.environ['HOME']

setup(name             = __whoami__,
      author           = __author__,
      packages         = [__whoami__],
      version          = __version__,
      description      = 'Shear bias estimation tools',
      long_description = readme(),
      url              = 'https://github.com/martinkilbinger/shear_bias',
      author_email     = __email__,
      platforms        = ['posix', 'mac os'],
      license          = "GNU GPLv3",
      classifiers      = [
                          'Programming Language :: Python',
                          'Natural Language :: English',
                         ],
      #ext_modules      = [CMakeExtension('{}/get_shapes'.format(__whoami__))],
      cmdclass         = dict(build_ext=CMakeBuild),
)


