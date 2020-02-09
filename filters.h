#include <source.h>
#include <sink.h>
#include <image.h>
#include <stdio.h>

#ifndef FILTER_PNME
#define FILTER_PNME
class Filter : public Source, public Sink{
    virtual void Update();
    virtual const char *FilterName() = 0;
    protected:
        void executeWorker(int, int, bool);
};
#endif

#ifndef COLOR_PNME
#define COLOR_PNME
class Color : public Source{
    public:
        Color(int w, int h, unsigned char r, unsigned char g, unsigned char b);
        void Update();
    protected:  
        void Execute();
        unsigned char red;
        unsigned char green;
        unsigned char blue;
};
#endif

#ifndef CHECKSUM_PNME
#define CHECKSUM_PNME
class CheckSum : public Sink{
    public:
        CheckSum();
        void OutputCheckSum(const char *);
    protected:
        unsigned char redSum;
        unsigned char greenSum;
        unsigned char blueSum;
};
#endif

#ifndef SHRINK_PNME
#define SHRINK_PNME
class Shrink : public Filter{
    protected:
        void Execute();
        const char *FilterName();
};
#endif

#ifndef LRJOIN_PNME
#define LRJOIN_PNME
class LRJoin : public Filter{
    protected:
        void Execute();
        const char *FilterName();
};
#endif

#ifndef TBJOIN_PNME
#define TBJOIN_PNME
class TBJoin : public Filter{
    protected:
        void Execute();
        const char *FilterName();
};
#endif

#ifndef BLEND_PNME
#define BLEND_PNME
class Blend : public Filter{
    public:
        void SetFactor(float);
        float GetFactor();
    protected:
        const char *FilterName();
        void Execute();
        float factor;
};
#endif

#ifndef MIRROR_PNME
#define MIRROR_PNME
class Mirror : public Filter{
    protected:
        void Execute();
        const char *FilterName();
};
#endif

#ifndef ROTATE_PNME
#define ROTATE_PNME
class Rotate : public Filter{
    protected:
        void Execute();
        const char *FilterName();
};
#endif

#ifndef SUB_PNME
#define SUB_PNME
class Subtract : public Filter{
    protected:
        void Execute();
        const char *FilterName();
};
#endif

#ifndef GRAY_PNME
#define GRAY_PNME
class Grayscale : public Filter{
    protected:
        void Execute();
        const char *FilterName();
};
#endif

#ifndef BLUR_PNME
#define BLUR_PNME
class Blur : public Filter{
    protected:
        void Execute();
        const char *FilterName();
};
#endif
