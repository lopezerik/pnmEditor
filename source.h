#ifndef SOURCE_PNME
#define SOURCE_PNME

#include <image.h>
class Source{
	public:
	    Source();
	    virtual void Update() = 0;
	    Image* GetOutput();
	protected:
	    virtual void Execute() = 0;
	    Image img;
};
#endif
