#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/rsa.h>
#include "macsnatch.h"

/* function which verifies the signature */
int VerifyHash(unsigned char *hash, size_t hash_len, unsigned char *signature, size_t sig_len)
{

    /* open file which holds public key */
    FILE *f = fopen("public.pem", "r"); /* public rsa key */
    if (!f)
    {
        perror("public.pem");
        return 0;
    }

    /* define an RSA struct to hold public key */
    RSA *public_key = PEM_read_RSA_PUBKEY(f, NULL, NULL, NULL);
    fclose(f);
    if (!public_key)
    { /* fall back */
        ERR_print_errors_fp(stderr);
        return 0;
    }

    /* function which checks if the signature is valid */
    int valid = RSA_verify(NID_sha512, hash, hash_len, signature, sig_len, public_key);
    RSA_free(public_key);
    return valid;
}