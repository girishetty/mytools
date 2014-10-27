rm -f a.out
cc main.c aes_algo.c hdcp_2x.c hdcp_2x_api.c hmac_sha.c hdcp_receiver.c -lcrypto -o a.out
