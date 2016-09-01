#include <iostream>
#include <windows.h>
#include <fstream>
#include <ctime>
#include <string>
#include <cstdlib>
#include <cmath>

using namespace std;

struct bmp{
	
	int width;
	int height;
	
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;
	
	size_t bitmapFileHeaderSize;
	size_t bitmapInfoHeaderSize;
	
	char* fileheader;
	char* infoheader;
	
	int filesize;
    
    bmp(int _width, int _height){
    	
		width=_width;
    	height=_height;
    	
    	filesize = width*height*3;
    	
    	bitmapFileHeaderSize = sizeof(bitmapFileHeader);
    	bitmapInfoHeaderSize = sizeof(bitmapInfoHeader);
    	
    	memset( &bitmapFileHeader, 0, bitmapFileHeaderSize );
    	bitmapFileHeader.bfType = ( 'B' | 'M' << 8 );
    	bitmapFileHeader.bfOffBits = bitmapFileHeaderSize + bitmapInfoHeaderSize;
    	bitmapFileHeader.bfSize = bitmapFileHeader.bfOffBits + width * height * 3;

    
    	memset( &bitmapInfoHeader, 0, bitmapInfoHeaderSize );
    	bitmapInfoHeader.biSize = bitmapInfoHeaderSize;
    	bitmapInfoHeader.biWidth = width;
    	bitmapInfoHeader.biHeight = height;
    	bitmapInfoHeader.biPlanes = 1;
    	bitmapInfoHeader.biBitCount = 24;
    	
    	fileheader = reinterpret_cast< char * >(&bitmapFileHeader);
    	infoheader = reinterpret_cast< char * >(&bitmapInfoHeader);
	}
};

struct Alphabet{
	int _alphabetlen;
	int _letterlen;
	int _letterheight;
	byte** alphabet;
	Alphabet (int alphabetlen, int letterlen, int letterheight){
		_alphabetlen = alphabetlen;
		_letterlen = letterlen;
		_letterheight = letterheight;
		alphabet = new byte*[_alphabetlen];
	}
};

void createBitmap( byte * imageData, const char * filename, int width, int height, bmp* _bmp )
{
    

    std::ofstream file( filename, std::fstream::binary );

    file.write( _bmp->fileheader, _bmp->bitmapFileHeaderSize );
    file.write( _bmp->infoheader, _bmp->bitmapInfoHeaderSize );

    file.write( (const char*)imageData, _bmp->filesize );

}

unsigned char* readBMP(const char* filename){
	int i;
    FILE* f = fopen(filename, "rb");
    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

    // extract image height and width from header
    int width = *(int*)&info[18];
    int height = *(int*)&info[22];

    int size = 3 * width * height;
    unsigned char* data = new unsigned char[size]; // allocate 3 bytes per pixel
    fread(data, sizeof(unsigned char), size, f); // read the rest of the data at once
    fclose(f);
    return data;
}

void makeLetter(unsigned char* data, Alphabet* alphabet, int num){
	for(int x=0;x<alphabet->_letterlen;x++){
    	for(int y=0;y<alphabet->_letterheight;y++){
			int pos = (x+y*alphabet->_letterlen);
			alphabet->alphabet[num][pos]=byte(data[pos*3]);	
		}
	}
}

void loadFont(string dict, Alphabet* alphabet){	
	for(int l = 0;l<alphabet->_alphabetlen;l++){
		alphabet->alphabet[l]= new byte[alphabet->_letterheight*alphabet->_letterlen];
		string letter="";
		letter+=dict[l];
		letter+=".bmp";
		unsigned char* data = readBMP(letter.c_str());
		makeLetter(data,alphabet,l);
		delete data;
	}
}

double period(){
    std::srand(std::time(0));
    return (rand()%45+75)/1000.0;
}

double phase(){
    std::srand(std::time(0));
    return (rand()%314)/100.0;
}

double amplitude(){
    std::srand(std::time(0));
    return (rand()%8+30)/10.0;
}

int main( int argc, const char * argv[] )
{
    string dict = "qwertyuiopasdfghjklzxcvbnm";
	int width = 128; 
    int height = 64;
    int lw = 32;
    int wordlen = 4;
    
    bmp _bmp(width,height);

    byte imageData[ width * height ];

    
    Alphabet alphabet(strlen(dict.data()),lw,height);
    
    loadFont(dict, &alphabet);
	
	int text[wordlen];
	
	srand(time(0));
		
	for(int i=0;i<wordlen;i++){
		text[i] = rand()%26;
	}

	for(register int c=0;c<wordlen;c++){
		int xlwc=lw*c;
		int lwcpl1 = lw*(c+1);	
		for(register int x=xlwc;x<lwcpl1;x++){
			for(register int y=0;y<height;y++){
					
				int pos =  (x + y * width);
					
				byte color = alphabet.alphabet[text[c]][x%lw+y*lw];
					
				imageData[pos]=color;
						
			}
		}
	}	
	byte newData[width*height*3] = {0xff};
	{
	//period
    double dx_period_x = period();
    double dx_period_y = period();
	double dy_period_x = period();
    double dy_period_y = period();
    //phase
    double dx_phase_x = phase();
    double dx_phase_y = phase();
	double dy_phase_x = phase();
	double dy_phase_y = phase();
    //amplitude
    double dx_amplitude = amplitude();
    double dy_amplitude = amplitude();
    //init
    //calc sin
    double dx_x[width];
    double dy_x[width];
    for(int x=0;x<width;x++){
    	dx_x[x]=sin(x * dx_period_x + dx_phase_x);
    	dy_x[x]=sin(x * dy_period_x + dy_phase_x);
	}
	double dx_y[height];
	double dy_y[height];
	for(int y=0;y<height;y++){
		dx_y[y] = sin(y * dx_period_y + dx_phase_y);
		dy_y[y] = sin(y * dy_period_y + dy_phase_y);
	}
	//rutine
	int wsub1 = width-1;
	int hsub1 = height-1;
    //color diff
    for(register int x=0;x<width;x++){
        for(register int y=0;y<height;y++){
            int color_diff = 0xff - imageData[x+y*width];
            if(!color_diff){
                continue;
            }
            //source x (double)
            double sx = x + (dx_x[x] + dx_y[y]) * dx_amplitude;
            if(!((0<=sx) && (sx<wsub1))){
                continue;
            }
            //source y (double)
        	double sy = y + (dy_x[x] + dy_y[y]) * dy_amplitude;
            if(!((0<=sy) && (sy<hsub1))){
                continue;
            }
            //
            int sx_i = int(sx);
            int sy_i = int(sy);
            double frx = sx - sx_i;
            double fry = sy - sy_i;
            int idx1 = sx_i + width*sy_i;
            int idx2 = idx1 + width;
            int tmp = int(color_diff * (1-frx) * (1-fry)); 
            int idx13=idx1*3;
            newData[idx13] -= tmp;
            newData[idx13+1] -=tmp;
            newData[idx13+2] -=tmp;
            tmp = int(color_diff * frx * (1-fry));
            int idx113=(idx1+1)*3;
            newData[idx113] -= tmp;
            newData[idx113+1] -= tmp;
            newData[idx113+2] -= tmp;
            tmp = int(color_diff * (1-frx) * fry);
            int idx23 = idx2*3;
            newData[idx23] -= tmp;
            newData[idx23+1]-= tmp;
            newData[idx23+2] -= tmp;
            tmp = int(color_diff * frx * fry);
            int idx213 = (idx2+1)*3;
            newData[idx213] -= tmp;
            newData[idx213+1] -= tmp;
            newData[idx213+2] -= tmp;
            }
        }
	}
	createBitmap( newData, "bitmap.bmp", width, height, &_bmp );

    return 0;
}
