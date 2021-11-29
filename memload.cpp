#include "memload.h"

#include <stdio.h>

bool memload()
{
    FILE *file = fopen("/proc/meminfo", "r");
    if (!file)
        return false;

    long int mem_total = 0;
    long int mem_free  = 0;
    long int mem_buffers = 0;
    long int mem_cached = 0;
    long int mem_sreclaimable = 0;
    unsigned int readmask = 0x10 | 0x8 | 0x4 | 0x2 | 0x1;

    char buf[80];
    while (readmask && fgets(buf, sizeof(buf), file))
    {
        if (sscanf(buf, "MemTotal: %ld kB\n", &mem_total) == 1)
        {
            readmask ^= 0x1;
            continue;
        }
        if (sscanf(buf, "MemFree: %ld kB\n", &mem_free) == 1)
        {
            readmask ^= 0x2;
            continue;
        }
        if (sscanf(buf, "Buffers: %ld kB\n", &mem_buffers) == 1)
        {
            readmask ^= 0x4;
            continue;
        }
        if (sscanf(buf, "Cached: %ld kB\n", &mem_cached) == 1)
        {
            readmask ^= 0x8;
            continue;
        }
        if (sscanf(buf, "SReclaimable: %ld kB\n", &mem_sreclaimable) == 1)
        {
            readmask ^= 0x10;
            continue;
        }
    }

    fclose(file);

    if (readmask)
        return false;

    //printf("total = %ld\n", mem_total);
    //printf("buffers = %ld\n", mem_buffers);
    //printf("free = %ld\n", mem_free);
    //printf("cached = %ld\n", mem_cached);
    //printf("sreclaimable = %ld\n", mem_sreclaimable);

    long int mem_used = mem_total - mem_free - mem_cached - mem_sreclaimable;

    /*- mem_buffers*/

    printf("used = %ld\n", mem_used);

    //double percent = mem_used / (double) mem_total;

    return true;
}


