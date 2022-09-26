#ifndef _GAME_H_
#define _GAME_H_

#include "IO.h"
#include "Pieces.h"
#include "Board.h"

class Game
{
public:

    void SetGame (Board *pBoard, Pieces *pPieces, IO *pIO, int pScreenHeight)
    {
        mScreenHeight = pScreenHeight;

        mBoard = pBoard;
        mPieces = pPieces;
        mIO = pIO;

        InitGame ();
    }

    //calls drawing methods for board and pieces
	void DrawScene ()
    {
        DrawBoard ();
        DrawPiece (mPosX, mPosY, mPiece, mRotation);
        DrawPiece (mNextPosX, mNextPosY, mNextPiece, mNextRotation);
    }

    //sets next piece as current piece and generates a new next piece
	void CreateNewPiece ()
    {
        mPiece = mNextPiece;
        mRotation = mNextRotation;
        mPosX = (BOARD_WIDTH / 2) + mPieces->GetXInitialPosition (mPiece, mRotation);
        mPosY = mPieces->GetYInitialPosition (mPiece, mRotation);

        mNextPiece = random (0, 7);
        mNextRotation = random (0, 4);
        // mNextPiece = GetRand (0, 6);
        // mNextRotation = GetRand (0, 3);
    }

    //info of the current piece (falling down)
	int mPosX, mPosY;
	int mPiece, mRotation;

private:

    //screen height in pixels
	int mScreenHeight;

    //info of the next piece
	int mNextPosX, mNextPosY;
	int mNextPiece, mNextRotation;

	Board *mBoard;
	Pieces *mPieces;
	IO *mIO;

    //random integers between two borders
	int GetRand (int pA, int pB)
    {
        return rand ()%(pB-pA+1)+pA;
    }

	void InitGame()
    {
        //srand ((unsigned int) time(NULL));

        //generates first piece
        // mPiece = GetRand (0, 6);
        // mRotation = GetRand (0, 3);

        mPiece = random (1, 7);
        mPiece = mPiece - random(0, 2);
        
        mRotation = random (0, 4);
        mPosX = (BOARD_WIDTH / 2) + mPieces->GetXInitialPosition (mPiece, mRotation);
        mPosY = mPieces->GetYInitialPosition (mPiece, mRotation);

        //generates next piece
        // mNextPiece = GetRand (0, 6);
        // mNextRotation = GetRand (0, 3);

        mNextPiece = random (0, 7);
        mNextRotation = random (0, 4);
        mNextPosX = BOARD_WIDTH + 5;
        mNextPosY = 5;	
    }

    //iterates through the piece matrix and draw each block
	void DrawPiece (int pX, int pY, int pPiece, int pRotation)
    {
        uint16_t mColor;

        int mPixelsX = mBoard->GetXPosInPixels (pX);
        int mPixelsY = mBoard->GetYPosInPixels (pY);

        for (int i = 0; i < PIECE_BLOCKS; i++)
        {
            for (int j = 0; j < PIECE_BLOCKS; j++)
            {
                switch (mPieces->GetBlockType (pPiece, pRotation, j, i))
                {
                    //case 0: mColor = TFT_BLACK; break;
                    case 1: mColor = TFT_GREEN; break;
                    case 2: mColor = TFT_BLUE; break;
                }
                
                if (mPieces->GetBlockType (pPiece, pRotation, j, i) != 0)
                    mIO->DrawRectangle	(mPixelsX + i * BLOCK_SIZE, 
                                        mPixelsY + j * BLOCK_SIZE, 
                                        (mPixelsX + i * BLOCK_SIZE) + BLOCK_SIZE - 1, 
                                        (mPixelsY + j * BLOCK_SIZE) + BLOCK_SIZE - 1, 
                                        mColor);
            }
        }
    }

	void DrawBoard ()
    {
        //board limits in pixels
        int mX1 = BOARD_POSITION - (BLOCK_SIZE * (BOARD_WIDTH / 2)) - 1;
        int mX2 = BOARD_POSITION + (BLOCK_SIZE * (BOARD_WIDTH / 2));
        int mY = mScreenHeight - (BLOCK_SIZE * BOARD_HEIGHT);
        
        // Check that the vertical margin is not to small
        //assert (mY > MIN_VERTICAL_MARGIN);

        //draw board limits
        mIO->DrawRectangle (mX1 - BOARD_LINE_WIDTH, mY, mX1, mScreenHeight - 1, TFT_BLUE);
        mIO->DrawRectangle (mX2, mY, mX2 + BOARD_LINE_WIDTH, mScreenHeight - 1, TFT_BLUE);
        
        // Check that the horizontal margin is not to small
        //assert (mX1 > MIN_HORIZONTAL_MARGIN);

        //draw blocks from the board
        mX1 += 1;
        for (int i = 0; i < BOARD_WIDTH-1; i++)
        {
            for (int j = 0; j < BOARD_HEIGHT; j++)
            {	
                // Check if the block is filled, if so, draw it
                if (mBoard->IsFreeBlock(i, j))	
                    mIO->DrawRectangle (	mX1 + i * BLOCK_SIZE, 
                                            mY + j * BLOCK_SIZE, 
                                            (mX1 + i * BLOCK_SIZE) + BLOCK_SIZE - 1, 
                                            (mY + j * BLOCK_SIZE) + BLOCK_SIZE - 1, 
                                            TFT_BLACK);
                 else
                     mIO->DrawRectangle (	mX1 + i * BLOCK_SIZE, 
                                             mY + j * BLOCK_SIZE, 
                                             (mX1 + i * BLOCK_SIZE) + BLOCK_SIZE - 1, 
                                             (mY + j * BLOCK_SIZE) + BLOCK_SIZE - 1, 
                                             TFT_RED);
            }
        }	
    }
};

#endif
