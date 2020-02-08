#include <filters.h>
#include <image.h>
#include <source.h>
#include <cstddef>
#include <sink.h>
#include <logging.h>

void Filter::Update(void){
	if(img1 != NULL){
		char msg[128];
		sprintf(msg, "%s: about to update input1", FilterName());
		Logger::LogEvent(msg);
		img1->Update();
		sprintf(msg, "%s: done updating input1", FilterName());
		Logger::LogEvent(msg);

		if(img2 != NULL){
			char msg[128];
			sprintf(msg, "%s: about to update input2", FilterName());
			Logger::LogEvent(msg);
			img2->Update();
			sprintf(msg, "%s: done updating input2", FilterName());
			Logger::LogEvent(msg);
		}

		sprintf(msg, "%s: about to execute", FilterName());
		Logger::LogEvent(msg);
		Execute();
		sprintf(msg, "%s: done executing", FilterName());
		Logger::LogEvent(msg);
	}


}

Color::Color(int width, int height, unsigned char r, unsigned char g, unsigned char b){
	GetOutput()->ResetSize(width, height);
	this->red = r;
	this->green = g;
	this->blue = b;
	Execute();
}

void Color::Update(void){
    char msg[128];
    sprintf(msg, "Color: about to execute");
    Logger::LogEvent(msg);
    Execute();
    sprintf(msg, "Color: done executing");
    Logger::LogEvent(msg);
}

void Color::Execute(void){
    for(int i = 0; i < GetOutput()->getH(); i++){
        for(int j = 0; j < GetOutput()->getW(); j++){
            GetOutput()->GetBuff()[GetOutput()->getW()*i + j].r = this->red;
            GetOutput()->GetBuff()[GetOutput()->getW()*i + j].g = this->green;
            GetOutput()->GetBuff()[GetOutput()->getW()*i + j].b = this->blue;
        }
    }
}

CheckSum::CheckSum(void){
	this->redSum = 0;
	this->greenSum = 0;
	this->blueSum = 0;
}

void CheckSum::OutputCheckSum(const char *fileName){
	char msg[128];
	sprintf(msg, "CheckSum: about to output CheckSum");
	Logger::LogEvent(msg);

	FILE *out = fopen(fileName, "w");
	for(int i = 0; i < GetImage()->getH(); i++){
        for(int j = 0; j < GetImage()->getW(); j++){
            this->redSum += GetImage()->GetBuff()[GetImage()->getW()*i + j].r;
            this->greenSum += GetImage()->GetBuff()[GetImage()->getW()*i + j].g;
            this->blueSum += GetImage()->GetBuff()[GetImage()->getW()*i + j].b;
        }
    }
	fprintf(out, "CHECKSUM: %d, %d, %d\n", this->redSum, this->greenSum, this->blueSum);
	sprintf(msg, "CheckSum: done outputing CheckSum file");
	Logger::LogEvent(msg);
	fclose(out);
}
	
	
const char *Shrinker::FilterName(void){
	return "Shrinker";
}

void Shrinker::Execute(void){
	// Set size
    int halfWidth = GetImage()->getW() / 2;
	int halfHeight = GetImage()->getH() / 2;
    GetOutput()->ResetSize(halfWidth, halfHeight);
    // (i, j) in this->GetImage()->= (2*i, 2*j) in input
    for(int i = 0; i < halfHeight; i++){
        for(int j = 0; j < halfWidth; j++){
            unsigned char r = GetImage()->GetBuff()[2*GetImage()->getW()*i + 2*j].r;
            GetOutput()->GetBuff()[halfWidth*i + j].r = r;
            unsigned char g = GetImage()->GetBuff()[2*GetImage()->getW()*i + 2*j].g;
            GetOutput()->GetBuff()[halfWidth*i + j].g = g;
            unsigned char b = GetImage()->GetBuff()[2*GetImage()->getW()*i + 2*j].b;
            GetOutput()->GetBuff()[halfWidth*i + j].b = b;
        }
    }
}

const char *LRConcat::FilterName(void){
	return "LRConcat";
}

void LRConcat::Execute(void){
	
	// Input2 error
	if(GetImage2() == NULL){
		char msg[1024];
		sprintf(msg, "LRConcat: no input2!");
		DataFlowException e("LRConcat", msg);
		throw e;
	}
	// Check for height exception
	if(GetImage()->getH() != GetImage2()->getH()){
		char msg[1024];
		sprintf(msg, "LRConcat: heights must match: %d, %d", GetImage()->getH(), GetImage2()->getH());
		DataFlowException e("LRConcat", msg);
		throw e;
	}
	 
	// Set this->GetImage()->size
    int newWidth = GetImage()->getW() + GetImage2()->getW();

    GetOutput()->ResetSize(newWidth, GetImage()->getH());

    // I used two loops for futre proofing in case images dont have the same width
    // Adds in left image
    for(int i = 0; i < GetOutput()->getH(); i++){
        for(int j = 0; j < newWidth; j++){
            if( j < GetImage()->getW()){
                unsigned char r = GetImage()->GetBuff()[(GetImage()->getW())*i + j].r;
                GetOutput()->GetBuff()[newWidth*i + j].r = r;
                unsigned char g = GetImage()->GetBuff()[(GetImage()->getW() )*i + j].g;
                GetOutput()->GetBuff()[newWidth*i + j].g = g;
                unsigned char b = GetImage()->GetBuff()[(GetImage()->getW() )*i + j].b;
                GetOutput()->GetBuff()[newWidth*i + j].b = b;
            } else{
                // Use Image2 
                unsigned char r2 = GetImage2()->GetBuff()[(GetImage2()->getW())*i + (j - GetImage()->getW())].r;
                GetOutput()->GetBuff()[newWidth*i + j   ].r = r2;
                unsigned char g2 = GetImage2()->GetBuff()[(GetImage2()->getW())*i + (j - GetImage()->getW())].g;
                GetOutput()->GetBuff()[newWidth*i + j   ].g = g2;
                unsigned char b2 = GetImage2()->GetBuff()[(GetImage2()->getW())*i + (j - GetImage()->getW())].b;
                GetOutput()->GetBuff()[newWidth*i + j   ].b = b2;
            }
        }
    }
}

const char *TBConcat::FilterName(void){
	return "TBConcat";
}

void TBConcat::Execute(void){
	if(GetImage2() == NULL){
		char msg[1024];
		sprintf(msg, "TBConcat: no input2!");
		DataFlowException e("TBConcat", msg);
		throw e;
	}
	if(GetImage()->getW() != GetImage2()->getW()){
		char msg[1024];
		sprintf(msg, "TBConcat: widths must match: %d, %d", GetImage()->getW(), GetImage2()->getW());
		DataFlowException e("TBConcat", msg);
		throw e;
	}
	// Set size
    int newHeight = GetImage()->getH() + GetImage2()->getH();
    GetOutput()->ResetSize(GetImage()->getW(), newHeight);

    // I used two loops for futre proofing in case images dont have the same height
    // Adds in top image
    for(int i = 0; i < newHeight; i++){
        for(int j = 0; j < GetImage()->getW(); j++){
            if(i < GetImage()->getH()){
                unsigned char r = GetImage()->GetBuff()[GetImage()->getW()*i + j].r;
                unsigned char g = GetImage()->GetBuff()[GetImage()->getW()*i + j].g;
                unsigned char b = GetImage()->GetBuff()[GetImage()->getW()*i + j].b;

                GetOutput()->GetBuff()[GetImage()->getW()*i + j].r = r;
                GetOutput()->GetBuff()[GetImage()->getW()*i + j].g = g;
                GetOutput()->GetBuff()[GetImage()->getW()*i + j].b = b;
            } else{
                // Use Image2
                unsigned char r2 = GetImage2()->GetBuff()[(GetImage2()->getW())*(i - GetImage()->getH()) + j].r;
                unsigned char g2 = GetImage2()->GetBuff()[(GetImage2()->getW())*(i - GetImage()->getH()) + j].g;
                unsigned char b2 = GetImage2()->GetBuff()[(GetImage2()->getW())*(i - GetImage()->getH()) + j].b;

                GetOutput()->GetBuff()[(GetImage()->getW())*(i ) + j].r = r2;
                GetOutput()->GetBuff()[(GetImage()->getW())*(i ) + j].g = g2;
                GetOutput()->GetBuff()[(GetImage()->getW())*(i ) + j].b = b2;
            }
        }
    }
}

const char *Blender::FilterName(void){
	return "Blender";
}
void Blender::SetFactor(float f){
	this->factor = f;
}

float Blender::GetFactor(void){
	return this->factor;
}

void Blender::Execute(void){
    if(GetFactor() <= 0.5){
        char msg[1024];
        sprintf(msg, "Blender: no invalid factor!");
        DataFlowException e("Blender", msg);
        throw e;
    }
    if(GetImage2() == NULL){
        char msg[1024];
        sprintf(msg, "Blender: no input2!");
        DataFlowException e("Blender", msg);
        throw e;
    }
    // Valid factor
    if(GetFactor() >= 1.0){
        char msg[1024];
        sprintf(msg, "Invalid factor for Blender: %f", GetFactor());
        DataFlowException e("Blender", msg);
        throw e;
    }
    // Set GetImage()->size
    int width = GetImage()->getW();
    int height = GetImage()->getH();
    GetOutput()->ResetSize(width, height);
    float factor = GetFactor();
    float lowFtr = 1.0 - factor;
    // Loop goes through and sets pixels to factor*image1 + (1-factor)*image2
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            unsigned char r = factor*GetImage()->GetBuff()[width*i + j].r +lowFtr*GetImage2()->GetBuff()[width*i + j].r;
            unsigned char g = factor*GetImage()->GetBuff()[width*i + j].g +lowFtr*GetImage2()->GetBuff()[width*i + j].g;
            unsigned char b = factor*GetImage()->GetBuff()[width*i + j].b +lowFtr*GetImage2()->GetBuff()[width*i + j].b;

            GetOutput()->GetBuff()[width*i + j].r = r;
            GetOutput()->GetBuff()[width*i + j].g = g;
            GetOutput()->GetBuff()[width*i + j].b = b;
        }
    }
}

const char *Mirror::FilterName(void){
	return "Mirror";
}

void Mirror::Execute(void){
	GetOutput()->ResetSize(GetImage()->getW(), GetImage()->getH());
    for(int i = 0; i < GetImage()->getH(); i++){
        for(int j = 0; j < GetImage()->getW(); j++){
            unsigned char r = GetImage()->GetBuff()[GetImage()->getW()*i + ((GetImage()->getW() - 1)- j)].r;
            unsigned char g = GetImage()->GetBuff()[GetImage()->getW()*i + ((GetImage()->getW() - 1)- j)].g;
            unsigned char b = GetImage()->GetBuff()[GetImage()->getW()*i + ((GetImage()->getW() - 1)- j)].b;
            
            GetOutput()->GetBuff()[GetImage()->getW()*i + j].r = r;
            GetOutput()->GetBuff()[GetImage()->getW()*i + j].g = g;
            GetOutput()->GetBuff()[GetImage()->getW()*i + j].b = b;
        }
    }
}

const char *Rotate::FilterName(void){
	return "Rotate";
}

void Rotate::Execute(void){
	GetOutput()->ResetSize(GetImage()->getH(), GetImage()->getW());
    for(int i = 0; i < GetImage()->getH(); i++){
        for(int j = 0; j < GetImage()->getW(); j++){
    		unsigned char r = GetImage()->GetBuff()[GetImage()->getW()*i + j].r;
    		unsigned char g = GetImage()->GetBuff()[GetImage()->getW()*i + j].g;
    		unsigned char b = GetImage()->GetBuff()[GetImage()->getW()*i + j].b;

    		GetOutput()->GetBuff()[GetOutput()->getW()*j + ((GetOutput()->getW() - 1) - i)].r = r;
    		GetOutput()->GetBuff()[GetOutput()->getW()*j + ((GetOutput()->getW() - 1) - i)].g = g;
    		GetOutput()->GetBuff()[GetOutput()->getW()*j + ((GetOutput()->getW() - 1) - i)].b = b;
        }
    }
}

const char *Subtract::FilterName(void){
	return "Subtract";
}

void Subtract::Execute(void){
	if(GetImage2() == NULL){
		char msg[1024];
		sprintf(msg, "Subtract: no input2!");
		DataFlowException e("Subtract", msg);
		throw e;
	}
	if(GetImage()->getW() != GetImage2()->getW() | GetImage()->getH() != GetImage2()->getH()){
	    char msg[1024];
	    sprintf(msg, "Invalid dimensions for Subtract: %dx%d and %dx%d", 
                GetImage()->getW(), GetImage()->getH(), GetImage2()->getW(), GetImage2()->getH());
        DataFlowException e("Subtract", msg);
	    throw e;
	}
	GetOutput()->ResetSize(GetImage()->getW(), GetImage()->getH());
    for(int i = 0; i < GetImage()->getH(); i++){
        for(int j = 0; j < GetImage()->getW(); j++){
    		unsigned char r;
            unsigned char r1 = GetImage()->GetBuff()[GetImage()->getW()*i + j].r;
            unsigned char r2 = GetImage2()->GetBuff()[GetImage2()->getW()*i + j].r;
        	if(r1 > r2){
           	    r = r1 - r2;
	       	} else{
		       r = 0;
            }
            GetOutput()->GetBuff()[GetImage()->getW()*i + j].r = r;
            unsigned char g;
            unsigned char g1 = GetImage()->GetBuff()[GetImage()->getW()*i + j].g;
            unsigned char g2 = GetImage2()->GetBuff()[GetImage2()->getW()*i + j].g;
            if(g1 > g2){
           	    g = g1 - g2;
            } else{
                g = 0;
            }
            GetOutput()->GetBuff()[GetImage()->getW()*i + j].g = g;
            unsigned char b;
            unsigned char b1 = GetImage()->GetBuff()[GetImage()->getW()*i + j].b;
            unsigned char b2 = GetImage2()->GetBuff()[GetImage2()->getW()*i + j].b;
            if(b1 > b2){
           	    b = b1 - b2;
            } else{
                b = 0;
            }
            GetOutput()->GetBuff()[GetImage()->getW()*i + j].b = b;
        }
    }
}

const char *Grayscale::FilterName(void){
	return "Grayscale";
}

void Grayscale::Execute(void){
	GetOutput()->ResetSize(GetImage()->getW(), GetImage()->getH());
    for(int i = 0; i < GetImage()->getH(); i++){
        for(int j = 0; j < GetImage()->getW(); j++){
            unsigned char r = GetImage()->GetBuff()[GetImage()->getW()*i + j].r;
            unsigned char g = GetImage()->GetBuff()[GetImage()->getW()*i + j].g;
            unsigned char b = GetImage()->GetBuff()[GetImage()->getW()*i + j].b;
            unsigned char gray = (r/5) + (g/2) + (b/4);

            GetOutput()->GetBuff()[GetImage()->getW()*i + j].r = gray;
            GetOutput()->GetBuff()[GetImage()->getW()*i + j].g = gray;
            GetOutput()->GetBuff()[GetImage()->getW()*i + j].b = gray;
        }
    }
}

const char *Blur::FilterName(void){
	return "Blur";
}

void Blur::Execute(void){
	GetOutput()->ResetSize(GetImage()->getW(), GetImage()->getH());
    for(int i = 0; i < GetImage()->getH(); i++){
        for(int j = 0; j < GetImage()->getW(); j++){
	        if(i == 0 | i == (GetImage()->getH() - 1) | j == 0 | j == (GetImage()->getW() - 1)){
                unsigned char r = GetImage()->GetBuff()[GetImage()->getW()*i + j].r;
                unsigned char g = GetImage()->GetBuff()[GetImage()->getW()*i + j].g;
                unsigned char b = GetImage()->GetBuff()[GetImage()->getW()*i + j].b;

                GetOutput()->GetBuff()[GetImage()->getW()*i + j].g = g;
                GetOutput()->GetBuff()[GetImage()->getW()*i + j].r = r;
                GetOutput()->GetBuff()[GetImage()->getW()*i + j].b = b;
            } else{
    		    unsigned char r1 = GetImage()->GetBuff()[(GetImage()->getW()*(i - 1)) + (j-1)].r / 8;
    		    unsigned char r2 = GetImage()->GetBuff()[(GetImage()->getW()*i) + (j-1)].r / 8;
    		    unsigned char r3 = GetImage()->GetBuff()[(GetImage()->getW()*(i + 1)) + (j-1)].r / 8;
    		    unsigned char r4 = GetImage()->GetBuff()[(GetImage()->getW()*(i - 1)) + (j)].r / 8;
    		    unsigned char r5 = GetImage()->GetBuff()[(GetImage()->getW()*(i + 1)) + (j)].r / 8;
    		    unsigned char r6 = GetImage()->GetBuff()[(GetImage()->getW()*(i - 1)) + (j+1)].r / 8;
    		    unsigned char r7 = GetImage()->GetBuff()[(GetImage()->getW()*i) + (j+1)].r / 8;
    		    unsigned char r8 = GetImage()->GetBuff()[(GetImage()->getW()*(i + 1)) + (j+1)].r / 8;
    		    unsigned char r = (r1 + r2 + r3 + r4 + r5 + r6 + r7 + r8);

    		    unsigned char g1 = GetImage()->GetBuff()[(GetImage()->getW()*(i - 1)) + (j-1)].g / 8;
    		    unsigned char g2 = GetImage()->GetBuff()[(GetImage()->getW()*i) + (j-1)].g / 8;
    		    unsigned char g3 = GetImage()->GetBuff()[(GetImage()->getW()*(i + 1)) + (j-1)].g / 8;
    		    unsigned char g4 = GetImage()->GetBuff()[(GetImage()->getW()*(i - 1)) + (j)].g / 8;
    		    unsigned char g5 = GetImage()->GetBuff()[(GetImage()->getW()*(i + 1)) + (j)].g / 8;
    		    unsigned char g6 = GetImage()->GetBuff()[(GetImage()->getW()*(i - 1)) + (j+1)].g / 8;
    		    unsigned char g7 = GetImage()->GetBuff()[(GetImage()->getW()*i) + (j+1)].g / 8;
    		    unsigned char g8 = GetImage()->GetBuff()[(GetImage()->getW()*(i + 1)) + (j+1)].g / 8;
    		    unsigned char g = (g1 + g2 + g3 + g4 + g5 + g6 + g7 + g8);

    		    unsigned char b1 = GetImage()->GetBuff()[(GetImage()->getW()*(i - 1)) + (j-1)].b / 8;
    		    unsigned char b2 = GetImage()->GetBuff()[(GetImage()->getW()*i) + (j-1)].b / 8;
    		    unsigned char b3 = GetImage()->GetBuff()[(GetImage()->getW()*(i + 1)) + (j-1)].b / 8;
    		    unsigned char b4 = GetImage()->GetBuff()[(GetImage()->getW()*(i - 1)) + (j)].b / 8;
    		    unsigned char b5 = GetImage()->GetBuff()[(GetImage()->getW()*(i + 1)) + (j)].b / 8;
    		    unsigned char b6 = GetImage()->GetBuff()[(GetImage()->getW()*(i - 1)) + (j+1)].b / 8;
    		    unsigned char b7 = GetImage()->GetBuff()[(GetImage()->getW()*i) + (j+1)].b / 8;
    		    unsigned char b8 = GetImage()->GetBuff()[(GetImage()->getW()*(i + 1)) + (j+1)].b / 8;
    		    unsigned char b = (b1 + b2 + b3 + b4 + b5 + b6 + b7 + b8);

                GetOutput()->GetBuff()[GetImage()->getW()*i + j].r = r;
                GetOutput()->GetBuff()[GetImage()->getW()*i + j].g = g;
                GetOutput()->GetBuff()[GetImage()->getW()*i + j].b = b;
            }
	    }
    }
}
