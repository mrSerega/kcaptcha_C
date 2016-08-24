#include <stdio.h>

#ifndef CASTOMSIN_H_24082016
#define CASTOMSIN_H_24082016

double _sin_asm(double x) 
{ 
  double y; 
  asm ("fsin" : "=t" (y) : "0" (x));
  return y;  
}

#endif
