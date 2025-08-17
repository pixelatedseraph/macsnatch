#ifndef MACSNATCH_H
#define MACSNATCH_H

#include <stdio.h>
#include <stdint.h>
#define LICENSE_LEN 16
#define HASH_LEN 64 /* SHA 512 produces 64 byte hash */
/* struct definition */
typedef struct License
{
    char *hwid; // hwid used
    char *key;  // license generated
    int expiry;
} License;

/* function which fetches hwid (MAC Address) from users device */
char *GetHWID(); /* returns hwid as string*/

/* function prototypes to generate hash & generate key */
unsigned char *GenerateHash(const char *);                  /* pass hwid and it generates a SHA-512 hash based on it */
unsigned char *SignHash(unsigned char *, size_t, size_t *); /* pass SHA-512 hash,hash length,signature len(to fetch two values from a function ) & it will sign the hash with RSA private key */

/* now lets write a function  which handles license validation */
int VerifyHash(unsigned char *,size_t,unsigned char *,size_t ); /* pass hash, hash length, signature , signature length and returns 1 if signature is valid */


#endif
