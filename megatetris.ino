#include "Game.h"
#include "pitches.h"

//pins
int KeyboardPin = A5;
int RandomSeedPin = A4;
int BuzzerSongPin = A4;

//music
int thisNote = -1;

//objects
IO mIO;
Pieces mainPieces;
Board mainBoard;
Game mGame;

//game state
enum GameStates : byte {MENU, GAME, PAUSE, GAME_OVER};
GameStates gamestate = MENU;
byte menuOption = 0;

//key reading
int keyRead = -1;
Keys mKey = NO_PRESS;

//game variables
int score = 0;
int speedTime = 700;

//timers
unsigned long waitTimer = 0;
unsigned long keyTimer = 0;

#define WAIT_TIME speedTime
#define KEY_PRESS_TIME 150
#define BEEP_TIME 50
#define BEEP 800

//set and initialize objects
void setup ()
{
   //music setup
    noInterrupts();
    TCCR1A = 0;
    TCCR1B = 0;
  
    TCNT1 = 48000; 
    TCCR1B |= (1 << CS12);
    TIMSK1 |= (1 << TOIE1);
    interrupts();    

    randomSeed (analogRead(RandomSeedPin));
    mIO.InitScreen();
    MenuScreen ();
    //NewGame ();
}

ISR(TIMER1_OVF_vect)        
{
    switch (gamestate)
    {
    case MENU:
    {
        thisNote = -1;
        break;
    }
    case GAME_OVER:
    {
        thisNote = -1;
        break;
    }
    case GAME:
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
    ReadKey ();
    
    if (mKey != NO_PRESS && gamestate != GAME)
    {
        Beep ();
    }
    
    switch (gamestate)
    {
    case MENU:
    {
        if (mKey == OK)
        {
            gamestate = GAME;
            NewGame ();
        }
    }
    break;
    case GAME:
    {
        if (mKey == OK)
        {
            gamestate = PAUSE;
            menuOption = 1;
            PauseScreen ();
        }
        else
        {
            GameScreen ();
        }
    }
    break;
    case PAUSE:
    {
        if ((mKey == UP) && (menuOption > 1))
        {
            menuOption = 1;
            PauseScreen ();
        }
        else if ((mKey == DOWN) && (menuOption < 2))
        {
            menuOption = 2;
            PauseScreen ();
        }
        else if (mKey == OK)
        {
            if (menuOption == 1)
            {
                gamestate = GAME;
                mIO.FillBG (TFT_WHITE);
            }
            else if (menuOption == 2)
            {
                gamestate = MENU;
                MenuScreen ();
            }
        }
    }
    break;
    case GAME_OVER:
    {
        if ((mKey == UP) && (menuOption > 1))
        {
            menuOption = 1;
            GameOverScreen ();
        }
        else if ((mKey == DOWN) && (menuOption < 2))
        {
            menuOption = 2;
            GameOverScreen ();
        }
        else if (mKey == OK)
        {
            if (menuOption == 1)
            {
                NewGame ();
            }
            else if (menuOption == 2)
            {
                gamestate = MENU;
                MenuScreen ();
            }
        }
    }
    break;
    default:
    break;
    }
}

void ReadKey ()
{
    keyRead = analogRead(KeyboardPin);

    if (millis() - keyTimer >= KEY_PRESS_TIME)
    {   
        mKey = mIO.Getkey(keyRead);
        keyTimer = millis();
    }
    else
    {
        mKey = NO_PRESS;
    }
}

void MenuScreen ()
{
    mIO.FillBG (TFT_BLACK);
    mIO.WriteGameTitle ();
    mIO.WriteText ("Press OK to\nstart a\nnew game", 3, TFT_WHITE, (SCREEN_WIDTH+1)/2, 2*(SCREEN_HEIGHT+1)/3);    
}

void GameScreen ()
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

            mainBoard.DeletePossibleLines (score, speedTime);

            if (mainBoard.IsGameOver())
            {
                gamestate = GAME_OVER;
                menuOption = 1;
                //isGameOver = true;
                GameOverScreen ();
            }
            else
            {
                mGame.CreateNewPiece();
            }
        }
        waitTimer = millis();
    }

    if ((gamestate != GAME_OVER) && ((mKey != NO_PRESS) || verticalMov))
    {
        mGame.DrawScene();
    }
}

void PauseScreen ()
{
    mIO.FillBG (TFT_BLACK);
    mIO.WriteText ("Pause", 4, TFT_WHITE, (SCREEN_WIDTH+1)/2, (SCREEN_HEIGHT+1)/5);
    mIO.WriteText ("Current score: " + String(score), 2, TFT_WHITE, (SCREEN_WIDTH+1)/2, 2*(SCREEN_HEIGHT+1)/5);
    if (menuOption == 1)
    {
        mIO.WriteText ("Resume game", 2, TFT_YELLOW, (SCREEN_WIDTH+1)/2, 3*(SCREEN_HEIGHT+1)/5);
        mIO.WriteText ("Return to menu", 2, TFT_WHITE, (SCREEN_WIDTH+1)/2, 4*(SCREEN_HEIGHT+1)/5);
    }
    else if (menuOption == 2)
    {
        mIO.WriteText ("Resume game", 2, TFT_WHITE, (SCREEN_WIDTH+1)/2, 3*(SCREEN_HEIGHT+1)/5);
        mIO.WriteText ("Return to menu", 2, TFT_YELLOW, (SCREEN_WIDTH+1)/2, 4*(SCREEN_HEIGHT+1)/5);
    }
}

void GameOverScreen ()
{
    //songstate = STOP;
    mIO.FillBG (TFT_BLACK);
    mIO.WriteText ("Game Over", 4, TFT_WHITE, (SCREEN_WIDTH+1)/2, (SCREEN_HEIGHT+1)/5);
    mIO.WriteText ("Score: " + String(score), 3, TFT_WHITE, (SCREEN_WIDTH+1)/2, 2*(SCREEN_HEIGHT+1)/5);
    if (menuOption == 1)
    {
        mIO.WriteText ("Start a new game", 2, TFT_YELLOW, (SCREEN_WIDTH+1)/2, 3*(SCREEN_HEIGHT+1)/5);
        mIO.WriteText ("Return to menu", 2, TFT_WHITE, (SCREEN_WIDTH+1)/2, 4*(SCREEN_HEIGHT+1)/5);
    }
    else if (menuOption == 2)
    {
        mIO.WriteText ("Start a new game", 2, TFT_WHITE, (SCREEN_WIDTH+1)/2, 3*(SCREEN_HEIGHT+1)/5);
        mIO.WriteText ("Return to menu", 2, TFT_YELLOW, (SCREEN_WIDTH+1)/2, 4*(SCREEN_HEIGHT+1)/5);
    }
}

void NewGame ()
{
    gamestate = GAME;
    score = 0;
    mIO.FillBG (TFT_WHITE);
    mainBoard.SetBoard(&mainPieces);
    mGame.SetGame(&mainBoard, &mainPieces, &mIO);
    mGame.DrawScene();
}

void Beep ()
{
    tone (BuzzerSongPin, BEEP, BEEP_TIME);
}
