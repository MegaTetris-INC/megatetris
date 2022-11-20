#ifndef _IO_H_
#define _IO_H_

#include <MCUFRIEND_kbv.h>

#define SCREEN_HEIGHT 319
#define SCREEN_WIDTH 239

#define LINE_SPACE 20

enum Keys {UP, DOWN, LEFT, RIGHT, OK, NO_PRESS};

class IO
{
private:

    MCUFRIEND_kbv tft;

public:
    void InitScreen()
    {
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
            h = 0;
            w = 0;
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

    void WriteGameTitle ()
    {
        int16_t x1, y1;
        uint16_t w = 0, h = 0, x = 0;
        tft.setTextSize (4);

        tft.setTextColor (TFT_WHITE);
        tft.getTextBounds ("Mega", 0, 0, &x1, &y1, &w, &h);        
        tft.setCursor (0, ((SCREEN_HEIGHT+1)/3) - (h/2));
        tft.print ("Mega");
        x += w;
        tft.setCursor (x, ((SCREEN_HEIGHT+1)/3) - (h/2));

        tft.setTextColor (TFT_RED);
        tft.getTextBounds ("T", 0, 0, &x1, &y1, &w, &h);
        tft.print ("T");
        x += w;
        tft.setCursor (x, ((SCREEN_HEIGHT+1)/3) - (h/2));

        tft.setTextColor (TFT_OLIVE);
        tft.getTextBounds ("E", 0, 0, &x1, &y1, &w, &h);
        tft.print ("E");
        x += w;
        tft.setCursor (x, ((SCREEN_HEIGHT+1)/3) - (h/2));
        
        tft.setTextColor (TFT_YELLOW);
        tft.getTextBounds ("T", 0, 0, &x1, &y1, &w, &h);
        tft.print ("T");
        x += w;
        tft.setCursor (x, ((SCREEN_HEIGHT+1)/3) - (h/2));

        tft.setTextColor (TFT_GREEN);
        tft.getTextBounds ("R", 0, 0, &x1, &y1, &w, &h);
        tft.print ("R");
        x += w;
        tft.setCursor (x, ((SCREEN_HEIGHT+1)/3) - (h/2));
        
        tft.setTextColor (TFT_BLUE);
        tft.getTextBounds ("I", 0, 0, &x1, &y1, &w, &h);
        tft.print ("I");
        x += w;
        tft.setCursor (x, ((SCREEN_HEIGHT+1)/3) - (h/2));

        tft.setTextColor (TFT_PURPLE);
        tft.getTextBounds ("S", 0, 0, &x1, &y1, &w, &h);
        tft.print ("S");
        x += w;
        tft.setCursor (x, ((SCREEN_HEIGHT+1)/3) - (h/2));
    }


	  Keys Getkey (int keyPressed)
    {
        if (0 <= keyPressed && keyPressed <= 10) return LEFT;
        if (160 <= keyPressed && keyPressed <= 170) return RIGHT;
        if (25 <= keyPressed && keyPressed <= 35) return UP;
        if (80 <= keyPressed && keyPressed <= 90) return DOWN;
        if (340 <= keyPressed && keyPressed <= 360) return OK;
        return NO_PRESS;
    }
};

#endif
