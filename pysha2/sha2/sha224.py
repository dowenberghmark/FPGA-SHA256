#!/usr/bin/python
__author__ = 'Thomas Dixon'
__license__ = 'MIT'

from sha256 import sha256

def new(m=None):
    return sha224(m)

class sha224(sha256):
    _h = (0xc1059ed8L, 0x367cd507L, 0x3070dd17L, 0xf70e5939L,
          0xffc00b31L, 0x68581511L, 0x64f98fa7L, 0xbefa4fa4L)
    _output_size = 7
    digest_size = 28
