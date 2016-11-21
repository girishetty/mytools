rm -f a.out
cc main.c  rsa.c  rsa_pss.c  rsa_pss_verification.c  rsa_verification.c -lcrypto

