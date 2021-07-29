/*
Connect4 for unix
ethanberg95@gmail.com
*/

#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#include "c4board.h"
struct termios stdterm;		// Store standard terminal flags
int gameRun = 1;		// always running until game ends

// call this function at program exit
void
resetTerm (int fd)
{
  tcsetattr (fd, TCSAFLUSH, &stdterm);
}

// problem with atexit, i had to do this
void
disableRawTerm ()
{
  resetTerm (STDIN_FILENO);
}

// set up terminal for proper reading of input
// so user doesnt have to press enter after everything
void
rawTerm (int fd)
{
  tcgetattr (STDIN_FILENO, &stdterm);
  atexit (disableRawTerm);

  struct termios newterm = stdterm;	// new terminal struct for modification
  newterm.c_lflag &= ~(ECHO | ICANON | ISIG);

  tcsetattr (fd, TCSAFLUSH, &newterm);	// apply new term settings
}

// check if win condition has been met
void
checkWin (int board[56], int tile)
{
  if (board[tile] ==
      board[tile + 8] /*&& board[tile+16] && board[tile+24] */ )
    {
      gameRun = 0;
    }
}

// check if there is room in a clumn and place piece on tile
void
checkCol (int board[56], int column)
{

  // check every index in the 7x6 that is in the same column as currentBlock
  for (int i = column; i < 56; i = i + 8)
    {
      if (board[i] == 4 && i - 8 > 7)
	{			// checks if the current index is colliding
	  board[i - 8] = 4;
	  checkWin (board, board[i - 8]);
	}
      else if (i + 8 >= 56 && i - 8 > 7)
	{			// seperated into 2 "if's"  
	  board[i] = 4;
	  checkWin (board, board[i]);
	}
    }
}

int
main ()
{
  rawTerm (STDIN_FILENO);
  int start = 6;		// start at index 6 in gameBoard
  int currentBlock = start;	// store current point to manipulate
  gameboard[start] = 3;		// init the first piece indicator

  char *message;
  char c;
  while (gameRun == 1)
    {
      system ("clear");
      // parse the gameboard and print 
      for (int i = 0; i < 56; i++)
	{
	  switch (gameboard[i])
	    {
	    case 0:
	      printf ("_");
	      break;
	    case 1:
	      printf ("\n");
	      break;
	    case 2:
	      printf (" ");
	      break;
	    case 3:
	      printf ("+");
	      break;
	    case 4:
	      printf ("O");
	      break;
	    }
	}
      printf ("%s\n", message);
      read (STDIN_FILENO, &c, 1);	// read input

      switch (c)
	{
	case 'q':		// exit for debugging
	  printf ("Quitting\n");
	  gameRun = 0;
	  break;
	case 67:		// right arrow input
	  // increment the indicator one tile to the right
	  if (gameboard[currentBlock + 1] != 1)
	    {
	      gameboard[currentBlock] = 2;
	      currentBlock++;
	      gameboard[currentBlock] = 3;
	    }

	  //printf("key right\n");
	  break;
	case 68:		// left arrow input moves the indicator left one tile
	  if (currentBlock - 1 != -1)
	    {
	      gameboard[currentBlock] = 2;
	      currentBlock--;
	      gameboard[currentBlock] = 3;
	    }

	  //printf("key left\n");
	  break;
	case 10:		// enter/return
	  checkCol (gameboard, currentBlock);
	}
    }
}
