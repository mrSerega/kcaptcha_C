#include <ctime>
#include <string>

#include "alphabet.h"
#include "bmp.h"
#include "gentext.h"
#include "wave.h"
#include "dwordtobyte.h"
#include "createbitmap.h"

using namespace std;

int main(int argc, char* argv[]){
	
	
	
	Alphabet alphabet("abcdefghijklmnopqrstuvwxyz","font");
	
	string genWord = "abcd";
	int lenOfWord = strlen(genWord.data());
	
	int height = alphabet._heightOfLetter;
	int width = alphabet._lenOfLetter*lenOfWord;	
	int length;
	bmp _bmp(width,height);
	
	dword bgc = 0xffffffff;
	
	dword* data = genText(lenOfWord,&alphabet,&length,genWord);		
	data = wave(data,width,height,bgc);
	byte* byteData = dwordToByte(data,length);
	createBitmap(byteData,"wave.bmp",&_bmp);//TODO
	
	//char somevar;
	//cin>>somevar;
	
	return 0;
}
