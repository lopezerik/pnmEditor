#ifndef PIXEL_PNME
#define PIXEL_PNME
struct Pixel{
    unsigned char r;
    unsigned char g;
    unsigned char b;
};
#endif

#ifndef IMAGE_PNME
#define IMAGE_PNME
// Forward decleration of class Source
class Source;
class Image{
    public:
        Image(void);
        Image(int w, int h);
        Image(Image &);
        Pixel *GetBuff() const;
        void setW(int);
        int getW() const;
        void setH(int);
        int getH() const;
        void setMax(int);
        int getMax() const;
        void Update() const;
        void setSrc(Source *source);
        void ResetSize(int width, int height);
    protected:
        Pixel *pixel;
        int width;
        int height;
        int maxval;
        Source *src;
};
#endif
