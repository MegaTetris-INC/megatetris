#ifndef _SCREEN_H_
#define _SCREEN_H_

//include libraries
#include <MCUFRIEND_kbv.h>

class screen
{
private:
    MCUFRIEND_kbv tft;

public:
    //initialize tft screen
    void init();
};

#endif