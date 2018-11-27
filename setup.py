#!/usr/bin/env python
# -*- coding: utf-8 -*-

from distutils.core import setup

exec(open('shear_bias/info.py').read())


def readme():
    with open('README.md') as f:
        return f.read()


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
      #scripts          = ['shear_bias/bin/{}'.format(fn) for fn in ['fits2ascii.py']],
)


