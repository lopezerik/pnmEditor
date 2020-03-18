#include <image.h>
#include <stdlib.h>
#include <stdio.h>
#include <PNMwriter.h>
#include <sink.h>

void PNMwriter::Write(const char *filename){ 
    if(filename == NULL){
        // User did not specify an output file
        printf("pnmEditor: missing argument\n");
        printf("pnmEditor: Try 'pnmEditor <input file> <output file>'\n");
        exit(EXIT_FAILURE);
    }

    // Open file to write in binary to
    FILE *f_out = fopen(filename, "wb");
    // Check f_out for errors
    if(f_out == NULL){
        printf("Unable to open file in PNMwriter \"%s\".\n", filename);
        exit(EXIT_FAILURE);
    }

    // Add in header
    // "Magic number" = P6, specifies a binary color image w/ 0-255 RGB channels
    fprintf(f_out, "%s\n%d %d\n%d\n", "P6", GetImage()->getW(), GetImage()->getH(), 255);
    // Write our buffer to our file
    fwrite(GetImage()->GetBuff(), 3, GetImage()->getW()*GetImage()->getH(), f_out);
    // Close our file
    fclose(f_out);
}

void PNMwriter::SetInput(Image *ptr){
    Sink::SetInput(ptr);
}
