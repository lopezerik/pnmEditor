#include <image.h>
#include <sink.h>
#include <cstddef>

Sink::Sink(void){
    img1 = NULL;
    img2 = NULL;
}

void Sink::SetInput(Image *input){
    this->img1 = input;
}

void Sink::SetInput2(Image *input2){
    this->img2 = input2;
}

const Image *Sink::GetImage (void) const{
    return this->img1;
}

const Image *Sink::GetImage2 (void) const{
    return this->img2;
}
