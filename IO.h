#ifndef _IO_H_
#define _IO_H_

#include <MCUFRIEND_kbv.h>

#define SCREEN_HEIGHT 319
#define SCREEN_WIDTH 239

class IO
{
private:

    MCUFRIEND_kbv tft;

public:
    void InitScreen()
    {
        tft.reset();
        tft.begin(tft.readID());
        tft.setRotation(0);
    }

    //might need math adjustments
	void DrawRectangle (int pX1, int pY1, int pX2, int pY2, uint16_t pC)
    {
        tft.fillRect(pX1, pY1, pX2-pX1, pY2-pY1, pC);
    }

    void FillBG (uint16_t pC)
    {
        tft.fillScreen(pC);
    }
    //search for possible screen updating???? idk if tft needs it

    void UpdateScreen()
    {
      FillBG (TFT_BLACK);
    }

    //key inputs, implement later
	int Pollkey				();
	int Getkey				();
	int IsKeyDown			(int pKey);
};

#endif
