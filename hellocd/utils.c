#include "utils.h"

#include <stdlib.h>
#include <stddef.h>
#include <libcd.h>
#include <stdio.h>

#define CD_SECTOR_SIZE 2048

char* FileRead(const char* filename, u_long* size) {
    CdlFILE file;
    int num_sectors;
    char* buffer = NULL;

    *size = 0;
    if(CdSearchFile(&file, (char*)filename)) {
        num_sectors = (file.size + (CD_SECTOR_SIZE - 1)) / CD_SECTOR_SIZE;

        buffer = (char*)malloc3(num_sectors * CD_SECTOR_SIZE);
        if(!buffer) {
            printf("Failed to allocate memory for file: %s\n", filename);
            return buffer;
        }

        CdControl(CdlSetloc, (u_char*)&file.pos, 0); // Set the location of the file
        CdReadFile(buffer, (u_long*)buffer, CdlModeSpeed); // Read the file
        CdReadSync(0, 0); // wait for the read to finish

        *size = file.size;
    } else {
        printf("File not found: %s\n", filename);
    }

    return buffer;
}