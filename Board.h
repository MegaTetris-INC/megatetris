#ifndef _BOARD_H_
#define _BOARD_H_

#include "Pieces.h"

/*ajustar conforme necessidade*/
#define BOARD_LINE_WIDTH 15			// Width of each of the two lines that delimit the board
#define BLOCK_SIZE 20				// Width and Height of each block of a piece
#define BOARD_POSITION 120			// Center position of the board from the left of the screen
#define BOARD_WIDTH 11				// Board width in blocks 
#define BOARD_HEIGHT 16				// Board height in blocks
#define PIECE_BLOCKS 5				// Number of horizontal and vertical blocks of a matrix piece

class Board
{
public:
    void SetBoard(Pieces *pPieces, int pScreenHeight)
    {
        mScreenHeight = pScreenHeight;
        mPieces = pPieces;
        InitBoard();
    }

    //block position in pixels
	int GetXPosInPixels (int pPos)
    {
	    return  ( ( BOARD_POSITION - (BLOCK_SIZE * (BOARD_WIDTH / 2)) ) + (pPos * BLOCK_SIZE) );
    }
	int GetYPosInPixels (int pPos)
    {
        return ( (mScreenHeight - (BLOCK_SIZE * BOARD_HEIGHT)) + (pPos * BLOCK_SIZE) );
    }

	bool IsFreeBlock (int pX, int pY)
    {
        return (mBoard [pX][pY] == POS_FREE);
    }
	bool IsPossibleMovement		(int pX, int pY, int pPiece, int pRotation)
    {
        //checks collision
        for (int i1 = pX, i2 = 0; i1 < pX + PIECE_BLOCKS; i1++, i2++)
        {
            for (int j1 = pY, j2 = 0; j1 < pY + PIECE_BLOCKS; j1++, j2++)
            {	
                //checks board limits
                if (i1 < 0 || i1 > BOARD_WIDTH  - 1	|| j1 > BOARD_HEIGHT - 1)
                {
                    if (mPieces->GetBlockType (pPiece, pRotation, j2, i2) != 0)
                    {
                        return false;
                    }
                }

                //checks collision against other blocks
                if (j1 >= 0)	
                {
                    if ((mPieces->GetBlockType (pPiece, pRotation, j2, i2) != 0) &&
                        (!IsFreeBlock (i1, j1))	)
                        {
                            return false;
                        }
                }
            }
        }
        return true;
    }

    //sets blocks where there are pieces with POS_FILLED
	void StorePiece (int pX, int pY, int pPiece, int pRotation)
    {
        for (int i1 = pX, i2 = 0; i1 < pX + PIECE_BLOCKS; i1++, i2++)
        {
            for (int j1 = pY, j2 = 0; j1 < pY + PIECE_BLOCKS; j1++, j2++)
            {	
                if (mPieces->GetBlockType (pPiece, pRotation, j2, i2) != 0)	
                    mBoard[i1][j1] = POS_FILLED;	
            }
        }
    }
    //deletes lines that should be deleted
	void DeletePossibleLines ()
    {
        for (int j = 0; j < BOARD_HEIGHT; j++)
        {
            //checks if all horizontal blocks from line j are filled
            int i = 0;
            while (i < BOARD_WIDTH)
            {
                if (mBoard[i][j] != POS_FILLED) break;
                i++;
            }
            //if the line is full, delete it
            if (i == BOARD_WIDTH) DeleteLine (j);
        }
    }
	bool IsGameOver ()
    {
        for (int i = 0; i < BOARD_WIDTH; i++)
        {
            if (mBoard[i][0] == POS_FILLED) return true;
        }

        return false;
    }

private:

	enum { POS_FREE, POS_FILLED };			// POS_FREE = free position of the board; POS_FILLED = filled position of the board
	int mBoard [BOARD_WIDTH][BOARD_HEIGHT];	// Board that contains the pieces
	Pieces *mPieces;
	int mScreenHeight;

	void InitBoard()
    {
        for (int i = 0; i < BOARD_WIDTH; i++)
        {
            for (int j = 0; j < BOARD_HEIGHT; j++)
            {
                mBoard[i][j] = POS_FREE;
            }
        }
    }

    //moves all upper blocks from pY line up one block down
	void DeleteLine (int pY)
    {
        for (int j = pY; j > 0; j--)
        {
            for (int i = 0; i < BOARD_WIDTH; i++)
            {
                mBoard[i][j] = mBoard[i][j-1];
            }
        }	
    }
};

#endif
