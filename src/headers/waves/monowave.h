#include "../randoms.h"
#include "cmath"

#ifndef MONOWAVE_H_25082016
#define MONOWAVE_H_25082016

unsigned char* monowave(unsigned char* imageData, int width, int height, unsigned char bgc){
 	unsigned char* newData = new unsigned char[width*height*3];
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
            int color_diff = bgc - imageData[x+y*width];
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
        return newData;
}

unsigned char* go3(unsigned char* imageData, int width, int height){
	unsigned char* newData = new unsigned char[width*height*3];
	for(int x=0;x<width;++x){
		for(int y=0;y<height;++y){
			int pos=x+y*width;
			newData[3*pos]=imageData[pos];
			newData[3*pos+1]=imageData[pos];
			newData[3*pos+2]=imageData[pos];
		}
	}
	return newData;
}

#endif
