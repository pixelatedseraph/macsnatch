#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <openssl/sha.h>
#include "macsnatch.h"
/* function to generate hash */
unsigned char *GenerateHash(const char *hwid)
{
    unsigned char *hash = malloc(SHA512_DIGEST_LENGTH); /* 64 bytes allocated and this array will hold final hash , assume caller calls free*/
    if (!hash)
        return NULL; 
    SHA512((unsigned char *)hwid, strlen(hwid), hash); /* input str, len of str , output array */

    return hash;
}
