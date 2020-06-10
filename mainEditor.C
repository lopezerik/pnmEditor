#include <PNMreader.h>
#include <PNMwriter.h>
#include <filters.h>

int main(int argc, char* argv[]){
    // Read in the pnm file specified by 'argv[1]'
    PNMreader reader(argv[1]);

    Image* inputImage = reader.GetOutput();

    // Example filter setup below
    // Create filter objects
    Blur br;
    Blur br2;
    Blur br3;
    Mirror mr;
    joinLR lr;
    Rotate rt;
    
    // Set filter order and heirachy
    br.SetInput(inputImage);
    br2.SetInput(br.GetOutput());
    br3.SetInput(br2.GetOutput());

    mr.SetInput(inputImage);

    lr.SetInput(br3.GetOutput());
    lr.SetInput2(mr.GetOutput());

    rt.SetInput(lr.GetOutput());

    // Update the final filter in order to execute all preceding filters
    rt.GetOutput()->Update();
    Image* finalImage = rt.GetOutput();

    // Write the image to the file specified in 'argv[2]'
    PNMwriter writer;
    writer.SetInput(finalImage);
    writer.Write(argv[2]);
}
