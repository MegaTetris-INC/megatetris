#ifndef _IO_H_
#define _IO_H_

#include <MCUFRIEND_kbv.h>

#define SCREEN_HEIGHT 319
#define SCREEN_WIDTH 239

#define LINE_SPACE 10

enum Keys {UP, DOWN, LEFT, RIGHT, OK, NO_PRESS};

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

	void DrawRectangle (int pX1, int pY1, int pX2, int pY2, uint16_t pC)
    {
        tft.fillRect(pX1, pY1, pX2-pX1, pY2-pY1, pC);
    }

    void FillBG (uint16_t pC)
    {
        tft.fillScreen(pC);
    }

    void WriteText (String pText, int textSize, uint16_t color, int x, int y)
    {
        int16_t x1, y1;
        uint16_t w = 0, h = 0;

        tft.setTextSize (textSize);
        tft.setTextColor (color);
        
        int lineStart = 0;
        int lineBreak = pText.indexOf('\n');
        String currentLine;

        if (lineBreak == -1)
        {
            int16_t x1, y1;
            uint16_t w = 0, h = 0;
            tft.getTextBounds (pText, 0, 0, &x1, &y1, &w, &h);
            tft.setCursor (x - (w/2), y - (h/2));
            tft.print (pText);
        }
        else
        {
            while (lineBreak != -1)
            {
                currentLine = pText.substring(lineStart, lineBreak);

                tft.getTextBounds (currentLine, 0, 0, &x1, &y1, &w, &h);
                tft.setCursor (x - (w/2), y - (h/2));
                tft.print (currentLine);
                y += (h/2 + LINE_SPACE);
                lineStart = lineBreak + 1;
                lineBreak = pText.indexOf('\n', lineStart);
            }
            currentLine = pText.substring(lineStart);
            tft.getTextBounds (currentLine, 0, 0, &x1, &y1, &w, &h);
            tft.setCursor (x - (w/2), y - (h/2));
            tft.print (currentLine);
        }

        tft.setCursor (0,0);
    }

	Keys Getkey (int keyPressed)
    {
        if (0 <= keyPressed && keyPressed <= 10) return LEFT;
        if (160 <= keyPressed && keyPressed <= 170) return RIGHT;
        if (25 <= keyPressed && keyPressed <= 35) return UP;
        if (80 <= keyPressed && keyPressed <= 90) return DOWN;
        if (350 <= keyPressed && keyPressed <= 360) return OK;
        return NO_PRESS;
    }
};

#endif
