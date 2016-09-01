#include "defines.h"

#include <iostream>
#include <windows.h>
#include <conio.h>

#ifndef DWORDTOBYTE_H_23082016
#define DWORDTOBYTE_H_23082016

unsigned char* dwordToByte(dword dwordData[],int length){
	int dwordDataSize = length;	
	int byteDataSize = dwordDataSize*3;
	
	unsigned char* byteData = new unsigned char[byteDataSize];
	
	for(register int i=0;i<dwordDataSize;++i){
		int i3=i*3;
		byteData[i3]  =(unsigned char)((dwordData[i]>>24)&0xff);
		byteData[i3+1]=(unsigned char)((dwordData[i]>>16)&0xff);
		byteData[i3+2]=(unsigned char)((dwordData[i]>>8) &0xff);
	}
	
	return byteData;
}

#endif
