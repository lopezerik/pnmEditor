#include <image.h>
#ifndef SINK_PNME
#define SINK_PNME
class Sink{
    public:
	    Sink();
		void SetInput(Image *);
		void SetInput2(Image *);
		const Image *GetImage() const;
		const Image *GetImage2() const;
    protected:
		const Image *img1;
		const Image *img2;
};
#endif
