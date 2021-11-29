#if 0
#include "memload.h"

#include <stdio.h>
#include <string.h>

#define PROCMEMINFOFILE "/proc/meminfo"
#define String_startsWith(s, match) (strncmp((s),(match),strlen(match)) == 0)

bool memload()
{
    unsigned long long int swapFree = 0;
    unsigned long long int shmem = 0;
    unsigned long long int sreclaimable = 0;

    unsigned long long int totalMem = 0;
    unsigned long long int freeMem = 0;
    unsigned long long int sharedMem = 0;
    unsigned long long int buffersMem = 0;
    unsigned long long int cachedMem = 0;
    unsigned long long int totalSwap = 0;
    unsigned long long int usedMem = 0;
    unsigned long long int usedSwap = 0;

    FILE *file = fopen(PROCMEMINFOFILE, "r");
    if (!file)
        return false;

    char buffer[128];

    while (fgets(buffer, 128, file))
    {
        #define tryRead(label, variable) (String_startsWith(buffer, label) && sscanf(buffer + strlen(label), " %32llu kB", variable))

        switch (buffer[0])
        {
        case 'M':
            if (tryRead("MemTotal:", &totalMem)) {}
            else if (tryRead("MemFree:", &freeMem)) {}
            else if (tryRead("MemShared:", &sharedMem)) {}
            break;
        case 'B':
            if (tryRead("Buffers:", &buffersMem)) {}
            break;
        case 'C':
            if (tryRead("Cached:", &cachedMem)) {}
            break;
        case 'S':
            switch (buffer[1])
            {
            case 'w':
                if (tryRead("SwapTotal:", &totalSwap)) {}
                else if (tryRead("SwapFree:", &swapFree)) {}
                break;
            case 'h':
                if (tryRead("Shmem:", &shmem)) {}
                break;
            case 'R':
                if (tryRead("SReclaimable:", &sreclaimable)) {}
                break;
            }
        break;
        }
        #undef tryRead
    }

    usedMem = totalMem - freeMem;

    //cachedMem = cachedMem + sreclaimable - shmem;
    //usedSwap = totalSwap - swapFree;

    printf("mem load : %llu\n", usedMem);

    fclose(file);

    return true;
}


#endif


