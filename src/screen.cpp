#include "../header/screen.h"

void init ()
{
    tft.reset();
    tft.begin(tft.readID());
    tft.setRotation(0);
}