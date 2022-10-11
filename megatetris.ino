#include "Game.h"
#include "pitches.h"
//#include "Tune.h"

//pins
int KeyboardPin = A5;
int RandomSeedPin = A4;
int BuzzerSongPin = 13;

//music variables
unsigned long previousMillis = 0;
unsigned long interval = 500;
int thisNote = -1;


// // sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// // there are two values per note (pitch and duration), so for each note there are four bytes
// int notes=sizeof(melody)/sizeof(melody[0])/2; 
// // this calculates the duration of a whole note in ms (60s/tempo)*4 beats
// int wholenote = (60000 * 4) / tempo;
// int divider = 0, noteDuration = 0;
// int thisNote = 0;


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
unsigned long tuneTimer = 0;

#define WAIT_TIME 700
#define KEY_PRESS_TIME 100

//set and initialize objects
void setup ()
{
   Serial.begin(9600);
//    //music setup
//      // Timer 1
//     noInterrupts();
//     TCCR1A = 0;
//     TCCR1B = 0;
  
//     TCNT1 = 48000; 
//     TCCR1B |= (1 << CS12);
//     TIMSK1 |= (1 << TOIE1);
//     interrupts();    

    randomSeed (analogRead(RandomSeedPin));
    mIO.InitScreen();
    NewGame ();
}

// ISR(TIMER1_OVF_vect)        
// {
//   TCNT1 = 48000;            
//   thisNote += 1;
//   if(thisNote==64) thisNote=0;
//   //iterate over the notes of the melody:
//     /*
//       to calculate the note duration, take one second divided by the note type.
//       e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
//     */
//     int noteDuration = 1000/noteDurations[thisNote];
//     tone(BuzzerSongPin, melody[thisNote],noteDuration);
//     /*
//       to distinguish the notes, set a minimum time between them.
//       the note's duration + 30% seems to work well:
//      */
//     int pauseBetweenNotes = noteDuration * 1.30;
//     //tone(A5, 0,pauseBetweenNotes);
//     delay(pauseBetweenNotes);
   
//     //noTone(A5); //stop the tone playing:
// //  }
// //  digitalWrite(A5,LOW);
// }


//game loop
void loop ()
{
    int keyRead = analogRead(KeyboardPin);
    Keys mKey; //= mIO.Getkey(keyRead);

    //PlayTune ();

    // isKeyPressed = (!(mKey == NO_PRESS));

    // if (isKeyPressed)
    // {
    //     Serial.println(keyRead);
    //     if ((millis() - keyTimer) < KEY_PRESS_TIME)
    //     {
    //         mKey = NO_PRESS;
    //     }
    //     else
    //     {
    //         keyTimer = millis();
    //     }
    // }

    // if ((mKey != NO_PRESS) && !isKeyPressed)
    // {
    //     isKeyPressed = true;
    //     keyTimer = millis();
    // }

    // if (isKeyPressed && (millis() - keyTimer < KEY_PRESS_TIME))
    // {
    //   Serial.println(mKey);
    //     mKey = NO_PRESS;
    // }
    // else
    // {
    //     isKeyPressed = false;
    // }

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

            //hard drop
            // case (OK):
            // {
            //     // Check collision from up to down
            //     while (mainBoard.IsPossibleMovement(mGame.mPosX, mGame.mPosY, mGame.mPiece, mGame.mRotation)) { mGame.mPosY++; }

            //     mainBoard.StorePiece (mGame.mPosX, mGame.mPosY - 1, mGame.mPiece, mGame.mRotation);

            //     mainBoard.DeletePossibleLines ();

            //     if (mainBoard.IsGameOver())
            //     {
            //         isGameOver = true;
            //         GameOverScreen ();
            //     }
            //     else
            //     {
            //         mGame.CreateNewPiece();
            //     }
            //     break;
            // }

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
    mIO.FillBG (TFT_BLACK);
    mIO.WriteText ("Game Over", 4, TFT_WHITE, (SCREEN_WIDTH+1)/2, (SCREEN_HEIGHT+1)/4);
    mIO.WriteText ("Score: " + String(score), 3, TFT_WHITE, (SCREEN_WIDTH+1)/2, (SCREEN_HEIGHT+1)/2);
    mIO.WriteText ("Press OK to start\na new game", 2, TFT_WHITE, (SCREEN_WIDTH+1)/2, 3*(SCREEN_HEIGHT+1)/4);
}

void NewGame ()
{
    score = 0;
    mIO.FillBG (TFT_WHITE);
    mainBoard.SetBoard(&mainPieces, SCREEN_HEIGHT);
    mGame.SetGame(&mainBoard, &mainPieces, &mIO, SCREEN_HEIGHT);
    mGame.DrawScene();
}

// void PlayTune ()
// {
//     // iterate over the notes of the melody. 
//     // Remember, the array is twice the number of notes (notes + durations)
//     //for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

//     if (millis () - tuneTimer >= noteDuration)
//     {
//         // calculates the duration of each note
//         divider = melody[thisNote + 1];
//         if (divider > 0)
//         {
//             // regular note, just proceed
//             noteDuration = (wholenote) / divider;
//         } 
//         else if (divider < 0) 
//         {
//             // dotted notes are represented with negative durations!!
//             noteDuration = (wholenote) / abs(divider);
//             noteDuration *= 1.5; // increases the duration in half for dotted notes
//         }

//         // we only play the note for 90% of the duration, leaving 10% as a pause
//         tone(BuzzerSongPin, melody[thisNote], noteDuration*0.9);

        
//         // Wait for the specief duration before playing the next note.
//         //delay(noteDuration);

//         // stop the waveform generation before the next note.
//         noTone(BuzzerSongPin);

//         thisNote += 2;
//         tuneTimer = 0;
//     }
//     //}
// }
