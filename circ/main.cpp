#include <ctime>
#include <windows.h>
#include <string>
#include <fstream>
#include <cmath>
#include <iostream>
#include <ctime>



using namespace std;

struct bmp{
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;
	
	int bitmapFileHeaderSize;
	int bitmapInfoHeaderSize;
	
	int width;
	int height;
	
	bmp(int _width,int _height){
		
		width=_width;
		height=_height;
		
		bitmapFileHeaderSize = sizeof(bitmapFileHeader);
		bitmapInfoHeaderSize = sizeof(bitmapInfoHeader);
    
		memset( &bitmapFileHeader, 0, bitmapFileHeaderSize );
    	bitmapFileHeader.bfType = ( 'B' | 'M' << 8 );
    	bitmapFileHeader.bfOffBits = bitmapFileHeaderSize +bitmapInfoHeaderSize;
    	bitmapFileHeader.bfSize = bitmapFileHeader.bfOffBits + width * height * 3;
    
		memset( &bitmapInfoHeader, 0, bitmapInfoHeaderSize );
    	bitmapInfoHeader.biSize = bitmapInfoHeaderSize;
    	bitmapInfoHeader.biWidth = width;
    	bitmapInfoHeader.biHeight = height;
    	bitmapInfoHeader.biPlanes = 1;
    	bitmapInfoHeader.biBitCount = 24;
	}
};

struct rgb{
	byte _r;
	byte _g;
	byte _b;
	
	rgb(){
		rgb(0xff,0xff,0xff);
	}
	
	rgb(byte r, byte g, byte b){
		_r=r;
		_g=g;
		_b=b;
	}
	
	bool operator == (rgb rhs){
		return ((_r==rhs._r) && (_g==rhs._g) && (_b==rhs._b));
	}
	
	rgb operator +(rgb rhs){
		return (rgb(_r+rhs._r,_g+rhs._g,_b+rhs._b));
	}
	
	void operator +=(rgb rhs){
		_r+=rhs._r;
		_g+=rhs._g;
		_b+=rhs._b;
	}
	
	rgb operator -(rgb rhs){
		return (rgb(_r-rhs._r,_g-rhs._g,_b-rhs._b));
	}
	
	void operator -=(rgb rhs){
		_r-=rhs._r;
		_g-=rhs._g;
		_b-=rhs._b;
	}
	
	rgb operator *(double rhs){
		_r=byte(_r*rhs);
		_g=byte(_g*rhs);
		_b=byte(_b*rhs);
	}
	
	void operator=(rgb rhs){
		_r=rhs._r;
		_g=rhs._g;
		_b=rhs._b;
	}
	
	bool operator !(){
		return !((_r==0x00)&&(_g==0x00)&&(_b==0x00));
	}
};

byte* rgbToByte_arr(rgb* in,int len);

unsigned char* readBMP(const char* filename, int* width, int* height);

struct Alphabet{
	int _len;
	int _lenOfLetter;
	int _heightOfLetter;
	rgb** _alphabet;//TODO ???????
	
	Alphabet(const char* dict, const char*  fontName){
		_len = strlen(dict);
		_alphabet = new rgb*[_len];
		for(register int i=0;i<_len;i++){
			string letter=fontName;
			letter+="\\";
			letter+=dict[i];
			letter+=".bmp";
			unsigned char* data = readBMP(letter.c_str(),&_lenOfLetter,&_heightOfLetter);//TODO: ????????? ??????????? ?? ?????????? ????? ?????
			_alphabet[i]= new rgb[_lenOfLetter*_heightOfLetter];
			for(register int x=0;x<_lenOfLetter;x++){
				for(register int y=0;y<_heightOfLetter;y++){
					int pos = (x+y*_lenOfLetter);
					_alphabet[i][pos]=rgb(data[3*pos],data[3*pos+1],data[3*pos+2]);
				}
			}
		}
	}
		
	
	~Alphabet(){
		for(register int i=0;i<_len;i++){
			delete _alphabet[i];
		}
		delete _alphabet;
	}
	
};

rgb* genText(const int lenOfWord, const Alphabet* alphabet, int* length);
int strToInt(const char* str);
void createBitmap( byte * imageData, const char * filename, bmp* _bmp );
rgb* wave(rgb* data, int width, int height, rgb bgc);
double period();
double phase();
double amplitude();

int main(int argc, char* argv[]){
	
	//TODO: сделать дефолтные значение в конфиг файле
	
	int lenOfWord = 4;
	try{
		lenOfWord = strToInt(argv[1]);
		//cout<<"lenOfWord:"<<lenOfWord<<endl;
	}catch(exception e){
		cout<<"len of word failed"<<endl;
	}
	
	char* dict = "qwertyuiopasdfghjklzxcvbnm";
	try{
		dict = argv[2];
		//cout<<"dict:"<<dict<<endl;	
	}catch(exception e){
		cout<<"dict failed"<<endl;
	}
	
	char* mode = "wave";
	try{
		mode = argv[3];
		//cout<<"mode:"<<mode<<endl;
	}catch(exception e){
		cout<<"mode failed"<<endl;
	}
	
	try{
		char* color = argv[4];
		//cout<<"color:"<<color<<endl;
	}catch(exception e){
		cout<<"color failed"<<endl;
	}
	
	char* fontName = "font";
	try{
		fontName = argv[5];
		//cout<<"font:"<<fontName<<endl;
	}catch(exception e){
		cout<<"font failed"<<endl;
	}
	
	int width;
	int height;
		
	//init args TODO: try catch обработка параметров; значения по умолчанию; оптимизироватьт порядок аргументов
	//количтесво символов в капче!
	//-словарь!
	//тип алгоритма искажения?значение по умолчанипю(первичный)
	//цвет(необязательный)?по умолчанию отсутсвует
	//ширина и высота генерируемой картинки?задать по умолчанию используемые 
	//TODO: управление этим параметромо необходимо создат после утилиты конвертации шрифта в битмап

	
	Alphabet alphabet(dict,fontName);
	
	height = alphabet._heightOfLetter;
	width = alphabet._lenOfLetter*lenOfWord;
	
	bmp _bmp(width,height);
	
	cout<<height<<" "<<width;
		
	int length;
	rgb bgc(0xff,0xff,0xff);
	
	cout<<"full:\n";
	int start = clock();
	for(int i=0;i<1000;i++){
		rgb* data = genText(lenOfWord,&alphabet,&length);
		data = wave(data,128,64,bgc);
		byte* byteData = rgbToByte_arr(data,length);
		createBitmap(byteData,"wave.bmp",&_bmp);//TODO	
	}
	int total = clock()-start;
	double avg=total/1000.0;	
	cout<<"total:"<<total<<endl;
	cout<<"avg:"<<avg<<endl;
	
	cout<<"without save:\n";
	start = clock();
	for(int i=0;i<1000;i++){
		rgb* data = genText(lenOfWord,&alphabet,&length);
		data = wave(data,128,64,bgc);
		byte* byteData = rgbToByte_arr(data,length);
		//createBitmap(byteData,"wave.bmp",&_bmp);//TODO	
	}
	total = clock()-start;
	avg=total/1000.0;	
	cout<<"total:"<<total<<endl;
	cout<<"avg:"<<avg<<endl;
	
	cout<<"without save and cast:\n";
	start = clock();
	for(int i=0;i<1000;i++){
		rgb* data = genText(lenOfWord,&alphabet,&length);
		data = wave(data,128,64,bgc);
		//byte* byteData = rgbToByte_arr(data,length);
		//createBitmap(byteData,"wave.bmp",&_bmp);//TODO	
	}
	total = clock()-start;
	avg=total/1000.0;	
	cout<<"total:"<<total<<endl;
	cout<<"avg:"<<avg<<endl;
	
	cout<<"without wave:\n";
	start = clock();
	for(int i=0;i<1000;i++){
		rgb* data = genText(lenOfWord,&alphabet,&length);
		//data = wave(data,128,64,bgc);
		//byte* byteData = rgbToByte_arr(data,length);
		//createBitmap(byteData,"wave.bmp",&_bmp);//TODO	
	}
	total = clock()-start;
	avg=total/1000.0;	
	cout<<"total:"<<total<<endl;
	cout<<"avg:"<<avg<<endl;

	
	//delete data;
	//delete byteData;
	
	char* someVar;
	cin>>someVar;
	
	return 0;
}

void createBitmap( byte * imageData, const char * filename, bmp* _bmp){

    std::ofstream file( filename, std::fstream::binary );

    file.write( reinterpret_cast< char * >( &(_bmp->bitmapFileHeader)), _bmp->bitmapFileHeaderSize );
    file.write( reinterpret_cast< char * >( &(_bmp->bitmapInfoHeader) ), _bmp->bitmapInfoHeaderSize );
    file.write( (const char*)imageData, _bmp->width * _bmp->height * 3 );
}

rgb* genText(const int lenOfWord, const Alphabet* alphabet, int* length){
	int width = lenOfWord*alphabet->_lenOfLetter;
	int height = alphabet->_heightOfLetter;
	rgb* data = new rgb[width*height];
	srand(time(0));
	int word[lenOfWord];
	for(int i=0;i<lenOfWord;i++){
		word[i]=rand()%alphabet->_len;
	}
	for(register int l=0;l<lenOfWord;l++){
		for(register int x=alphabet->_lenOfLetter*l;x<alphabet->_lenOfLetter*(l+1);x++){
			for(register int y=0;y<alphabet->_heightOfLetter;y++){
				int pos=(x+y*width);
				rgb color = alphabet->_alphabet[word[l]][x%alphabet->_lenOfLetter+y*alphabet->_lenOfLetter];
				data[pos]=color;
			}
		}
	}
	*length = width*height;
	return data;
}

unsigned char* readBMP(const char* filename, int* _width, int* _height){
	int i;
    FILE* f = fopen(filename, "rb");
    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

    // extract image height and width from header
    int width = *(int*)&info[18];
    int height = *(int*)&info[22];
    
    *_width = width;
    *_height = height;

    int size = 3 * width * height;
    unsigned char* data = new unsigned char[size]; // allocate 3 bytes per pixel
    fread(data, sizeof(unsigned char), size, f); // read the rest of the data at once
    fclose(f);
    return data;
}

int strToInt(const char* str){
	int len = strlen(str);
	int sum =0;
	for(int i=len-1;i>=0;i--){
		int n = (int)str[i]-48;
		sum+=n*pow(10,len-i-1);
	}
	return sum;
}

byte* rgbToByte_arr(rgb* in,int len){
	byte* out = new byte[len*3];
	for(int i=0;i<len;i++){
		int pos = i*3;
		out[pos]=in[i]._r;
		out[pos+1]=in[i]._g;
		out[pos+2]=in[i]._b;
	}
	return out;
}

rgb* wave(rgb* data, int width, int height, rgb bgc){
	rgb* newData = new rgb[width*height];
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
    //color diff
    for(register int x=0;x<width;x++){
        for(register int y=0;y<height;y++){
            rgb color_diff = bgc - data[x+y*width];
            if(!color_diff){
                continue;
                newData[x+y*width]=bgc;
            }
            //source x (double)
            double dx_x = sin(x * dx_period_x + dx_phase_x);
            double dx_y = sin(y * dx_period_y + dx_phase_y);
            double sx = x + (dx_x + dx_y) * dx_amplitude;
            if(!((0<=sx) && (sx<width-1))){
                continue;
                 newData[x+y*width]=bgc;
            }
            //source y (double)
            double dy_x = sin(x * dy_period_x + dy_phase_x);
            double dy_y = sin(y * dy_period_y + dy_phase_y);
        	double sy = y + (dy_x + dy_y) * dy_amplitude;
            if(!((0<=sy) && (sy<height-1))){
                continue;
                 newData[x+y*width]=bgc;
            }
            //
            int sx_i = int(sx);
            int sy_i = int(sy);
            double frx = sx - sx_i;
            double fry = sy - sy_i;
            int idx1 = sx_i + width*sy_i;
            int idx2 = idx1 + width;
            rgb tmp = color_diff * (1-frx) * (1-fry); 
            newData[idx1] += tmp;
            tmp = color_diff * frx * (1-fry);
            newData[(idx1+1)] += tmp;
            tmp = color_diff * (1-frx) * fry;
            newData[idx2] -= tmp;
            tmp = color_diff * frx * fry;
            newData[(idx2+1)] -= tmp;
        }
    }
    return newData;
}

double period(){
    srand(std::time(0));
    return (rand()%45+75)/1000.0;
}

double phase(){
    srand(std::time(0));
    return (rand()%314)/100.0;
}

double amplitude(){
    srand(std::time(0));
    return (rand()%8+30)/10.0;
}
