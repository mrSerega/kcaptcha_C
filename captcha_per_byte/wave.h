#include <cmath>

#include "randoms.h"

#include "defines.h"

#ifndef WAVE_H_23082016
#define WAVE_H_23082016

dword* wave(dword* data, int width, int height, dword bgc){
	dword* newData = new dword[width*height];
	//period
    register double dx_period_x = period();
    register double dx_period_y = period();
	register double dy_period_x = period();
    register double dy_period_y = period();
    //phase
    register double dx_phase_x = phase();
    register double dx_phase_y = phase();
	register double dy_phase_x = phase();
	register double dy_phase_y = phase();
    //amplitude
    register double dx_amplitude = amplitude();
    register double dy_amplitude = amplitude();
    //init
    //color diff
    for(register int x=0;x<width;x++){
        for(register int y=0;y<height;y++){
            dword color_diff = bgc - data[x+y*width];
            if(!color_diff){
                continue;
                newData[x+y*width]=bgc;
            }
            //source x (double)
            //int sinxstart = clock();
            double dx_x = sin(x * dx_period_x + dx_phase_x);
            double dx_y = sin(y * dx_period_y + dx_phase_y);
            //cout<<clock()-sinxstart;
            double sx = x + (dx_x + dx_y) * dx_amplitude;
            if(!((0<=sx) && (sx<width-1))){
                continue;
                 newData[x+y*width]=bgc;
            }
            //source y (double)
            //int sinystart = clock();
            double dy_x = sin(x * dy_period_x + dy_phase_x);
            double dy_y = sin(y * dy_period_y + dy_phase_y);
            //cout<<clock()-sinystart;
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
            dword tmp = (dword)(color_diff * (1-frx) * (1-fry)); 
            newData[idx1] -= tmp;
            
			tmp = (dword)(color_diff * frx * (1-fry));
            newData[(idx1+1)] -= tmp;
            
			tmp = (dword)(color_diff * (1-frx) * fry);
            newData[idx2] -= tmp;
            
			tmp = (dword)(color_diff * frx * fry);
            newData[(idx2+1)] -= tmp;
        }
    }
    return newData;
}

#endif
