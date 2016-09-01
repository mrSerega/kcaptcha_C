#include <iostream>
#include <fstream>

#include "readbmp.h"
#include "parse.h"
#include "createbitmap.h"
#include "bmp.h"

using namespace std;

int main(){
	int letter_width=0;
	int letter_height=0;
	ifstream file_info;
	file_info.open("info.txt");
	file_info>>letter_width;
	file_info>>letter_height;
	
	cout<<letter_width<<" "<<letter_height<<endl;
	 
	int width=0;
	int height=0;
	unsigned char* data = readBMP("test.bmp",&width,&height);
	
	cout<<width<<" "<<height<<endl;
	
	unsigned char** parsed_data = parse(data,letter_width,letter_height,width,height);
	
	Bmp bmp(32,32);
	
	string arr[] = {"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30","31","32"};
	
	cout<<"ok\n";
	for(int i=0;i<32;i++){
		string name = arr[i]+".bmp";
		createBitmap(parsed_data[i],name.data(),&bmp);
	}
	
	return 0;
}
