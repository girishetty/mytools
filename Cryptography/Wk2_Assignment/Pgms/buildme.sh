rm a.out
cc main.c aes_cbc.c aes_ctr.c -lcrypto -o a.out

