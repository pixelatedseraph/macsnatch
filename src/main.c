#include <stdio.h>
#include <stdlib.h>
#include "macsnatch.h"

int main()
{
    /* generate hardware id */
    char *hwid = GetHWID();
    if (!hwid)
    {
        fprintf(stderr, "failed to generate hardware id\n ");
        return 1;
    }

    /* generate hash based on hwid */
    unsigned char *hash = GenerateHash(hwid);
    if (!hash)
    {
        fprintf(stderr, "failed to generate hash\n ");
        return 1;
    }

    /* sign hash to create a license */
    size_t sig_len;
    unsigned char *License = SignHash(hash, HASH_LEN, &sig_len);
    if (!License)
    {
        fprintf(stderr, "failed to generate signature\n");
        return 1;
    }
    printf("License Generated, length = %zu bytes \n", sig_len);

    /* verify license  */
    int Verify = VerifyHash(hash, HASH_LEN, License, sig_len);
    if (Verify == 1)
    {
        printf("License Verfied Succesfully! \n");
    }
    else
    {
        printf("Invalid license\n");
    }
    free(License);
    free(hash);
    free(hwid);
    return 0;
}
