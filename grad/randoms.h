#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

#ifndef RANDOM_H_23082016
#define RANDOM_H_23082016

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

#endif
