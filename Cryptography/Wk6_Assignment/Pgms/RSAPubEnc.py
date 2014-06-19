import sys
import math
import re
import gmpy2
from gmpy2 import mpz


#--------------------------------------------------------------
# RSA Public Encryption System
#--------------------------------------------------------------
class RSAPublicEnc(object):

    def __init__(self):
        #set the precision for gmpy to very high
        gmpy2.get_context().precision=5000
        self.N = 0
        self.p = 0
        self.q = 0
        self.e = 0
        self.d = 0
        self.pi_of_N = 0
        self.pkcsByte = "02"
        self.separatorByte = "00"

    def setModulusN(self, n_):
        self.N = mpz(n_)

    def setP(self, p_):
        self.p = mpz(p_)

    def setQ(self, q_):
        self.q = mpz(q_)

    def setEncyptionExponent(self, e_):
        self.e = mpz(e_)
        self.d = self.findExponentInverse(self.e)
        #print("pi_of_N: " + str(self.pi_of_N))
        #print("e: " + str(self.e))
        #print("d: " + str(self.d))

    def setDecyptionExponent(self, d_):
        self.d = mpz(d_)
        self.e = findExponentInverse(self, self.d)

    def findExponentInverse(self, expo_):
        #pi_of_N = (p - 1)(q-1)
        self.pi_of_N = gmpy2.mul(gmpy2.sub(self.p, mpz(1)), gmpy2.sub(self.q, mpz(1)))
        #e.d = 1 mod pi_of_N ==> e = 1/d mod pi_of_N AND d = 1/e mod pi_of_N
        expo_inv = gmpy2.invert(mpz(expo_), self.pi_of_N)
        return expo_inv

    def findFactorsCaseOne(self):
        print("===========================================")
        print("Modulus N is: " + str(self.N))
        #A = (p+q)/2 == ceil(sqrt(N))
        A = gmpy2.ceil(gmpy2.sqrt(self.N))
        print("A is: " + str(A))
        #x = sqrt(A^2 - N)
        A_square = gmpy2.mul(A, A)
        x = gmpy2.sqrt(gmpy2.sub(A_square, self.N))
        print("x is: " + str(x))
        #p = A - x AND q = A + x
        self.p = gmpy2.sub(A, x)
        self.q = gmpy2.add(A, x)
        prod = gmpy2.mul(self.p, self.q)
        print("Product of pq is: " + str(prod))
        if prod == self.N:
            print("We have got the factors RIGHT")
        else:
            print("We didn't get the factors")
            print(self.p)
            print(self.q)
            self.p = 0
            self.q = 0
        print("===========================================")

    def findFactorsCaseTwo(self):
        print("===========================================")
        print("Modulus N is: " + str(self.N))
        #A = (p+q)/2 AND A can be any where from ceil(sqrt(N)) to ceil(sqrt(N)) + 2 ^ 20
        A = gmpy2.ceil(gmpy2.sqrt(self.N))
        print("A is: " + str(A))
        upperRange = 2 ** 20
        for index in range (0, upperRange + 1):
            #x = sqrt(A^2 - N)
            A_square = gmpy2.mul(A, A)
            diff = gmpy2.sub(A_square, self.N)
            x = mpz(gmpy2.sqrt(diff))
            #p = A - x AND q = A + x
            self.p = gmpy2.sub(A, x)
            self.q = gmpy2.add(A, x)
            prod = gmpy2.mul(self.p, self.q)
            if prod == self.N:
                print("We have got the factors RIGHT")
                return
            else:
                A = A + 1
        print("We didn't get the factors")
        self.p = 0
        self.q = 0

    def findFactorsCaseThree(self):
        print("===========================================")
        print("Modulus N is: " + str(self.N))
        #A = ceil(2 * sqrt(6N))
        A = gmpy2.ceil(gmpy2.mul(mpz(2), gmpy2.sqrt(gmpy2.mul(mpz(6), self.N))))
        print("A is: " + str(A))
        #X = sqrt(A^2 - 24N)
        A_square = gmpy2.mul(A, A)
        X = gmpy2.sqrt(gmpy2.sub(A_square, gmpy2.mul(mpz(24), self.N)))
        print("X is: " + str(X))
        #q = (A + X)/4 AND p = N/q
        self.q = gmpy2.f_div(mpz(gmpy2.add(A, X)), mpz(4))
        self.p = gmpy2.div(self.N, self.q)
        prod = gmpy2.mul(self.p, self.q)
        print("Product of pq is: " + str(prod))
        if prod == self.N:
            print("We have got the factors RIGHT")
        else:
            print("We didn't get the factors")
            print(self.p)
            print(self.q)
            self.p = 0
            self.q = 0
        print("===========================================")

    def getFactorP(self):
        return self.p

    def getFactorQ(self):
        return self.q

    def displayFactors(self):
        print("===========================================")
        print("Factors of Modulus N are:")
        print("p = " + str(self.p))
        print("q = " + str(self.q))
        print("===========================================")

    def decryptCipher(self, e_):
        cipher = mpz(e_)
        # y <- RSA(x) = x^e
        # x <- RSA_inv(y) = y^d
        RSA_inverse = gmpy2.powmod(mpz(cipher), self.d, self.N)
        #print("RSA Decrypted Message: " + str(RSA_inverse))
        # Convert it to HEX byte string now
        RSA_inverse_hex = "%X" % RSA_inverse
        # Look for PKCS1 v1.5 Separator Byte (0x00 or 0xFF). The message follows that
        # <0x02><Random Pad><0x00><Message>
        message = RSA_inverse_hex.split(self.separatorByte, 1)[1]
        #Print using UTF-8 as the message contains the ASCII values
        plaintext = bytes.fromhex(message).decode('utf-8')
        print("Plain Text Message: " + plaintext)

if __name__ == "__main__":

    # Instantiate RSAPublicEnc object
    rsa = RSAPublicEnc()
    rsa.setModulusN(sys.argv[1])
    #rsa.findFactorsCaseOne()
    #rsa.findFactorsCaseTwo()
    rsa.findFactorsCaseThree()
    rsa.displayFactors()
    #rsa.setModulusN(179769313486231590772930519078902473361797697894230657273430081157732675805505620686985379449212982959585501387537164015710139858647833778606925583497541085196591615128057575940752635007475935288710823649949940771895617054361149474865046711015101563940680527540071584560878577663743040086340742855278549092581)
    #rsa.setP(13407807929942597099574024998205846127479365820592393377723561443721764030073662768891111614362326998675040546094339320838419523375986027530441562135724301)
    #rsa.setQ(13407807929942597099574024998205846127479365820592393377723561443721764030073778560980348930557750569660049234002192590823085163940025485114449475265364281)
    #rsa.setEncyptionExponent(65537)
    #rsa.decryptCipher(sys.argv[1])
