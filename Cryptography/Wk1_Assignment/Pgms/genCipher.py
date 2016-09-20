#!/usr/bin/python
import sys

#MSGS = ( ---  11 secret messages  --- )
MSGS = ('secreat message one', 'message secreat two')

def strxor(a, b):     # xor two strings of different lengths
    if len(a) > len(b):
        return "".join([chr(ord(x) ^ ord(y)) for (x, y) in zip(a[:len(b)], b)])
    else:
        return "".join([chr(ord(x) ^ ord(y)) for (x, y) in zip(a, b[:len(a)])])

def random(size=16):
    return open("/dev/urandom").read(size)

def encrypt(key, msg):
    c = strxor(key, msg)
    print "msg: " + msg
    print "cipher: " + c.encode('hex')
    return c

def main():
    #key = random(100)
    key = "secreat"
    print "key: " + key
    ciphertexts = [encrypt(key, msg) for msg in MSGS]

main()
