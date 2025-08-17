#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/if.h>
#include <sys/types.h>
#include <linux/sockios.h>
#include <dirent.h>
#include "macsnatch.h"

char *GetHWID()
{
    /*allocate heap to store hwid ,assume caller calls free */

    char *hwid = malloc(64); /* allocate 64c bytes for hwid */
    if (!hwid)
        return NULL; /* if malloc failed */

    /* open the directory where all nics stored on a linux system */
    DIR *d = opendir("/sys/class/net");
    if (!d)
    {
        strcpy(hwid, "00:00:00:00:00:00"); /* fallback */
        return hwid;
    }

    struct dirent *entry;

    /* create a dummy socket */
    int fd = socket(AF_INET, SOCK_DGRAM, 0); /* create a dummy socket */
    if (fd < 0)
    {
        strcpy(hwid, "00:00:00:00:00:00"); /* fallback */
        closedir(d);
        return hwid;
    }
    /* read entries from directory */
    while ((entry = readdir(d)) != NULL)
    {
        if ((strcmp(entry->d_name, "lo") == 0) || (strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0))
            continue; /* skip lo because it is loopback and you can safely skip . and ..*/

        /* set up ifreq struct */

        struct ifreq ifr;                                   /* ifr is a struct used by ioctl to query network info */
        memset(&ifr, 0, sizeof(ifr));                       /* to clear up struct for no garbage */
        strncpy(ifr.ifr_name, entry->d_name, IFNAMSIZ - 1); /* .ifr_name is struct field to set NIC name ex "eth0", "enp3s0" */

        /* query mac addresss with ioctl */
        if (ioctl(fd, SIOCGIFHWADDR, &ifr) == 0) /* asks kernel hardware address of current NIC & if the call succeeds it returns 0 else -1*/
        {
            unsigned char *mac = (unsigned char *)ifr.ifr_hwaddr.sa_data; /* ifr.irf_hwaddr is a struct sockaddr within ifreq struct & sa_data give you raw address of hardware */
            sprintf(hwid, "%02X:%02X:%02X:%02X:%02X:%02X",
                    mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]); /* we cast sa_data to  unsigned char so we can treat it as array of bytes */
            break;                                                   /* once you successfully find a valid NIC with valid Mac , end loop */
        }
    }
    close(fd);
    closedir(d);
    return hwid;
}
