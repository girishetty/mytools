import urllib2
import sys

HOST = 'http://crypto-class.appspot.com/po?er='

# IV == CIPHER[0]
# Actual cipher message =  CIPHER[i] for i = 1, len - 1
CIPHER = [ 'f20bdba6ff29eed7b046d1df9fb70000', '58b1ffb4210a580f748b4ac714c001bd',
           '4a61044426fb515dad3f21f18aa577c0', 'bdf302936266926ff37dbf7035d5eeb4']

#--------------------------------------------------------------
# padding oracle Implementation
#--------------------------------------------------------------
class PaddingOracle(object):

    # Send the HTTP query to the web HOST and wait for the response
    def query(self, iv, cipher):
        # construst query URL
        target = HOST + urllib2.quote(iv + cipher)
        # Send HTTP request to server
        req = urllib2.Request(target)

        # Wait for response
        try:
            f = urllib2.urlopen(req)
        except urllib2.HTTPError, e:          
            # Check the response code
            if e.code == 404:
                #print "Valid Padding, but Invalid MAC"
                return True
            else:
                #print "Invalid Padding"
                return False
        else:
            print "Valid Cipher ==> Valid padding, MAC"
            return True

    #Converts ASCCI value based string to ASCII string
    def convertAsciiString(self, string):
        #Split every 2 digits (2 digits corresponds to one ASCII character)
        n = 2
        listString = "".join([chr(int(string[i:i+n], 16)) for i in range(0, len(string), n)])
        return listString

    #XOR 2 strings, which can be of variable length too
    def xorBytes(self, left, right):
        xorString = ""
        a = left
        b = right
        diff = len(left) - len(right)

        if diff > 0:
            xorString = left[:diff]
            a = left[diff:]
        elif diff < 0:
            diff = -diff
            xorString = right[:diff]
            b = right[diff:];

        xorred = "".join((["{:x}".format(int(x, 16) ^ int(y, 16)) for (x, y) in zip(a, b)]))
        return xorString + xorred

    #Constructs the padding bytes
    def getPadding(self, padCount):
        padBytes = ""
        for count in range (0, padCount):
            padBytes = padBytes + "".join("{:02x}".format(padCount))
        return padBytes

    #Breaks cipher message using one byte oracle algorithm
    def oneByteOracle(self, iv, cipher, byte, previousByte):
        if byte <= 16 and byte > 0:
            padding = self.getPadding(byte)
            #print "Padding Byte is: " + padding
            for count in range (0, 256):
                if count == byte and byte == 1:
                    #For the last byte (LSB), we dont have to do this as guess (0x01) XOR pad (0x01) yields 0
                    continue
                guess = "".join("{:02x}".format(count)) + previousByte;
                new_guess = self.xorBytes(guess, padding)
                #print "new_guess: " + new_guess
                new_iv = self.xorBytes(iv, new_guess)

                # Issue HTTP Query now
                good_padding = self.query(new_iv, cipher)
                if good_padding:
                    #print "Our guess for " + guess + " is right"
                    return guess
        print "Got nothing!!"
        return ""

    #Decrytps a block (16 byte) of cipher message using one byte oracle algorithm
    def blockDecryptionOracle(self, iv, cipher):
        #Do this for every CBC Cipher block (128 bits => 16 bytes)
        blockVal = ""
        for byteCount in range (1, 17):
            byteVal = self.oneByteOracle(iv, cipher, byteCount, blockVal)
            if byteVal:
                # update the block
                blockVal = byteVal
                print "Decrypted bytes: " + blockVal
            
        asciiBlock = self.convertAsciiString(blockVal)
        print "Decrypted block in ASCII: " + asciiBlock
        return asciiBlock

    #Decrytps a cipher message into plain text message using padding oracle algorithm
    def decryptCipherMessage(self, cipherMessage):
        cipherBlocks = len(cipherMessage)
        plainText = ""
        for blockCount in range (0, cipherBlocks - 1):
            # break one block at a time
            blockPlainText = self.blockDecryptionOracle(cipherMessage[blockCount], cipherMessage[blockCount + 1])
            plainText = plainText + blockPlainText

        return plainText


#sys.argv[1] ==> Command line arguments
if __name__ == "__main__":

    # Instantiate Padding Oracle object
    po = PaddingOracle()
    message = po.decryptCipherMessage(CIPHER)
    print "Decrypted Message is : " + message 

