#include "Game.h"

//make it into a macro later plz
int mScreenHeight = SCREEN_HEIGHT;

IO mIO;
Pieces mainPieces;
Board mainBoard;
Game mGame;
//unsigned long mTime1 = SDL_GetTicks();
//unsigned long mTime1 = millis();

//set and initialize objects
void setup ()
{
    randomSeed (analogRead(A4));
    mIO.InitScreen();
    mainBoard.SetBoard(&mainPieces, mScreenHeight);
    mGame.SetGame(&mainBoard, &mainPieces, &mIO, mScreenHeight);
}

//game loop
void loop ()
{
	//mIO.ClearScreen (); 		// Clear screen
	//mIO.UpdateScreen ();		// Put the graphic context in the screen

    //mIO.UpdateScreen();
    mGame.DrawScene ();

    /*insert input code here*/

    //unsigned long mTime2 = SDL_GetTicks();
    
    //unsigned long mTime2 = millis();

    delay (WAIT_TIME);

    //if ((mTime2 - mTime1) > WAIT_TIME)
    //{
        if (mainBoard.IsPossibleMovement (mGame.mPosX, mGame.mPosY + 1, mGame.mPiece, mGame.mRotation))
        {
            mGame.mPosY++;
        }
        else
        {
            mainBoard.StorePiece (mGame.mPosX, mGame.mPosY, mGame.mPiece, mGame.mRotation);

            mainBoard.DeletePossibleLines ();

            if (mainBoard.IsGameOver())
            {
                //mIO.Getkey();
                //exit(0);

                //provisory game over screen, will have to reset arduino manually
                while(true)
                {
                    mIO.FillBG(TFT_BLUE);
                }
            }

            mGame.CreateNewPiece();
        }

        //mTime1 = SDL_GetTicks();
        //mTime1 = millis();
    //}
}
