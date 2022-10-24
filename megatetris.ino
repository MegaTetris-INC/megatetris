#include "Game.h"
#include "pitches.h"

//pins
int KeyboardPin = A5;
int RandomSeedPin = A4;
int BuzzerSongPin = A4;

//music variables
int thisNote = -1;
int noteDuration = 0;
enum SongStates : byte {STOP, PLAY, PAUSE};
SongStates songstate = STOP;

IO mIO;
Pieces mainPieces;
Board mainBoard;
Game mGame;

bool isGameOver = false;
bool isKeyPressed = false;

int score = 0;

//timers
unsigned long waitTimer = 0;
unsigned long keyTimer = 0;

#define WAIT_TIME 700
#define KEY_PRESS_TIME 100

//set and initialize objects
void setup ()
{
   Serial.begin(9600);
   //music setup
     //Timer 1
    noInterrupts();
    TCCR1A = 0;
    TCCR1B = 0;
  
    TCNT1 = 48000; 
    TCCR1B |= (1 << CS12);
    TIMSK1 |= (1 << TOIE1);
    interrupts();    

    randomSeed (analogRead(RandomSeedPin));
    mIO.InitScreen();
    NewGame ();
}

ISR(TIMER1_OVF_vect)        
{
    switch (songstate)
    {
    case STOP:
    {
        thisNote = -1;
        break;
    }
    case PLAY:
    {
        TCNT1 = 48000;
        thisNote += 1;
        if(thisNote==64) thisNote=0;

        //iterate over the notes of the melody:
        int noteDuration = 1000/noteDurations[thisNote];
        tone(BuzzerSongPin, melody[thisNote], noteDuration);
        break;
    }
    case PAUSE:
        break;
    default:
        break;
    }
}


//game loop
void loop ()
{    
    int keyRead = analogRead(KeyboardPin);
    Keys mKey;

    Serial.println(keyRead);

    if (millis() - keyTimer >= KEY_PRESS_TIME)
    {   
        mKey = mIO.Getkey(keyRead);
        keyTimer = millis();
    }
    else
    {
        mKey = NO_PRESS;
    }

    if (!isGameOver)
    {
        //input
        switch (mKey)
        {
            //move right/left
            case (RIGHT): 
            {
                if (mainBoard.IsPossibleMovement (mGame.mPosX + 1, mGame.mPosY, mGame.mPiece, mGame.mRotation))
                    mGame.mPosX++;
                break;
            }

            case (LEFT): 
            {
                if (mainBoard.IsPossibleMovement (mGame.mPosX - 1, mGame.mPosY, mGame.mPiece, mGame.mRotation))
                    mGame.mPosX--;	
                break;
            }

            //soft drop
            case (DOWN):
            {
                if (mainBoard.IsPossibleMovement (mGame.mPosX, mGame.mPosY + 1, mGame.mPiece, mGame.mRotation))
                    mGame.mPosY++;	
                break;
            }

            //rotation
            case (UP):
            {
                if (mainBoard.IsPossibleMovement (mGame.mPosX, mGame.mPosY, mGame.mPiece, (mGame.mRotation + 1) % 4))
                    mGame.mRotation = (mGame.mRotation + 1) % 4;

                break;
            }
            default:
                break;
        }

        //vertical movement

        bool verticalMov = false;

        if (millis() - waitTimer >= WAIT_TIME)
        {
            verticalMov = true;
            if (mainBoard.IsPossibleMovement (mGame.mPosX, mGame.mPosY + 1, mGame.mPiece, mGame.mRotation))
            {
                mGame.mPosY++;
            }
            else
            {
                mainBoard.StorePiece (mGame.mPosX, mGame.mPosY, mGame.mPiece, mGame.mRotation);

                mainBoard.DeletePossibleLines (score);

                if (mainBoard.IsGameOver())
                {
                    isGameOver = true;
                    GameOverScreen ();
                }
                else
                {
                    mGame.CreateNewPiece();
                }
            }
            waitTimer = millis();
        }

        if (!isGameOver && ((mKey != NO_PRESS) || verticalMov))
        {
            mGame.DrawScene();
        }
    }
    else
    {
        if (mKey == OK)
        {
            isGameOver = false;
            NewGame ();
        }
    }
}

void GameOverScreen ()
{
    songstate = STOP;
    mIO.FillBG (TFT_BLACK);
    mIO.WriteText ("Game Over", 4, TFT_WHITE, (SCREEN_WIDTH+1)/2, (SCREEN_HEIGHT+1)/4);
    mIO.WriteText ("Score: " + String(score), 3, TFT_WHITE, (SCREEN_WIDTH+1)/2, (SCREEN_HEIGHT+1)/2);
    mIO.WriteText ("Press OK to start\na new game", 2, TFT_WHITE, (SCREEN_WIDTH+1)/2, 3*(SCREEN_HEIGHT+1)/4);
}

void NewGame ()
{
    score = 0;
    mIO.FillBG (TFT_WHITE);
    mainBoard.SetBoard(&mainPieces);
    mGame.SetGame(&mainBoard, &mainPieces, &mIO);
    mGame.DrawScene();
    songstate = PLAY;
}
