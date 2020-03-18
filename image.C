#include <image.h>
#include <stdlib.h>
#include <string.h>
#include <source.h>
#include <PNMreader.h>

Image::Image(void){
    this->maxval = 255;
    this->height = 0;
    this->width = 0;
    this->pixel = (Pixel *) malloc(sizeof(Pixel)*this->width*this->height);
    this->src = NULL;
}

Image::Image(int w, int h){
    this->width = w;
    this->height = h;
    this->maxval = 255;
    this->pixel = (Pixel *) malloc(sizeof(Pixel)*this->width*this->height);
    this->src = NULL;
}

Image::Image(Image &ptr){
    this->width = ptr.width;
    this->height = ptr.height;
    this->maxval = 255;
    this->pixel = (Pixel *) malloc(sizeof(Pixel)*this->width*this->height);
    memcpy(this->pixel, ptr.pixel, sizeof(Image));
    this->src = ptr.src;
}

void Image::ResetSize(int width, int height){
    this->width = width;
    this->height = height;
    free(this->pixel);
    this->pixel = (Pixel *) malloc(sizeof(Pixel)*this->width*this->height);
}

Pixel *Image::GetBuff(void) const{
    return this->pixel;
}

void Image::setW(int w){
    this->width = w;
}

int Image::getW(void) const{
    return this->width;
}

void Image::setH(int h){
    this->height = h;
}

int Image::getH(void) const{
    return this->height;
}

void Image::setMax(int m){
    this->maxval = m;
}

int Image::getMax(void) const{
    return this->maxval;
}

void Image::setSrc(Source *ptr){
    this->src = ptr;
}

void Image::Update(void) const{
    if(src != NULL){
        src->Update();
    }
}
