#include <image.h>
#include <stdlib.h>
#include <source.h>
#include <string.h>
#include <stdio.h>
#include <PNMreader.h>
#include <logging.h>

PNMreader::PNMreader(char *fileInput){
    if(fileInput == NULL){
        // User did not specify an input file
        printf("pnmEditor: missing argument\n");
        printf("pnmEditor: Try 'pnmEditor <input file> <output file>'\n");
        exit(EXIT_FAILURE);
    }
    this->filename = new char[strlen(fileInput) + 1];
    memmove(this->filename, fileInput, strlen(fileInput) + 1);
}

void PNMreader::Update(void){
    char msg[128];
    sprintf(msg, "PNMreader: about to execute");
    Logger::LogEvent(msg);
    Execute();
    sprintf(msg, "PNMreader: done executing");
    Logger::LogEvent(msg);
}

void PNMreader::Execute(){
    FILE *f_in = fopen(this->filename, "rb");
    if(f_in == NULL){
        printf("Unable to open file in PNMreader \"%s\".\n", this->filename);
        char msg[1024];
        sprintf(msg, "Unable to open file in PNMreader ");
        DataFlowException e("LRJoin", msg);
        throw e;
        exit(EXIT_FAILURE);
    }
    char magicNum[128];
    int width, height, maxval;
    fscanf(f_in, "%s\n%d %d\n%d\n", magicNum, &width, &height, &maxval);

    // Allocate Image
    this->GetOutput()->ResetSize(width, height);
    // Read into our buffer
    fread(this->GetOutput()->GetBuff(), 3, width*height, f_in);
    fclose(f_in);
    }

// "~" = C++ desctructor operator
// Class desctructor
PNMreader::~PNMreader(){
    if(this->filename != NULL){
        delete [] (this->filename);
        this->filename = NULL;
    }
}
