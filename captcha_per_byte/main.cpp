#include <ctime>

#include "alphabet.h"
#include "bmp.h"
#include "gentext.h"
#include "wave.h"
#include "dwordtobyte.h"
#include "createbitmap.h"

using namespace std;

int main(int argc, char* argv[]){
	Alphabet alphabet("abcd","font");
	
	int lenOfWord =4;
	
	int height = alphabet._heightOfLetter;
	int width = alphabet._lenOfLetter*lenOfWord;	
	int length;
	bmp _bmp(width,height);
	
	dword bgc = 0x00000000;
	
	cout<<"all:\n";
	int start = clock();
	for(int i=0;i<1000;++i){
		dword* data = genText(lenOfWord,&alphabet,&length);		
		data = wave(data,128,64,bgc);
		byte* byteData = dwordToByte(data,length);
		createBitmap(byteData,"wave.bmp",&_bmp);//TODO
	}
	int total = clock()-start;
	double avg = total/1000.0;
	cout<<"total\n:"<<total;

	cout<<"without save:\n";
	start = clock();
	for(int i=0;i<1000;++i){
		dword* data = genText(lenOfWord,&alphabet,&length);		
		data = wave(data,128,64,bgc);
		byte* byteData = dwordToByte(data,length);
		//createBitmap(byteData,"wave.bmp",&_bmp);//TODO
	}
	total = clock()-start;
	avg = total/1000.0;
	cout<<"total:\n"<<total;
	
	cout<<"without wave:\n";
	start = clock();
	for(int i=0;i<1000;++i){
		dword* data = genText(lenOfWord,&alphabet,&length);		
		//data = wave(data,128,64,bgc);
		//byte* byteData = dwordToByte(data,length);
		//createBitmap(byteData,"wave.bmp",&_bmp);//TODO
	}
	total = clock()-start;
	avg = total/1000.0;
	cout<<"total:\n"<<total;
	
	
	char somevar;
	cin>>somevar;
	
	return 0;
}
