import sys
import gmpy2
from gmpy2 import mpz


#--------------------------------------------------------------
# Discrete Logarithm Implementation
#--------------------------------------------------------------
class DiscreteLog(object):

    def __init__(self):
        self.prime = 0
        self.element = 0
        self.exponent = 0
        self.h_by_gx_hash = {}
        self.B = 2 ** 20

    def setPrime(self, prime_):
        self.prime = prime_

    def setElement(self, element_):
        self.element = element_

    def setExponent(self, exponent_):
        self.exponent = exponent_

    def buildHash_h_by_gx(self):
        for x1 in range (0, self.B + 1):
            expo_gx1 = gmpy2.powmod(mpz(self.element), x1, mpz(self.prime))
            inv_gx1 = gmpy2.invert(mpz(expo_gx1), mpz(self.prime))
            left_x1 = gmpy2.t_mod(gmpy2.mul(mpz(self.exponent), inv_gx1), mpz(self.prime))
            self.h_by_gx_hash[left_x1] = x1

    def findDiscreteLog(self):
        x = -1
        element_powB = gmpy2.powmod(mpz(self.element), mpz(self.B), mpz(self.prime))
        for x0 in range (0, self.B + 1):
            right_x0 = gmpy2.powmod(element_powB, x0, mpz(self.prime))
            if right_x0 in self.h_by_gx_hash:
                 print("x0: %d" % (x0))
                 x1 = self.h_by_gx_hash[right_x0]
                 print("x1: %d" % (x1))
                 x = gmpy2.t_mod(gmpy2.add(gmpy2.mul(x0, self.B), x1), mpz(self.prime))
                 print("x: %d" % (x))
                 return str(x)
        return x
			

if __name__ == "__main__":

    # Instantiate Padding Oracle object
    dl = DiscreteLog()
    print("Prime: (p) " + sys.argv[1])
    dl.setPrime(sys.argv[1])
    print("Element: (g) " + sys.argv[2])
    dl.setElement(sys.argv[2])
    print("Exponent: (h) " + sys.argv[3])
    dl.setExponent(sys.argv[3])
    dl.buildHash_h_by_gx()
    x = dl.findDiscreteLog()

    #Verify the output by checking whether g^x = h
    h = gmpy2.powmod(mpz(sys.argv[2]), mpz(x), mpz(sys.argv[1]))
    print(h)
    if mpz(h) == mpz(sys.argv[3]):
        outMsg = "DiscreteLog of " + sys.argv[3] + " in Zp for p = " + sys.argv[1] + " and element g = " + sys.argv[1] + " is : "
        print(outMsg)
        print(x)
    else :
        print("We got the calculation wrong")

