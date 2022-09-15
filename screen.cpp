#include "screen.h"

void screen::init ()
{
    tft.reset();
    tft.begin(tft.readID());
    tft.setRotation(0);
}

void screen::draw_bg()
{
  tft.fillScreen(TFT_BLACK);  
}
