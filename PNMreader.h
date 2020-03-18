#include <source.h>

#ifndef READER_PNME
#define READER_PNME
class PNMreader : public Source{
    public:
        void Update();
        PNMreader(char *);
        virtual ~PNMreader();
    protected:
        void Execute();
        char *filename;
};
#endif
