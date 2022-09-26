#include "Game.h"
#include "pitches.h"

//music variables
unsigned long previousMillis = 0;
unsigned long interval = 500;
int thisNote = -1;


//make it into a macro later plz
int mScreenHeight = SCREEN_HEIGHT;

IO mIO;
Pieces mainPieces;
Board mainBoard;
Game mGame;
//unsigned long mTime1 = SDL_GetTicks();
//unsigned long mTime1 = millis();

#define WAIT_TIME 400

//set and initialize objects
void setup ()
{
   //music setup
     // Timer 1
    noInterrupts();
    TCCR1A = 0;
    TCCR1B = 0;
  
    TCNT1 = 48000; 
    TCCR1B |= (1 << CS12);
    TIMSK1 |= (1 << TOIE1);
    interrupts();    

    randomSeed (analogRead(A4));
    mIO.InitScreen();
    mainBoard.SetBoard(&mainPieces, mScreenHeight);
    mGame.SetGame(&mainBoard, &mainPieces, &mIO, mScreenHeight);
}

ISR(TIMER1_OVF_vect)        
{
  TCNT1 = 48000;            
  thisNote += 1;
  if(thisNote==64) thisNote=0;
  //iterate over the notes of the melody:
    /*
      to calculate the note duration, take one second divided by the note type.
      e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    */
    int noteDuration = 1000/noteDurations[thisNote];
    tone(13, melody[thisNote],noteDuration);
    /*
      to distinguish the notes, set a minimum time between them.
      the note's duration + 30% seems to work well:
     */
    int pauseBetweenNotes = noteDuration * 1.30;
    //tone(A5, 0,pauseBetweenNotes);
    delay(pauseBetweenNotes);
   
    //noTone(A5); //stop the tone playing:
//  }
//  digitalWrite(A5,LOW);
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
