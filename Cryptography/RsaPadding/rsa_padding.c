#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char U8;
typedef unsigned int U32;
typedef int          STATUS;

void trace_hex_data(const char* what, const unsigned char* pbData, unsigned int len);

#define com_lib_misc_memory_cpy memcpy
#define secure_memcmp memcmp

#define STATUS_SUCCESS 0x00
#define STATUS_FAILURE 0xFF

typedef enum _tagComRsaPaddingMode {
   COM_RSA_PADDING_MODE_NO_PADDING               = 0x00000000,
   COM_RSA_PADDING_MODE_EME_OAEP_SHA1            = 0x00000001,
   COM_RSA_PADDING_MODE_EME_OAEP_SHA256          = 0x00000002,
   COM_RSA_PADDING_MODE_EME_OAEP_SHA384          = 0x00000003,
   COM_RSA_PADDING_MODE_EME_OAEP_SHA512          = 0x00000004,
   COM_RSA_PADDING_MODE_EME_PKCS15               = 0x00000005,
   COM_RSA_PADDING_MODE_EMSA_PKCS15_SHA1         = 0x00000006,
   COM_RSA_PADDING_MODE_EMSA_PKCS15_SHA256       = 0x00000007,
   COM_RSA_PADDING_MODE_EMSA_PKCS15_SHA384       = 0x00000008,
   COM_RSA_PADDING_MODE_EMSA_PKCS15_SHA512       = 0x00000009,
   COM_RSA_PADDING_MODE_EMSA_PSS_SHA1            = 0x0000000a,
   COM_RSA_PADDING_MODE_EMSA_PSS_SHA256          = 0x0000000b,
   COM_RSA_PADDING_MODE_EMSA_PSS_SHA384          = 0x0000000c,
   COM_RSA_PADDING_MODE_EMSA_PSS_SHA512          = 0x0000000d,
   COM_RSA_PADDING_MODE_LAST,
} COM_RSA_PADDING_MODE;

static STATUS com_lib_rsa_get_padding_hash_size(COM_RSA_PADDING_MODE mode, U32* size, U32* hash_mode)
{
    STATUS status = STATUS_SUCCESS;
    U32 local_size = 0;

    switch (mode) {
    case COM_RSA_PADDING_MODE_NO_PADDING:
        local_size = 0;
       break;

    case COM_RSA_PADDING_MODE_EME_OAEP_SHA1:
    case COM_RSA_PADDING_MODE_EMSA_PKCS15_SHA1:
    case COM_RSA_PADDING_MODE_EMSA_PSS_SHA1:
        local_size = 20;
       break;

    case COM_RSA_PADDING_MODE_EME_OAEP_SHA256:
    case COM_RSA_PADDING_MODE_EMSA_PKCS15_SHA256:
    case COM_RSA_PADDING_MODE_EMSA_PSS_SHA256:
        local_size = 32;
       break;

    case COM_RSA_PADDING_MODE_EME_OAEP_SHA384:
    case COM_RSA_PADDING_MODE_EMSA_PKCS15_SHA384:
    case COM_RSA_PADDING_MODE_EMSA_PSS_SHA384:
        local_size = 48;
       break;

    case COM_RSA_PADDING_MODE_EME_OAEP_SHA512:
    case COM_RSA_PADDING_MODE_EMSA_PKCS15_SHA512:
    case COM_RSA_PADDING_MODE_EMSA_PSS_SHA512:
        local_size = 64;
       break;

    default:
       *size = 0;
       status = STATUS_FAILURE;
       break;
    }

    if (size != NULL) {
       *size = local_size;
    }

    return status;
}

//Refer to RFC3447 Page 42, EMES-PKCS1_v1_5_Encoding
U8 T_SHA1[15]   = {0x30, 0x21, 0x30, 0x09, 0x06, 0x05, 0x2b, 0x0e, 0x03, 0x02, 0x1a, 0x05, 0x00, 0x04, 0x14};
U8 T_SHA256[19] = {0x30, 0x31, 0x30, 0x0d, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x01, 0x05, 0x00, 0x04, 0x20};
U8 T_SHA384[19] = {0x30, 0x41, 0x30, 0x0d, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x02, 0x05, 0x00, 0x04, 0x30};
U8 T_SHA512[19] = {0x30, 0x51, 0x30, 0x0d, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x03, 0x05, 0x00, 0x04, 0x40};

static STATUS com_lib_rsa_get_T(COM_RSA_PADDING_MODE mode, U8 **T_Hash)
{
    STATUS status = STATUS_SUCCESS;

    switch (mode) {
    case COM_RSA_PADDING_MODE_EMSA_PKCS15_SHA1:
       *T_Hash = T_SHA1;
       break;

    case COM_RSA_PADDING_MODE_EMSA_PKCS15_SHA256:
       *T_Hash = T_SHA256;
       break;

    case COM_RSA_PADDING_MODE_EMSA_PKCS15_SHA384:
       *T_Hash = T_SHA384;
       break;

    case COM_RSA_PADDING_MODE_EMSA_PKCS15_SHA512:
       *T_Hash = T_SHA512;
       break;

    default:
       *T_Hash = NULL;
       status = STATUS_FAILURE;
       break;
    }
    return status;
}

/******************************************************************************
**
** Function:   com_lib_rsa_emsa_pkcs15_encode
**
** Synopsis:   RSA EMSA PKCS15 Encoding interface
**
** Arguments:
**
**   pInput:        Pointer to message to be padded
**   uInSize:       Size of the input message in byte
**   pOutput:       Pointer to the padded EM message
**   uOutSize:      Size of the padded EM message in byte (should be RSA Modulus N size)
**   paddingScheme: Padding message. Support PKCS15-SHA1/SHA256/SHA384/SHA512
**
** Returns:    See the status.h and com_cry_common.h
**
** Refer to RFC3447 Page 41:
**
**  EM =  0x00 || 0x01 || PS || 0x00 || T
**
**  PS: Padding String 0xff and its size is based the size of output EM and Hash
**
**  T : DER_Encoding_DigestInfo || H
**       -->SHA1   : 0x30 21 30 09 06 05 2b 0e 03 02 1a 05 00 04 14 || H
**       -->SHA256 : 0x30 31 30 0d 06 09 60 86 48 01 65 03 04 02 01 05 00 04 20 || H
**       -->SHA384 : 0x30 41 30 0d 06 09 60 86 48 01 65 03 04 02 02 05 00 04 30 || H
**       -->SHA512 : 0x30 51 30 0d 06 09 60 86 48 01 65 03 04 02 03 05 00 04 40 || H
**
******************************************************************************/
#define EM_MIN_LEN          11
#define EM_HEADER_LEN       2
#define EM_OTHER_SIZE       (EM_HEADER_LEN + 1)
#define EM_MIN_PS_LEN       8
STATUS com_lib_rsa_emsa_pkcs15_encode_(U8* pInput, U32 uInSize, U8* pOutput, U32 uOutSize, COM_RSA_PADDING_MODE paddingScheme)
{
    STATUS status = STATUS_FAILURE;
    U32    emLen = uOutSize;
    U32    tLen = 0;
    U32    hLen = 0;
    U32    psLen = 0;
    U8*    pT = NULL;
    U32    i = 0;

    if (pInput && pOutput) {
        if (COM_RSA_PADDING_MODE_EME_PKCS15 == paddingScheme) {
            //EM = 0x00 || 0x01 || PS || 0x00 || M
            //Message is not a digest and we dont have to add DigestInfo in this case
            hLen = uInSize;
            tLen = 0;
            status = STATUS_SUCCESS;
        } else {
            //EM = 0x00 || 0x01 || PS || 0x00 || DER_Encoding_DigestInfo || M
            //Message is digest and we need to add DigestInfo
            tLen = 19;
            if (paddingScheme == COM_RSA_PADDING_MODE_EMSA_PKCS15_SHA1) {
                tLen = 15;
            }
            status = com_lib_rsa_get_T(paddingScheme, &pT);
            //Message has to be digest of given size
            status |= com_lib_rsa_get_padding_hash_size(paddingScheme, &hLen, NULL);
        }

        //Validate the input message/digest before proceeding
        if (status == STATUS_SUCCESS && hLen == uInSize &&
            emLen >= (EM_MIN_LEN + tLen + uInSize)) {
            //Two leading characters
            pOutput[0] = 0x00;
            pOutput[1] = 0x01;

            //Calculate PS size and generate octate string of hexadecimal value 0xff
            psLen = emLen - (EM_OTHER_SIZE + tLen + uInSize);
            for (i = EM_HEADER_LEN; i < (EM_HEADER_LEN + psLen); i++) {
                pOutput[i] = 0xFF;
            }

            //0x00 seperate character
            //i = EM_HEADER_LEN + psLen
            pOutput[i++] = 0x00;

            //Add DigestInfo now
            if (pT) {
                //i = EM_HEADER_LEN + psLen + 1
                com_lib_misc_memory_cpy(pOutput + i,  pT, tLen);
                i += tLen;
            }

            //Add the message/digest now
            com_lib_misc_memory_cpy(pOutput + i, pInput, uInSize);
        } else {
            status = STATUS_FAILURE;
        }
    }

    return status;
}

/******************************************************************************
**
** Function:   com_lib_rsa_emsa_pkcs15_decode
**
** Synopsis:   RSA EMSA PKCS15 Decoding interface
**
** Arguments:
**
**   pInput:        Padded EM message
**   uInSize:       Size of the input message in byte (should be RSA Modulus N size)
**   pOutput:       Pointer to the message
**   pOutSize:      Size of the message in byte
**   paddingScheme: Padding message. Support PKCS15-SHA1/SHA256/SHA384/SHA512
**
** Returns:    See the status.h and com_cry_common.h
**
** Refer to RFC3447 Page 41:
**
**  EM =  0x00 || 0x01 || PS || 0x00 || T
**
**  PS: Padding String 0xff and its size is based the size of output EM and Hash
**
**  T : DER_Encoding_DigestInfo || H
**       -->SHA1   : 0x30 21 30 09 06 05 2b 0e 03 02 1a 05 00 04 14 || H
**       -->SHA256 : 0x30 31 30 0d 06 09 60 86 48 01 65 03 04 02 01 05 00 04 20 || H
**       -->SHA384 : 0x30 41 30 0d 06 09 60 86 48 01 65 03 04 02 02 05 00 04 30 || H
**       -->SHA512 : 0x30 51 30 0d 06 09 60 86 48 01 65 03 04 02 03 05 00 04 40 || H
**
******************************************************************************/
STATUS com_lib_rsa_emsa_pkcs15_decode_(U8* pInput, U32 uInSize, U8* pOutput, U32* pOutSize, COM_RSA_PADDING_MODE paddingScheme)
{
    STATUS status = STATUS_FAILURE;
    U8* pM        = NULL;
    U32 i         = 0;
    U32 psLen   = 0;
    U32 hLen    = 0;
    U32 tLen    = 0;
    U8 *pT      = NULL;

    if (pInput && pOutput && pOutSize) {
        //The input encoded message (EM) should be in this format
        //EM = 0x00 || 0x01 || PS || 0x00 || M
        //   OR
        //EM = 0x00 || 0x01 || PS || 0x00 || DER_Encoding_DigestInfo || M
        if (pInput[0] == 0x00 && pInput[1] == 0x01) {
            //Look for PS || 0x00, PS should be 0xFF
            for (i = EM_HEADER_LEN; i < uInSize; i++) {
                if (pInput[i] != 0xFF) {
                    break;
                }
            }
            psLen = i - EM_HEADER_LEN;
            //Make sure we have found PS || 0x00 and the length of PS >= 8
            if (i < uInSize && pInput[i] == 0x00 && psLen >= EM_MIN_PS_LEN) {
                status = STATUS_SUCCESS;
            }
            //Increment the index to point to M
            i++;
            if (COM_RSA_PADDING_MODE_EME_PKCS15 == paddingScheme) {
                //No DigestInfo, rest everything in message
                tLen = 0;
                hLen = uInSize - i;
            } else {
                //There is DigestInfo, followed by digest/message
                tLen = 19;
                if (paddingScheme == COM_RSA_PADDING_MODE_EMSA_PKCS15_SHA1) {
                    tLen = 15;
                }
                status = com_lib_rsa_get_T(paddingScheme, &pT);
                //Message has to be digest of given size
                status |= com_lib_rsa_get_padding_hash_size(paddingScheme, &hLen, NULL);
            }
            //Check for DigestInfo now
            if (pT) {
                //i = EM_HEADER_LEN + psLen + 1
                if (0 != secure_memcmp(pInput + i,  pT, tLen)) {
                    status = STATUS_FAILURE;
                }
                i += tLen;
            }
            //Irrespective of error case - do the following to disable timing attack or failure specific error code
            //Now we are at message/digest part. Verify the size before proceeding
            *pOutSize = uInSize - i;
            if (*pOutSize != hLen) {
                status = STATUS_FAILURE;
            }
            *pOutSize = uInSize - i;
            //copy the message (M) to output buffer
            pM = pInput + i;
            for (i = 0; i < *pOutSize; i++) {
               pOutput[i] = pM[i];
            }
        }
    }

    return status;
}

#if 0
/*
 * Internal Function: RSA EMSA PKCS1.5 encoding
 *
 */
STATUS com_lib_rsa_emsa_pkcs15_encode_(U8* pInput, U32 uInSize, U8* pOutput, U32 uKeySize)
{
    STATUS status = STATUS_FAILURE;
    U32 uPS       = 0;
    U32 i         = 0;
    U32 j         = 0;

    if (pInput && pOutput && uInSize <= (uKeySize - EM_MIN_LEN)) {
        //The input message (M) should be encoded into: EM = 0x00 || 0x01 || PS || 0x00 || M
        pOutput[0] = 0x00;
        pOutput[1] = 0x01;
        //Calculate PS size and generate non-zero pseudo-random PS string
        uPS = uKeySize - (uInSize + EM_OTHER_SIZE);
        generate_random(&pOutput[2], uPS);
        for (i = 2; i < uPS + 2; i++) {
            if (pOutput[i] == 0) {
                pOutput[i] = 0xFF;
            }
        }

        pOutput[2 + uPS] = 0x00;
        j = EM_OTHER_SIZE + uPS;
        //copy the message (M) to end of output buffer
        for (i = 0; i < uInSize; i++, j++) {
            pOutput[j] = pInput[i];
        }
        status = STATUS_SUCCESS;
    }

    return status;
}

/*
 * Internal Function: RSA EMSA PKCS1.5 decoding
 *
 */
STATUS com_lib_rsa_emsa_pkcs15_decode_(U8* pInput, U32 uInSize, U8* pOutput, U32* pOutSize)
{
    STATUS status = STATUS_FAILURE;
    U8* pM        = NULL;
    U32 i         = 0;

    if (pInput && pOutput && pOutSize) {
        //The input encoded message (EM) should be in this format
        //EM = 0x00 || 0x01 || PS || 0x00 || M
        if (pInput[0] == 0x00 && pInput[1] == 0x01) {
            //Look for PS || 0x00
            for (i = EM_HEADER_LEN; i < uInSize; i++) {
                if (pInput[i] == 0x00) {
                    break;
                }
            }

            //Make sure we have found PS || 0x00 and the length of PS >= 8
            if (i < uInSize && i >= (EM_HEADER_LEN + EM_MIN_PS_LEN)) {
                status = STATUS_SUCCESS;
            }
            //Irrespective of error case - do the following to disable timing attack or failure specific error code
            //copy the message (M) to output buffer, skip the 0x00 || 0x01 || PS || 0x00
            *pOutSize = uInSize - (i + 1);
            pM = pInput + (i + 1);
            for (i = 0; i < *pOutSize; i++) {
               pOutput[i] = pM[i];
            }
        }
    }

    return status;
}

/******************************************************************************
** Function:   com_lib_rsa_emsa_pss_encode_
** Synopsis:   RSA EMSA PSS Encoding Interface
**
** Arguments:
**   [mHash]:         Pointer to hashed message to be padded
**   [mLen]:          size of the input message in byte
**   [EM]:            Pointer to the padded EM message
**   [emBits]:        size of the padded EM message in bits
**   [paddingScheme]: RSA padding mode
**   [com_hash_mode]: RSA HASH mode
**
** Returns:    Ses the status.h and com_cry_common.h
**
** Notes: Refer to RFC3447 Page 38 EMSA-PSS-ENCODE
**
** EM = maskedDB || H || 0xbc
**
**   --> H = Hash(M')
**         -->M' = 0x00 0x00 0x00 0x00  0x00 0x00 0x00 0x00 || input || salt
**   --> maskedDB = DB XOR dbMask
**         -->DB = PS || 0x01 || salt
**         -->dbMask = MGF(H, emLen - hLen -1)
**
**  emLen: length of the EM
**  hLen : length of H, which depends on the Hash Mode applied
**  db_size  = emLen - hLen -1
**  max sLen = emLen - hLen -2  (We apply 20 as salt length)
******************************************************************************/
STATUS com_lib_rsa_emsa_pss_encode_(U8* mHash, U32 mLen, U8* EM, U32 emBits,
                                    COM_RSA_PADDING_MODE paddingScheme, COM_HASH_MODE com_hash_mode)
{
    STATUS status = STATUS_SUCCESS;
    U32 idx      = 0;
    U32 hLen     = 0;
    U32 leftZero = 0;
    U32 maskLen  = 0;
    U32 msgLen   = 0;
    U32 digest_size = 0;
    U32 sLen     = PSS_SALT_SIZE_IN_BYTE;
    U32 emLen    = (emBits + 7) / 8;
    U8  MSG[8 + SZ_DIGEST_SHA512 + PSS_SALT_SIZE_IN_BYTE];
    U8  dbMask[MAX_DB_SIZE_IN_BYTE];
    U8  maskedDB[MAX_DB_SIZE_IN_BYTE];
    U8  leadingMask = 0xFF;
    U8  salt[PSS_SALT_SIZE_IN_BYTE];
    COM_DIGEST H;

    if (NULL == mHash || NULL == EM) {
        return STATUS_UNSUPPORTED_PARAMETER;
    }

    //input is assumed to be already a hash/digest
    status = com_lib_rsa_get_padding_hash_size(paddingScheme, &hLen, &com_hash_mode);
    if (hLen != mLen || emLen < (hLen + sLen + 2)) {
        return STATUS_FAILURE;
    }
    maskLen = emLen - hLen - 1;

    //Generate random salt of length sLen
    status = com_lib_drbg_random_generator(salt, sLen);
    COM_CHECK_STATUS(status);

    //Compose the M' Message = 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 || input || salt
    com_lib_misc_memory_set(MSG, 0, 8);
    com_lib_misc_memory_cpy(MSG + 8, mHash, hLen);
    com_lib_misc_memory_cpy(MSG + 8 + hLen, salt, sLen);
    msgLen = 8 + hLen + sLen;

    //Compute H = Hash(M')
    status = com_lib_hash(com_hash_mode, MSG, msgLen, &H, &digest_size);
    COM_CHECK_STATUS(status);

    //Compute dbMask = MGF(H, emLen - hLen -1)
    status = com_lib_rsa_mgf1((U8*)&H, hLen, dbMask, maskLen, com_hash_mode);
    COM_CHECK_STATUS(status);

    //Generate PS (of all 0s) of lenght (emLen - sLen - hLen - 2) = (maskLen - sLen - 1)
    //Generate DB as PS || 0x01 || salt of length emLen - hLen - 1
    //maskedDB = DB xor dbMask
    com_lib_misc_memory_cpy(maskedDB, dbMask, maskLen - sLen - 1);  // PS ^ dbMash[0..psLen]
    maskedDB[maskLen - sLen - 1] = 0x01 ^ dbMask[maskLen - sLen - 1];
    //salt ^ dbMask[psLen+1..maskLen)
    for (idx = 0; idx < sLen ; idx++) {
        maskedDB[maskLen - sLen + idx] = salt[idx] ^ dbMask[maskLen - sLen + idx];
    }

    //Set the leftmost 8emLen - emBits bits of the leftmost octet in maskedDB to zero.
    leftZero = emLen * 8 - emBits;
    for (idx = 0; idx < leftZero; idx++) {
        leadingMask = leadingMask >> 1;
    }
    maskedDB[0] = maskedDB[0] & (leadingMask & 0xFF);

    //Compose the EM message as maskedDB || H || 0xbc
    com_lib_misc_memory_cpy(EM, maskedDB, maskLen);
    com_lib_misc_memory_cpy(EM + maskLen, (U8*)&H, hLen);
    EM[emLen - 1] = 0xbc;

    return status;
}

/******************************************************************************
** Function:  com_lib_rsa_emsa_pss_verify_
** Synopsis:   RSA EMSA PSS Decode interface
**
** Arguments:
**   [mHash]:         Pointer to hashed message to be padded
**   [mLen]:          size of the input message in byte
**   [EM]:            Pointer to the padded EM message
**   [emBits]:        size of the padded EM message in bits
**   [paddingScheme]: RSA padding mode
**   [com_hash_mode]: RSA HASH mode
**
** Returns:    See the status.h and com_cry_common.h
******************************************************************************/
STATUS com_lib_rsa_emsa_pss_verify_(U8* mHash, U32 mLen, U8* EM, U32 emBits,
                                    COM_RSA_PADDING_MODE paddingScheme, COM_HASH_MODE com_hash_mode)
{
    STATUS status = STATUS_SUCCESS;
    U32 emLen = (emBits + 7)/8;
    U32 hLen = 0;
    U32 sLen = PSS_SALT_SIZE_IN_BYTE;
    U8  salt[PSS_SALT_SIZE_IN_BYTE];
    U8  MSG[PSS_MESSAGE_SIZE_IN_BYTE];
    U32 digest_size = 0;
    U32 msg_size    = 0;
    U8  DB[MAX_DB_SIZE_IN_BYTE];
    U8  DB_mask[MAX_DB_SIZE_IN_BYTE];
    U32 ps_size = 0;
    U32 db_mask_size = 0;
    U32 i = 0;
    COM_DIGEST digest;

    status = com_lib_rsa_get_padding_hash_size(paddingScheme, &hLen, &com_hash_mode);
    COM_CHECK_STATUS(status);

    //Validate the input length and the last byte
    if (emLen < (hLen + sLen + 2)) {
        return STATUS_FAILURE;
    }
    if (EM[emLen-1] != 0xbc) {
        return STATUS_FAILURE;
    }

    db_mask_size = emLen - hLen - 1;

    //dbMask = MGF(H, emLen-hLen-1)
    status = com_lib_rsa_mgf1(EM+db_mask_size, hLen, DB_mask, db_mask_size, com_hash_mode);
    COM_CHECK_STATUS(status);

    //DB = maksedDB xor dbMasks
    com_lib_misc_math_xor(DB_mask, EM, DB, db_mask_size);

    //Check the PS part
    ps_size = emLen - hLen - sLen - 2;
    for (i = 0; i < ps_size; i++) {
        if (DB[i] != 0x00) {
            return STATUS_FAILURE;
        }
    }

    //Check the 0x01 flag part.
    if (DB[ps_size] != 0x01) {
        return STATUS_FAILURE;
    }

    //Compose the M' Message = 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 || input || salt
    com_lib_misc_memory_set(MSG, 0, PSS_MESSAGE_SIZE_IN_BYTE);
    com_lib_misc_memory_cpy(MSG + 8, mHash, mLen);
    com_lib_misc_memory_cpy(MSG + 8 + mLen, DB + (db_mask_size - sLen), sLen);
    msg_size = 8 + sLen + mLen;

    status = com_lib_hash(com_hash_mode, MSG, msg_size, &digest, &digest_size);
    COM_CHECK_STATUS(status);

    //Compare if  H = H'
    status = com_lib_misc_memory_cmp((U8*)&digest, EM+db_mask_size, digest_size);
    return status;
}
#endif

const U8 test_input[] = {
  0x30, 0x82, 0x02, 0x77, 0x02, 0x01, 0x00, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7,
  0x0d, 0x01, 0x01, 0x01, 0x05, 0x00, 0x04, 0x82, 0x02, 0x61, 0x30, 0x82, 0x02, 0x5d, 0x02, 0x01,
  0x00, 0x02, 0x81, 0x81, 0x00, 0xdb, 0x2a, 0xea, 0xc5, 0x96, 0x47, 0x49, 0xac, 0xc0, 0x1f, 0x66,
  0x62, 0xda, 0xe5, 0x01, 0x85, 0xca, 0x4a, 0xf0, 0x9c, 0x2b, 0x5f, 0x1f, 0x9e, 0x9c, 0xc4, 0x34,
  0xd1, 0x5f, 0x58, 0xab, 0x82, 0x08, 0x57, 0xde, 0xcb, 0x4b, 0x68, 0x59, 0xfc, 0x57, 0x85, 0xa2,
  0x5d, 0x1e, 0xdb, 0x91, 0xa2, 0x4b, 0x1e, 0x7f, 0xc3, 0xcd, 0x45, 0x4a, 0xbe, 0xef, 0x41, 0xdd,
  0x77, 0x89, 0xd3, 0x2a, 0x0a, 0x0f, 0x44, 0x95, 0x13, 0xad, 0x8e, 0x2b, 0x34, 0x21, 0x6f, 0xf6,
  0x46, 0x39, 0x8a, 0xf8, 0xd0, 0xeb, 0xbb, 0xff, 0x55, 0xf7, 0xef, 0x2c, 0x8a, 0xe9, 0xee, 0xe3,
  0xda, 0x6a, 0x9b, 0x23, 0xc0, 0xa9, 0xa3, 0x1d, 0x42, 0x39, 0x54, 0x26, 0xc7, 0x08, 0x5e, 0x6a,
  0x06, 0x1e, 0xf0, 0x65, 0x29, 0x13, 0xc3, 0x85, 0xdc, 0x30, 0x91, 0xdd, 0x0a, 0x29, 0xe2, 0xce,
  0x36, 0x6d, 0xed, 0x9e, 0x7d, 0x02, 0x03, 0x01, 0x00, 0x01, 0x02, 0x81, 0x80, 0x54, 0x5f, 0xab,
  0x4e, 0x31, 0xd2, 0x73, 0xec, 0x8b, 0x72, 0x0b, 0x2a, 0x97, 0x76, 0x7e, 0x25, 0x91, 0x81, 0x54,
  0x2b, 0xeb, 0x0e, 0x8d, 0xcf, 0xb4, 0x2e, 0x5c, 0xdf, 0x15, 0xc3, 0xbf, 0x13, 0x2a, 0x3f, 0x19,
  0xc8, 0x4d, 0xd9, 0x78, 0x7b, 0x0a, 0xfe, 0xa9, 0x49, 0x0c, 0x2e, 0x1f, 0x5c, 0xc9, 0x6a, 0x8e,
  0x69, 0x44, 0xc7, 0x1e, 0x45, 0x6f, 0x3a, 0x7e, 0x16, 0x1c, 0xfb, 0x2c, 0x3d, 0x48, 0xc9, 0x63,
  0x98, 0xef, 0xa1, 0x5e, 0x10, 0x65, 0x62, 0x76, 0x6b, 0x30, 0x1d, 0x66, 0x21, 0x19, 0xa4, 0xdb,
  0xfe, 0xfc, 0x07, 0x4b, 0x95, 0x22, 0x3b, 0x33, 0x2b, 0x62, 0xb7, 0xda, 0xb1, 0xca, 0xe8, 0x92,
  0xd2, 0xed, 0x93, 0x76, 0xa4, 0x40, 0x95, 0xd4, 0x16, 0x34, 0x94, 0xd7, 0x94, 0x8f, 0xca, 0xba,
  0x3f, 0x7d, 0x4e, 0x06, 0x5f, 0xbf, 0xfa, 0x58, 0x82, 0x4e, 0xda, 0x1d, 0xc9, 0x02, 0x41, 0x00,
  0xf0, 0x58, 0x0f, 0xa0, 0xe0, 0x7c, 0x0d, 0x2c, 0x5f, 0x4e, 0x3f, 0xcb, 0x85, 0xdf, 0x56, 0xaf,
};

void trace_hex_data(const char* what, const unsigned char* pbData, unsigned int len)
{
    unsigned int i = 0;
    printf("%s(%u) is:\n", what, len);

    for (i = 0; i < len; i++) {
        if ((i != 0) && (i % 16 == 0)) {
            printf("\n");
        }
        printf("0x%02x, ", pbData[i]);
    }
    printf("\n");
}

void verify_pkcs15(COM_RSA_PADDING_MODE paddingMode)
{
    STATUS status = STATUS_SUCCESS;
    U32 msgSize = 0;
    U32 msgSize_ = 0;
    U8 MSG[254] = {0};
    U8 MSG_[254] = {0};
    U8 EM[254] = {0};

    if (paddingMode != COM_RSA_PADDING_MODE_EME_PKCS15) {
        com_lib_rsa_get_padding_hash_size(paddingMode, &msgSize, NULL);
    } else {
        msgSize = 32;
    }
    memcpy(MSG, test_input, msgSize);
    status = com_lib_rsa_emsa_pkcs15_encode_(MSG, msgSize, EM, 128, paddingMode);
    if (status != STATUS_SUCCESS) {
        printf("verify_pkcs15: com_lib_rsa_emsa_pkcs15_encode_: Failed for %d\n", paddingMode);
    } else {
        status = com_lib_rsa_emsa_pkcs15_decode_(EM, 128, MSG_, &msgSize_, paddingMode);
        if (status != STATUS_SUCCESS) {
            printf("verify_pkcs15: com_lib_rsa_emsa_pkcs15_decode_: Failed for %d\n", paddingMode);
        } else if (msgSize != msgSize_ || memcmp(MSG, MSG_, msgSize) != 0) {
            printf("verify_pkcs15: Failed for %d\n", paddingMode);
            trace_hex_data("MSG", MSG, msgSize);
            trace_hex_data("EM", EM, 128);
            trace_hex_data("MSG", MSG_, msgSize_);
        } else {
            printf("verify_pkcs15: Success for %d\n", paddingMode);
        }
    }
}

int main()
{
    verify_pkcs15(COM_RSA_PADDING_MODE_EME_PKCS15);
    verify_pkcs15(COM_RSA_PADDING_MODE_EMSA_PKCS15_SHA1);
    verify_pkcs15(COM_RSA_PADDING_MODE_EMSA_PKCS15_SHA256);
    verify_pkcs15(COM_RSA_PADDING_MODE_EMSA_PKCS15_SHA384);
    verify_pkcs15(COM_RSA_PADDING_MODE_EMSA_PKCS15_SHA512);
    return 0;
}
