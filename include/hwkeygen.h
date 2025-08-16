#ifndef HW_KEY_GEN_H
#define HW_KEY_GEN_H

#include <stdio.h>
#include<stdint.h>
#define LICENSE_LEN 16

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
uint64_t GenerateHash(const char *); /* pass hwid and it generates a hash based on it */
char *GenerateLicense(uint64_t);     /* pass hash and it returns License */

/* now lets write a function  which handles license validation */
void VerifyLicenseKey(const char *); /* now it verifies the user-typed license */

#endif