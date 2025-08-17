#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include "macsnatch.h"
/* function which signs the SHA 512 hash generated */
unsigned char *SignHash(unsigned char *hash, size_t hash_len, size_t *sig_len)
{

    /* the reason why i defined size_t *sig_len is because the RSA_sign function needs to write the signature length
     and in c , a function can have only one return value so we mimic a function returning two values via pointers */

    /* OpenSSl's RSA_Sign function signature looks like below */
    /* int RSA_sign(int type,
             const unsigned char *m, unsigned int m_len,
             unsigned char *sigret, unsigned int *siglen, this function needs to write the length of signature
             RSA *rsa);
*/

    /* RSA_Sign() needs to write back the length of signature ,so we pass a pointer unsigned int* siglen */

    /* open file which holds the private key*/
    FILE *f = fopen("private.pem", "r"); /* private rsa key */
    if (!f)
    {
        perror("private.pem");
        return NULL;
    }

    RSA *private_key = PEM_read_RSAPrivateKey(f, NULL, NULL, NULL); /* reads the private key from PEM file and stores it RSA structure */
    fclose(f);
    if (!private_key)
    {                                /* if reading failed */
        ERR_print_errors_fp(stderr); /* prints OpenSSL errors and returns NULL */
        return NULL;
    }

    /* lets allocate memory for signature on heap */
    unsigned char *signature = malloc(RSA_size(private_key));
    if (!signature)
        return NULL;

    /* function which actually signs hash */
    if (RSA_sign(NID_sha512, hash, hash_len, signature, (unsigned int *)sig_len, private_key) != 1) /* RSA_sign () returns 1 if signature embedding is successful */
    {
        ERR_print_errors_fp(stderr); /* OPenSSL errors */
        free(signature);
        RSA_free(private_key);
        return NULL;
    }
    RSA_free(private_key);
    return signature;
}