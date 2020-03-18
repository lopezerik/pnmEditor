#include <image.h>
#include <source.h>

Source::Source(void){
    img.setSrc(this);
}

Image* Source::GetOutput(void){
    return &(this->img);
}
