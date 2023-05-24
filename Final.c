// Kylee Smith and Eli Moralez
// 9 puzzle solver
// *Note* There are 9! = 362,880 different board configurations!!!
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

typedef unsigned char BYTE;                                  // sizeof(BYTE) = 1

typedef BYTE BOARD[4+1][4+1];
/*
   *Note* The order of the following MOVE enum-eration definition is essential to 
      both the for-statement "counting" found in FindASolution() and in the ordering 
      of constant  char-acter array MOVES[] found in DisplaySolution().
*/
typedef enum          { U,  D,  R,  L } MOVE;
const char MOVES[] = { 'U','D','R','L' };

#define MAXCOUNT 27 

//------------------------------------------------------------------
int main()
//------------------------------------------------------------------
{
   void SetAndDisplayInitialBoard(BOARD board,int *row0,int *col0, int one, int two,int three,int four,int five,int six,int seven,int eight,int nine);
   void FindAllSolutions(BOARD board,int row0,int col0,int count,BOARD boards[],MOVE moves[],int *solutions);
   void CopyBoard(BOARD LHS,BOARD RHS);
   
   BOARD board;
   int row0,col0;
   int solutions;
   int one, two, three, four, five, six, seven, eight, nine;

/*
  *Note* The locally-scoped variables moves[] and boards[] can be globally-defined 
     and then do not need to be passed to FindASolution(), thus making the
     FindASolution() activation record a little smaller (2 fewer pointers). Doing
     so, however, will require changes to the DisplaySolution() interface--one of the 
     modules called by FindASolution()--because it requires moves[].
*/
   MOVE *moves =   (MOVE * ) malloc(sizeof(MOVE )*(MAXCOUNT+1));
   BOARD *boards = (BOARD *) malloc(sizeof(BOARD)*(MAXCOUNT+1));

   assert( moves  != NULL );
   assert( boards != NULL );
   
   printf("Please enter the desired board configuration: options are 0, 1, 2, 3, 4, 5, 7, 8.\n");
   printf("Please enter a number for spot 1: ");
   scanf("%i", &one);
   printf("Please enter a number for spot 2: ");
   scanf("%i", &two);
   printf("Please enter a number for spot 3: ");
   scanf("%i", &three);
   printf("Please enter a number for spot 4: ");
   scanf("%i", &four);
   printf("Please enter a number for spot 5: ");
   scanf("%i", &five);
   printf("Please enter a number for spot 6: ");
   scanf("%i", &six);
   printf("Please enter a number for spot 7: ");
   scanf("%i", &seven);
   printf("Please enter a number for spot 8: ");
   scanf("%i", &eight);
   printf("Please enter a number for spot 9: ");
   scanf("%i", &nine);

   SetAndDisplayInitialBoard(board,&row0,&col0, one, two, three, four, five, six, seven, eight, nine);
/*
   See the boards[0] *Note* in FindASolution() for justification for the
      following CopyBoard() reference.
*/
   CopyBoard(boards[0],board);
   solutions = 0;
   FindAllSolutions(board,row0,col0,0,boards,moves,&solutions);

   free(moves); free(boards);

   system("PAUSE");
   return( 0 );
}

//------------------------------------------------------------------
void SetAndDisplayInitialBoard(BOARD board,int *row0,int *col0,int one,int two,int three,int four,int five, int six,int seven, int eight, int nine)
//------------------------------------------------------------------
{
	int row, col, i;
/*
    | 1  2  3
  -----------
  1 | 1  2  3
  2 | 4  5  6
  3 | 7  0  8
*/

   		board[1][1] =  one;
   		board[1][2] =  two;
   		board[1][3] =  three;
   		board[2][1] =  four;
   		board[2][2] =  five;
   		board[2][3] =  six;
   		board[3][1] =  seven;
   		board[3][2] =  eight;
   		board[3][3] =  nine;
   
   *row0 = 3;
   *col0 = 2; 

   printf("Initial board\n");
   printf("=============\n");
   for (row = 1; row <= 3; row++)
   {
      for (col = 1; col <= 3; col++)
         printf(" %2d",board[row][col]);
      printf("\n");
   }
   printf("\n");
}

//------------------------------------------------------------------
void FindAllSolutions(BOARD board,int row0,int col0,int count,BOARD boards[],MOVE moves[],int *solutions)
//------------------------------------------------------------------
{
   bool IsSolution(BOARD board);
   void Swap(BYTE *LHS,BYTE *RHS);
   void CopyBoard(BOARD LHS,BOARD RHS);
   bool BoardsAreEQ(BOARD LHS,BOARD RHS);
   bool IsFeasibleMove(BOARD nextBoard,int nextRow0,int nextCol0,
                       BOARD     board,int     row0,int     col0);
   int i;
   MOVE move;
 //  DisplayBoard(board,count,moves);                             // added to demonstrate progress (not essential)

   if ( IsSolution(board) )                                     // *BASE* case
   {
      *solutions += 1;
      printf("#%8d (%3d): ",*solutions,count);
      for ( i = 1; i <= count; i++)
         printf("%c",MOVES[moves[i]]);
      printf("\n");
   }
   else                                                         // *GENERAL* case
   {
      for (move = U; move <= L; move++)
      {
         BOARD nextBoard;
         int nextRow0,nextCol0;

         switch ( move )
         {
            case U: nextRow0 = row0 - 1; nextCol0 = col0    ;
                    break;
            case D: nextRow0 = row0 + 1; nextCol0 = col0    ;
                    break;
            case R: nextRow0 = row0    ; nextCol0 = col0 + 1;
                    break;
            case L: nextRow0 = row0    ; nextCol0 = col0 - 1;
                    break;
         }
         CopyBoard(nextBoard,board);
         if ( ((1 <= nextRow0) && (nextRow0 <= 3)) 
           && ((1 <= nextCol0) && (nextCol0 <= 3)))
         {
            Swap(&nextBoard[nextRow0][nextCol0],&nextBoard[row0][col0]);
         /*
            *Note* boards[0] is the initial board (technically not derived as the
               result of a move) that heads the list of boards[] so that any move sequence 
               that repeats the initial board can be avoided.
         */
            bool repeat = false;
            for (i = 0; i <= count; i++)
               repeat = repeat || BoardsAreEQ(nextBoard,boards[i]);

            if ( !repeat && IsFeasibleMove(nextBoard,nextRow0,nextCol0,board,row0,col0) && ((count+1) <= MAXCOUNT) )
            {
               CopyBoard(boards[count+1],nextBoard);
               moves[count+1] = move;
               FindAllSolutions(nextBoard,nextRow0,nextCol0,count+1,boards,moves,solutions);
            }
         }
      }
   }
}

//------------------------------------------------------------------
void Swap(BYTE *LHS,BYTE *RHS)
//------------------------------------------------------------------
{
   BYTE T = *LHS; *LHS = *RHS; *RHS = T;
}

//------------------------------------------------------------------
void CopyBoard(BOARD LHS,BOARD RHS)                             // LHS := RHS
//------------------------------------------------------------------
{
   int row, col;
   for ( row = 1; row <= 3; row++)
      for ( col = 1; col <= 3; col++)
         LHS[row][col] = RHS[row][col];
}

//------------------------------------------------------------------
bool IsSolution(BOARD board)
//------------------------------------------------------------------
{
   bool BoardsAreEQ(BOARD LHS,BOARD RHS);
/*
    | 1  2  3
  -----------
  1 | 1  2  3
  2 | 4  5  6
  3 | 7  8  0
*/
   static BOARD SOLUTION =
   {
      { 99,99,99,99 },
      { 99, 1, 2, 3 },
      { 99, 4, 5, 6 },
      { 99, 7, 8, 0 }
   };

   return( BoardsAreEQ(SOLUTION,board) );
}

//------------------------------------------------------------------
bool BoardsAreEQ(BOARD LHS,BOARD RHS)
//------------------------------------------------------------------
{
   bool r = true;
   int row, col;
   
   for ( row = 1; row <= 3; row++)
      for ( col = 1; col <= 3; col++)
         r = r && (LHS[row][col] == RHS[row][col]);
   return( r );
}

//------------------------------------------------------------------
bool IsFeasibleMove(BOARD nextBoard,int nextRow0,int nextCol0,
                    BOARD     board,int     row0,int     col0)
//------------------------------------------------------------------
{
   int Metric(BOARD board,int row0,int col0);

   return( true );

}

//------------------------------------------------------------------
int Metric(BOARD board,int row0,int col0)
//------------------------------------------------------------------
{
   int metric = 0;
   int row, col;

   metric = abs(4-row0) + abs(4-col0);
   for ( row = 1; row <= 4; row++)
      for ( col = 1; col <= 4; col++)
      {
         int x = board[row][col];
         metric += abs( ((x-1)/4+1)-row);
         metric += abs( ((x-1)%4+1)-col);
      }

printf("%3d\n",metric);

   return( metric );
}

