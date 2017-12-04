#!/usr/bin/python
__author__ = 'Thomas Dixon'
__license__ = 'MIT'

from sha512 import sha512

def new(m=None):
    return sha384(m)

class sha384(sha512):
    _h = (0xcbbb9d5dc1059ed8L, 0x629a292a367cd507L, 0x9159015a3070dd17L, 0x152fecd8f70e5939L,
          0x67332667ffc00b31L, 0x8eb44a8768581511L, 0xdb0c2e0d64f98fa7L, 0x47b5481dbefa4fa4L)
    _output_size = 6
    digest_size = 48
