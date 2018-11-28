#!/usr/bin/env python
# -*- coding: utf-8 -*-

from distutils.core import setup
from distutils.core import Extension
import os

exec(open('shear_bias/info.py').read())


def readme():
    with open('README.md') as f:
        return f.read()


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
      ext_modules      = [
                          Extension(
                                    'get_shapes',
                                    ['src/get_shapes.cc'],
                                    include_dirs=[
                                                  '{}/.local/include'.format(home),
                                                  '/opt/local/include'],
                                    libraries=['cfitsio', 'shapelens'],
                                    library_dirs=['{}//astro/others/software/tmv/lib'.format(home),
                                                  '{}/.local/lib'.format(home),
                                                  '/opt/local/lib'],
                                   )
                         ],
      #scripts          = ['shear_bias/bin/{}'.format(fn) for fn in ['fits2ascii.py']],
)


