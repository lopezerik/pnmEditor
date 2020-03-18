#include <image.h>
#include <sink.h>

#ifndef WRITER_PNME
#define WRITER_PNME
class PNMwriter : public Sink{
    public:
        void Write(const char *);
        void SetInput(Image *);
};
#endif
